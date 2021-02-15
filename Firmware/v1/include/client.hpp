#ifndef CLIENT_H
#define CLIENT_H

#include <Arduino.h>

#include <display.hpp>
#include <cmd.hpp>
#include <sort.hpp>

#define JOIN_TIME 1300
#define LEAVE_TIME 1300

typedef enum {
  ACTION_NONE,
  ACTION_JOIN,
  ACTION_LEAVE
} action_type_t;

typedef struct {
  action_type_t action = ACTION_NONE;
  unsigned long actionStamp = 0;
} action_t;

class client_t {
  public:
    uint16_t id = 0;
    int32_t talkPower = 0;
    uint8_t state;
    char name[MAX_STR_LEN + 1];
    action_t lastAction;
};

void disconnect(client_t** clients);
void onClientJoined(client_t** clients, cmd_t* cmdIn);
void onClientLeft(client_t** clients, uint16_t userId);
void onClientTalkState(client_t** clients, uint16_t userId, uint8_t state);
void onClientMicState(client_t** clients, uint16_t userId, uint8_t state);
void onClientSoundState(client_t** clients, uint16_t userId, uint8_t state);
void onClientAwayState(client_t** clients, uint16_t userId, uint8_t state);
void onClientRename(client_t** clients, cmd_t* cmdIn);

void tryAppendClient(client_t** clients, cmd_t* cmdIn);

void checkActions(client_t** clients);

int findClient(client_t** clients, uint16_t clientId);
int findLine(client_t** clients, int idx);

void checkActions(client_t** clients);

#endif