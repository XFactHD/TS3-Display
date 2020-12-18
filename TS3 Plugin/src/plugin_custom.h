#pragma once

#include <stdlib.h>
#include <string>
#include <sstream>
#include <Windows.h>

#include "gui/config.h"

#define MAX_STR_LEN 35 // TODO: check how many characters can be displayed
#define REMOTE_USER_LIMIT 14 // Max amount of users the display can show at once

#define CMD_CONNECT				0x10	// We connected to a server (own userId)
#define CMD_DISCONNECT			0x11	// We left the server (no args)
#define CMD_SERVER_NAME			0x20	// New servers name (server name)
#define CMD_CHANNEL_SWITCH		0x30	// We switch channel (channel name)
#define CMD_CHANNEL_JOIN		0x31	// Someone joined our channel or we joined a channel (userId, state, talkPower, user name)
#define CMD_CHANNEL_APPEND		0x32	// Someone who is already in our channel, can now be shown on the display
#define CMD_CHANNEL_LEAVE		0x33	// Someone left our channel (userId)
#define CMD_CLIENT_TALK			0x41	// Someone starts/stops speaking (userId, status)
#define CMD_CLIENT_MIC_MUTE		0x42	// Someone muted/unmuted their mic (userId, status)
#define CMD_CLIENT_SOUND_MUTE	0x43	// Someone muted/unmuted their sound (userId, status)
#define CMD_CLIENT_AWAY			0x44	// Someone went away or came back (userId, status)
#define CMD_CLIENT_RENAME		0x45	// Someone changed their name
#define CMD_DISP_ON				0x50	// Switch display backlight on
#define CMD_DISP_OFF			0x51	// Switch display backlight off
#define CMD_ACK					0x60	// Command acknowledge

#pragma pack(push, 1) //Causes the struct to not be padded for memory allignment
typedef struct {
	byte cmdID;					// Command to execute
	anyID userId;				// User id, may be unset, depending on command
	int groupId;				// Users channel group id, may be unset depending on command
	byte state;					// State info (talk, mic mute, sound mute, etc)
	char data[MAX_STR_LEN + 1];	// Server, channel or user name, if applicable
} cmd_t;
#pragma pack(pop)

typedef struct {
	int talking;
	int mic_mute;
	int sound_mute;
	int away;
} userState;



void selfChannelSwitch(uint64 serverConnectionHandlerID, uint64 newChannelID, anyID clientID);

void userJoinChannel(uint64 serverConnectionHandlerID, anyID clientID);

void userLeaveChannel(uint64 serverConnectionHandlerID, anyID clientID);



void readConfigFile();

void writeConfigFile();

void setNewConfigValues(std::string port, unsigned long baud);

void sanitizeName(char* name);

void enqueueCommand(unsigned char cmd, anyID userId, int groupId, unsigned char state, const char* data);

int waitForMutex(HANDLE mutex, DWORD ms, char* sourceThread);

userState getCurrentState(anyID userID);

byte packUserState(userState state);
