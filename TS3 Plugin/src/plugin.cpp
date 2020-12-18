#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#pragma warning (disable : 4100)  /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"

#include "plugin_custom.h"
#include "SerialPort.h"
#include <queue>
#include <map>
#include <vector>

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 23

#define PATH_BUFSIZE 512

using namespace std;

const char* LOG_MSG_CHANNEL = "SerialDisplay";

static char* pluginID = NULL;
char filePath[PATH_BUFSIZE];

volatile char pluginRunning = 0;
volatile char threadInitError = 0;
volatile char threadExited = 0;
HANDLE threadHandle;
HANDLE threadMutex;
char portName[64] = { 0 };
DWORD baudrate;

int connStatus = STATUS_DISCONNECTED;
uint64 schID = 0;
anyID ownClientID = 0;
uint64 ownChannel = 0;
map<anyID, userState> users; //Contains the users in our channel
vector<anyID> remoteUsers; //Contains the users that were sent to the display
queue<cmd_t> cmdBacklog; //Contains the commands that are waiting to be enqueued
queue<cmd_t> cmdQueue; //Contains the commands that are currently enqueued and waiting to be transmitted

config* cfg;

/* Unique name identifying this plugin */
const char* ts3plugin_name() {
	return "SerialDisplay";
}

/* Plugin version */
const char* ts3plugin_version() {
    return "1.0";
}

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */
int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

/* Plugin author */
const char* ts3plugin_author() {
	return "XFactHD";
}

/* Plugin description */
const char* ts3plugin_description() {
	return "This plugin sends info about your current channel to a serial device.";
}

/* Set TeamSpeak 3 callback functions */
void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
    ts3Functions = funcs;
}

/*
 * Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
 * If the function returns 1 on failure, the plugin will be unloaded again.
 */
int ts3plugin_init() {
	char configPath[PATH_BUFSIZE];
	ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	sprintf_s(filePath, "%s%s", configPath, "ts3_serial_display.ini");

	readConfigFile();

	threadMutex = CreateMutex(NULL, FALSE, NULL);
	if (threadMutex == NULL) { return 1; }
	ts3Functions.logMessage("Mutex created", LogLevel_INFO, LOG_MSG_CHANNEL, 0);

	threadHandle = CreateThread(NULL, 0, outputThread, NULL, 0, NULL);
	if (threadHandle == NULL) { return 1; }
	ts3Functions.logMessage("Thread created", LogLevel_INFO, LOG_MSG_CHANNEL, 0);
	while (!pluginRunning) {
		if (threadInitError) {
			CloseHandle(threadHandle);
			CloseHandle(threadMutex);
			return 1;
		}
	}
	ts3Functions.logMessage("Thread started", LogLevel_INFO, LOG_MSG_CHANNEL, 0);

	return 0;  /* 0 = success, 1 = failure, -2 = failure but client will not show a "failed to load" warning */
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
	if (pluginRunning) { //When pluginRunning is 0, the thread never fully started, which means there is no thread to stop
	/* Your plugin cleanup code here */
		ts3Functions.logMessage("Stopping thread", LogLevel_INFO, LOG_MSG_CHANNEL, 0);
		pluginRunning = 0;
		while (!threadExited) { Sleep(1); }

		ts3Functions.logMessage("Thread stopped", LogLevel_INFO, LOG_MSG_CHANNEL, 0);
	}
	if (threadHandle != NULL) { CloseHandle(threadHandle); }
	if (threadMutex != NULL) { CloseHandle(threadMutex); }
	ts3Functions.logMessage("Thread cleaned up", LogLevel_INFO, LOG_MSG_CHANNEL, 0);

	/* Free pluginID if we registered it */
	if(pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

/* Tell client if plugin offers a configuration window. If this function is not implemented, it's an assumed "does not offer" (PLUGIN_OFFERS_NO_CONFIGURE). */
int ts3plugin_offersConfigure() {
	printf("PLUGIN: offersConfigure\n");
	/*
	 * Return values:
	 * PLUGIN_OFFERS_NO_CONFIGURE         - Plugin does not implement ts3plugin_configure
	 * PLUGIN_OFFERS_CONFIGURE_NEW_THREAD - Plugin does implement ts3plugin_configure and requests to run this function in an own thread
	 * PLUGIN_OFFERS_CONFIGURE_QT_THREAD  - Plugin does implement ts3plugin_configure and requests to run this function in the Qt GUI thread
	 */
	//return PLUGIN_OFFERS_NO_CONFIGURE;  /* In this case ts3plugin_configure does not need to be implemented */ //TODO: offer qt configure
	return PLUGIN_OFFERS_CONFIGURE_QT_THREAD;
}

void ts3plugin_configure(void* handle, void* qParentWidget) {
	cfg = new config((QWidget*)qParentWidget, setNewConfigValues, string(portName).substr(4), baudrate);
	cfg->show();
}

/*
 * If the plugin wants to use error return codes, plugin commands, hotkeys or menu items, it needs to register a command ID. This function will be
 * automatically called after the plugin was initialized. This function is optional. If you don't use these features, this function can be omitted.
 * Note the passed pluginID parameter is no longer valid after calling this function, so you must copy it and store it in the plugin.
 */
void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	if (pluginID == NULL) { return; }
	_strcpy(pluginID, sz, id);  /* The id buffer will invalidate after exiting this function */
	printf("PLUGIN: registerPluginID: %s\n", pluginID);
}

/* Plugin command keyword. Return NULL or "" if not used. */
const char* ts3plugin_commandKeyword() {
	return NULL;
}

/* Required to release the memory for parameter "data" allocated in ts3plugin_infoData and ts3plugin_initMenus */
void ts3plugin_freeMemory(void* data) {
	free(data);
}

/*
 * Plugin requests to be always automatically loaded by the TeamSpeak 3 client unless
 * the user manually disabled it in the plugin dialog.
 * This function is optional. If missing, no autoload is assumed.
 */
int ts3plugin_requestAutoload() {
	return 1;  /* 1 = request autoloaded, 0 = do not request autoload */
}

				

/*
 * Called when our connection status changed
 */
void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber) {
	if ((schID != 0 && serverConnectionHandlerID != schID) || !pluginRunning) { return; }

	if (connStatus != newStatus) {
		if (newStatus == STATUS_CONNECTED) {
			schID = serverConnectionHandlerID;
			ts3Functions.logMessage("[TS3 Callback] Status: connected", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);

			unsigned int error = ts3Functions.getClientID(serverConnectionHandlerID, &ownClientID);
			if (error != ERROR_ok) {
				ts3Functions.logMessage("[TS3 Callback] Error while querying own client ID!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
				//TODO: maybe print error message
				return; // If we can't get our own client ID, we may aswell fail completely
			}

			enqueueCommand(CMD_CONNECT, ownClientID, 0, 0, NULL);

			char* serverName;
			error = ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_NAME, &serverName);
			if (error == ERROR_ok) { sanitizeName(serverName); }
			enqueueCommand(CMD_SERVER_NAME, 0, 0, 0, error == ERROR_ok ? serverName : "<unknown>");
			if (error == ERROR_ok) { ts3Functions.freeMemory(serverName); }
		}
		else if (newStatus == STATUS_CONNECTION_ESTABLISHED) {
			ts3Functions.logMessage("[TS3 Callback] Status: connection established", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);

			unsigned int error = ts3Functions.getChannelOfClient(serverConnectionHandlerID, ownClientID, &ownChannel);
			if (error != ERROR_ok) { return; }

			selfChannelSwitch(serverConnectionHandlerID, ownChannel, ownClientID);
		}
		else if (newStatus == STATUS_DISCONNECTED) {
			schID = 0;
			ts3Functions.logMessage("[TS3 Callback] Status: disconnected", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
			enqueueCommand(CMD_DISCONNECT, 0, 0, 0, NULL);
		}
		connStatus = newStatus;
	}
}

/*
 * Called when a client moves by himself (joining or leaving channels, connecting or disconnecting from the server)
 */
void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage) {
	if (connStatus != STATUS_CONNECTION_ESTABLISHED || serverConnectionHandlerID != schID || !pluginRunning) { return; }

	char msg[512];
	sprintf_s(msg, "Client ID: %ud, Old channel: %lld, New channel: %lld, Message: %s", clientID, oldChannelID, newChannelID, moveMessage);
	ts3Functions.logMessage(msg, LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);

	if (clientID == ownClientID) { // We switched channel => invalidate user list, change active channel name
		selfChannelSwitch(serverConnectionHandlerID, newChannelID, clientID);
	}
	else if (oldChannelID == ownChannel) { // Someone left our channel => remove them from the list
		userLeaveChannel(serverConnectionHandlerID, clientID);
	}
	else if (newChannelID == ownChannel) { // Someone joined our channel => add them to the list
		userJoinChannel(serverConnectionHandlerID, clientID);
	}
}

/*
 * Called when a client gets moved by another client
 */
void ts3plugin_onClientMoveMovedEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID moverID, const char* moverName, const char* moverUniqueIdentifier, const char* moveMessage) {
	ts3plugin_onClientMoveEvent(serverConnectionHandlerID, clientID, oldChannelID, newChannelID, visibility, moveMessage); // TODO: check if this is enough or if I need to copy all of this functions code to here
}

/*
 * Called when another client looses connection to the server
 */
void ts3plugin_onClientMoveTimeoutEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* timeoutMessage) {
	ts3plugin_onClientMoveEvent(serverConnectionHandlerID, clientID, oldChannelID, newChannelID, visibility, timeoutMessage);
}

/*
 * Called when someone starts or stops talking
 */
void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID) {
	if (connStatus == STATUS_DISCONNECTED || serverConnectionHandlerID != schID || !pluginRunning) { return; }

	map<anyID, userState>::iterator entry = users.find(clientID);
	if (entry != users.end()) { // The client is in our channel
		/*char msg[512];
		sprintf_s(msg, "[TS3 Callback] A client got a talk update! Our ID: %d, his ID: %d", ownClientID, clientID);
		ts3Functions.logMessage(msg, LogLevel_INFO, LOG_MSG_CHANNEL, 0);*/

		if (entry->second.talking != (status == STATUS_TALKING)) {
			entry->second.talking = (status == STATUS_TALKING);

			enqueueCommand(CMD_CLIENT_TALK, clientID, 0, status == STATUS_TALKING, NULL);
			ts3Functions.logMessage("[TS3 Callback] Status: someone started or stopped talking", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
		}
	}
}

/*
 * Called for all kinds of client state changes
 */
void ts3plugin_onUpdateClientEvent(uint64 serverConnectionHandlerID, anyID clientID, anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier) {
	if (connStatus == STATUS_DISCONNECTED || serverConnectionHandlerID != schID || !pluginRunning) { return; }

	/*char msg[512];
	sprintf_s(msg, "[TS3 Callback] A client got an update! Our ID: %d, his ID: %d", ownClientID, clientID);
	ts3Functions.logMessage(msg, LogLevel_INFO, LOG_MSG_CHANNEL, 0);*/

	map<anyID, userState>::iterator entry = users.find(clientID);
	if (entry != users.end()) { // We care about this client because he is in our channel
		int micMute = 0;
		int soundMute = 0;
		int away = 0;

		/*char msg[512];
		sprintf_s(msg, "[TS3 Callback] A client got an update! Our ID: %d, his ID: %d", ownClientID, clientID);
		ts3Functions.logMessage(msg, LogLevel_INFO, LOG_MSG_CHANNEL, 0);*/

		ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_INPUT_MUTED, &micMute);
		ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_OUTPUT_MUTED, &soundMute);
		ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_AWAY, &away);

		if (entry->second.mic_mute != micMute) {
			entry->second.mic_mute = micMute;

			enqueueCommand(CMD_CLIENT_MIC_MUTE, clientID, 0, micMute, NULL);
			ts3Functions.logMessage("[TS3 Callback] Status: someone muted or unmuted their mic", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
		}

		if (entry->second.sound_mute != soundMute) {
			entry->second.sound_mute = soundMute;

			enqueueCommand(CMD_CLIENT_SOUND_MUTE, clientID, 0, soundMute, NULL);
			ts3Functions.logMessage("[TS3 Callback] Status: someone muted or unmuted their sound", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
		}

		if (entry->second.away != away) {
			entry->second.away = away;

			enqueueCommand(CMD_CLIENT_AWAY, clientID, 0, away, NULL);
			ts3Functions.logMessage("[TS3 Callback] Status: someone went away or came back", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
		}
	}
}

/*
 * Called when a client gets kicked from the channel
 */
void ts3plugin_onClientKickFromChannelEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID kickerID, const char* kickerName, const char* kickerUniqueIdentifier, const char* kickMessage) {
	if (connStatus == STATUS_DISCONNECTED || serverConnectionHandlerID != schID || !pluginRunning) { return; }

	if (clientID == ownClientID) { // We switched channel => invalidate user list, change active channel name
		selfChannelSwitch(serverConnectionHandlerID, newChannelID, clientID);
	}
	else if (oldChannelID == ownChannel) { // Someone left our channel => remove them from the list
		userLeaveChannel(serverConnectionHandlerID, clientID);
	}
}

/*
 * Called when a client gets kicked from the server
 */
void ts3plugin_onClientKickFromServerEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, anyID kickerID, const char* kickerName, const char* kickerUniqueIdentifier, const char* kickMessage) {
	if (connStatus == STATUS_DISCONNECTED || serverConnectionHandlerID != schID || !pluginRunning) { return; }

	if (clientID == ownClientID) { // We switched channel => invalidate user list, change active channel name
		selfChannelSwitch(serverConnectionHandlerID, newChannelID, clientID);
	}
	else if (oldChannelID == ownChannel) { // Someone left our channel => remove them from the list
		userLeaveChannel(serverConnectionHandlerID, clientID);
	}
}

/*
 * Called when a client changes his name
 */
void ts3plugin_onClientDisplayNameChanged(uint64 serverConnectionHandlerID, anyID clientID, const char* displayName, const char* uniqueClientIdentifier) {
	if (connStatus == STATUS_DISCONNECTED || serverConnectionHandlerID != schID || !pluginRunning) { return; }

	if (users.find(clientID) != users.end()) { //User is in our channel => change name
		enqueueCommand(CMD_CLIENT_RENAME, clientID, 0, 0, displayName);
	}
}



void selfChannelSwitch(uint64 serverConnectionHandlerID, uint64 newChannelID, anyID clientID) {
	/*char msg[512];
	sprintf_s(msg, "[TS3 Callback] Switched from channel %lld to channel %lld", oldChannelID, newChannelID);
	ts3Functions.logMessage(msg, LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);*/

	users.clear();
	remoteUsers.clear();
	ownChannel = newChannelID;

	if (newChannelID == 0) { return; } // Channel ID 0 is the "fallback" value for disconnecting from the server

	char* chanName;
	unsigned int error = ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, newChannelID, CHANNEL_NAME, &chanName);
	if (error == ERROR_ok) { sanitizeName(chanName); }
	enqueueCommand(CMD_CHANNEL_SWITCH, 0, 0, 0, error == ERROR_ok ? chanName : "<unknown>");
	ts3Functions.freeMemory(chanName);

	anyID* newUsers;
	error = ts3Functions.getChannelClientList(serverConnectionHandlerID, newChannelID, &newUsers);
	if (error == ERROR_ok) {
		for (int i = 0; newUsers[i] != NULL; i++) {
			userState state = getCurrentState(newUsers[i]);
			users.insert(pair<anyID, userState>(newUsers[i], state));
			
			char userName[MAX_STR_LEN + 1];
			unsigned int error = ts3Functions.getClientDisplayName(serverConnectionHandlerID, newUsers[i], userName, MAX_STR_LEN);
			if (error == ERROR_ok) { sanitizeName(userName); }

			int talkPower = 0;
			error = ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, newUsers[i], CLIENT_TALK_POWER, &talkPower);
			if (error != ERROR_ok) {
				char msg[512];
				sprintf_s(msg, "Failed to get talk power from user with id %d, returned %d with error code %d", newUsers[i], talkPower, error);
				ts3Functions.logMessage(msg, LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
				talkPower = 0;
			}

			char msg[512];
			sprintf_s(msg, "Joined channel with id %lld, adding client %s with id %d to list", newChannelID, error == ERROR_ok ? userName : "<unknown>", newUsers[i]);
			ts3Functions.logMessage(msg, LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);

			if (remoteUsers.size() < 14) {
				remoteUsers.push_back(newUsers[i]);
				
				enqueueCommand(CMD_CHANNEL_JOIN, newUsers[i], talkPower, packUserState(state), error == ERROR_ok ? userName : "<unknown>");
			}
		}
		ts3Functions.freeMemory(newUsers);
	}
}

void userJoinChannel(uint64 serverConnectionHandlerID, anyID clientID) {
	userState state = getCurrentState(clientID);
	users.insert(pair<anyID, userState>(clientID, state));

	char userName[MAX_STR_LEN + 1];
	unsigned int error = ts3Functions.getClientDisplayName(serverConnectionHandlerID, clientID, userName, MAX_STR_LEN);
	if (error == ERROR_ok) { sanitizeName(userName); }

	int talkPower = 0;
	unsigned int ignore = ts3Functions.getClientVariableAsInt(serverConnectionHandlerID, clientID, CLIENT_TALK_POWER, &talkPower);

	if (remoteUsers.size() < 14) {
		remoteUsers.push_back(clientID);

		enqueueCommand(CMD_CHANNEL_JOIN, clientID, talkPower, packUserState(state), error == ERROR_ok ? userName : "<unknown>");
	}

	ts3Functions.logMessage("[TS3 Callback] Status: someone joined our channel", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
}

void userLeaveChannel(uint64 serverConnectionHandlerID, anyID clientID) {
	users.erase(clientID);
	
	vector<anyID>::iterator pos = find(remoteUsers.begin(), remoteUsers.end(), clientID);
	if (pos != remoteUsers.end()) { remoteUsers.erase(pos); }

	enqueueCommand(CMD_CHANNEL_LEAVE, clientID, 0, 0, NULL);
	ts3Functions.logMessage("[TS3 Callback] Status: someone left our channel", LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
}

//Checks if there are users in our channel that are not being displayed on the display
void checkAppendExistingUser() {
	if (remoteUsers.size() >= 14) { return; }

	for (map<anyID, userState>::iterator it = users.begin(); it != users.end(); it++) {
		if (remoteUsers.size() >= 14) { break; }

		anyID user = (*it).first;
		vector<anyID>::iterator pos = find(remoteUsers.begin(), remoteUsers.end(), user);
		if (pos == remoteUsers.end()) {
			remoteUsers.push_back(user);

			int talkPower = 0;
			unsigned int ignore = ts3Functions.getClientVariableAsInt(schID, user, CLIENT_TALK_POWER, &talkPower);

			char userName[MAX_STR_LEN + 1];
			unsigned int error = ts3Functions.getClientDisplayName(schID, user, userName, MAX_STR_LEN);

			userState state = (*it).second;
			enqueueCommand(CMD_CHANNEL_APPEND, user, talkPower, packUserState(state), error == ERROR_ok ? userName : "<unknown>");
		}
	}
}



void readConfigFile() {
	HANDLE file = CreateFileA(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_NOT_FOUND) {
		file = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (GetLastError() != ERROR_SUCCESS) {
			ts3Functions.logMessage("Failed to create config file!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
			return;
		}

		char buffer[] = "[General]\nport=COM7\nbaud=1000000";
		DWORD bytesToWrite = (DWORD)strlen(buffer);
		DWORD bytesWritten;
		BOOL success = WriteFile(file, buffer, bytesToWrite, &bytesWritten, NULL);
		if (!success || bytesWritten != bytesToWrite) {
			ts3Functions.logMessage("Failed to initialize config file!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
		}

		sprintf_s(portName, "\\\\.\\COM7");
		baudrate = 1000000;
	}
	else {
		char buffer[512] = { 0 };
		DWORD bytesRead;
		if (ReadFile(file, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
			stringstream content;
			content << buffer;

			string portText;
			string baudText;
			content.ignore(512, '\n');
			content >> portText >> baudText;

			sprintf_s(portName, "\\\\.\\%s", portText.substr(portText.find_first_of('=') + 1).c_str());
			baudrate = stol(baudText.substr(baudText.find_first_of('=') + 1));
		}
		else { ts3Functions.logMessage("Failed to read config file!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0); }
	}
	CloseHandle(file);
}

void writeConfigFile() {
	HANDLE file = CreateFileA(filePath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	char buffer[512] = { 0 };
	sprintf_s(buffer, "[General]\nport=%s\nbaud=%lu", portName + 4, baudrate);

	DWORD bytesToWrite = (DWORD)strlen(buffer);
	DWORD bytesWritten;
	BOOL success = WriteFile(file, buffer, bytesToWrite, &bytesWritten, NULL);
	if (!success || bytesWritten != bytesToWrite) {
		char text[128] = "";
		sprintf_s(text, "Failed to write config file! Error: 0x%03X", GetLastError());
		ts3Functions.logMessage(text, LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
	}

	CloseHandle(file);
}

void setNewConfigValues(string port, unsigned long baud) {
	delete cfg;

	if (!port.empty()) { sprintf_s(portName, "\\\\.\\%s", port.c_str()); }
	if (baud != 0) { baudrate = baud; }
	
	writeConfigFile();
}

void enqueueCommand(unsigned char cmdCode, anyID userId, int groupId, unsigned char state, const char* data) {
	cmd_t newCmd = { cmdCode, userId, groupId, state, "" };
	if (data != NULL) {
		size_t len = strlen(data);
		memcpy(newCmd.data, data, min(len, MAX_STR_LEN + 1));
		if (len > MAX_STR_LEN) {
			int offset = MAX_STR_LEN - 3;
			memcpy(newCmd.data + offset, "...", 4);
		}
	}

	// Wait for thread lock
	if (waitForMutex(threadMutex, 10, "TS3 Callback")) {
		//If the mutex is currently locked, put the command on the backlog
		cmdBacklog.push(newCmd);
		return;
	}
	
	while(!cmdBacklog.empty()) {
		cmdQueue.push(cmdBacklog.front());
		cmdBacklog.pop();
	}
	cmdQueue.push(newCmd);
	ReleaseMutex(threadMutex); // Release thread lock
}

//Returns 1 if an error occurs
int waitForMutex(HANDLE mutex, DWORD ms, char* sourceThread) {
	DWORD waitResult = WaitForSingleObject(mutex, ms);
	if (waitResult != WAIT_OBJECT_0) {
		if (waitResult == WAIT_ABANDONED) {
			char msg[512];
			sprintf_s(msg, "[%s] Thread Mutex was abandoned, this is a bug!", sourceThread);
			ts3Functions.logMessage(msg, LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
		}
		return 1;
	}
	return 0;
}

userState getCurrentState(anyID userID) {
	userState state = {};

	ts3Functions.getClientVariableAsInt(schID, userID, CLIENT_FLAG_TALKING, &state.talking);
	ts3Functions.getClientVariableAsInt(schID, userID, CLIENT_INPUT_MUTED, &state.mic_mute);
	ts3Functions.getClientVariableAsInt(schID, userID, CLIENT_OUTPUT_MUTED, &state.sound_mute);
	ts3Functions.getClientVariableAsInt(schID, userID, CLIENT_AWAY, &state.away);

	return state;
}

byte packUserState(userState state) {
	byte result = 0;

	result |= state.talking;
	result |= (state.mic_mute << 1);
	result |= (state.sound_mute << 2);
	result |= (state.away << 3);

	return result;
}

void sanitizeName(char* name) {
	int lastGoodSymbol = -1;
	for (unsigned int i = 0; name[i] != '\0'; i++) {
		while (!((name[i] >= 32 && name[i] <= 126) || name[i] == '\0')) {
			unsigned int j;
			for (j = i; name[j] != '\0'; j++) {
				name[j] = name[j + 1];
			}
			name[j] = '\0';
		}
	}
}



bool waitForAcknowledge(SerialPort* com, int timeout = 200) {
	int counter = 0;
	char buffer;
	do {
		Sleep(10);
		com->readSerialPort(&buffer, 1);
		counter++;
	} while (buffer != CMD_ACK && counter < timeout);

	if (counter >= timeout) { ts3Functions.logMessage("[OutputThread] Serial acknowledge timed out!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0); }
	return counter < 200;
}

DWORD WINAPI outputThread(LPVOID lpParam) {
	SerialPort* com = new SerialPort(portName, baudrate);
	char out[8 + MAX_STR_LEN + 1] = { 0 }; //Output data buffer

	if (!com->isConnected()) {
		threadInitError = 1;
		ts3Functions.logMessage("[OutputThread] Failed to open serial port! Is the device attached?", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
		return 1;
	}
	
	pluginRunning = 1;

	//Send dummy byte to cause the MCU to wake up
	char dummy = 0;
	if (!com->writeSerialPort(&dummy, sizeof(dummy))) {
		ts3Functions.logMessage("[OutputThread] Failed to write start command to serial port!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
		pluginRunning = 0;
	}
	if (pluginRunning && !waitForAcknowledge(com, 500)) { //Longer timeout to make sure the MCU has time to wake up
		ts3Functions.logMessage("[OutputThread] Start command acknowledge timed out, shutting down output thread!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
		pluginRunning = 0;
	}

	if (pluginRunning) {
		enqueueCommand(CMD_DISP_ON, 0, 0, 0, "");

		//Send display on after wake up
		out[0] = CMD_DISP_ON;
		com->writeSerialPort(out, sizeof(out));
		waitForAcknowledge(com);
	}

	while (pluginRunning) {
		if (!cmdQueue.empty()) {
			if (waitForMutex(threadMutex, INFINITE, "OutputThread")) { continue; }

			cmd_t toSend = cmdQueue.front();
			out[0] = toSend.cmdID;
			memcpy(out + 1, &toSend.userId, 2);
			memcpy(out + 3, &toSend.groupId, 4);
			out[7] = toSend.state;
			memcpy(out + 8, toSend.data, MAX_STR_LEN + 1);

			ReleaseMutex(threadMutex);

			if (com->writeSerialPort((char*)&toSend, sizeof(cmd_t))) {
			//if (com->writeSerialPort(out, sizeof(out))) {
				char msg[512];
				sprintf_s(msg, "[OutputThread] Sent cmd 0x%x to serial port!", toSend.cmdID);
				ts3Functions.logMessage(msg, LogLevel_DEBUG, LOG_MSG_CHANNEL, 0);
				if (waitForMutex(threadMutex, INFINITE, "OutputThread")) { continue; }
				cmdQueue.pop(); // Only pop the command off the queue after it was actually sent
				ReleaseMutex(threadMutex);

				// Wait for cmd execution to finish
				if (!waitForAcknowledge(com)) {
					ts3Functions.logMessage("[OutputThread] Command acknowledge timed out!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
				}
			}
			else {
				ts3Functions.logMessage("[OutputThread] Failed to write to serial port!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);

				//If the serial port is not connected anymore, shut down to prevent spamming the log
				if (!com->isConnected()) {
					pluginRunning = 0;
				}
			}
		}
		Sleep(100);
	}

	out[0] = CMD_DISP_OFF;
	memset(out + 1, 0, sizeof(out) - 1);
	if (!com->writeSerialPort(out, sizeof(out))) {
		ts3Functions.logMessage("[OutputThread] Failed to write stop command to serial port!", LogLevel_ERROR, LOG_MSG_CHANNEL, 0);
	}
	Sleep(50);

	if (com->isConnected()) { com->closeSerial(); }

	threadExited = 1;
	return 0;
}
