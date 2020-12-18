#include "client.hpp"

extern uint16_t ownClientId;
client_t* waiting = nullptr;

void onClientJoined(client_t** clients, cmd_t* cmdIn) {
    if(findClient(clients, cmdIn->userID) != -1) { return; } //Client is already on the display

    int idx = -1;
    for(int i = 0; i < 14; i++) {
        if(clients[i]->id == 0) {
            clients[i]->id = cmdIn->userID;
            clients[i]->talkPower = cmdIn->talkPower;
            clients[i]->state = cmdIn->state;
            memcpy(clients[i]->name, cmdIn->data, MAX_STR_LEN + 1);

            clients[i]->lastAction.action = ACTION_JOIN;
            clients[i]->lastAction.actionStamp = millis();

            idx = i;
            break;
        }
    }

    if (idx != -1) {
        idx = sortClientList(clients);
        redrawUserListPartial(clients, idx, ownClientId);
    }
}

void onClientLeft(client_t** clients, uint16_t userId) {
    int idx = findClient(clients, userId);

    if(idx == -1) { return; }

    clients[idx]->lastAction.action = ACTION_LEAVE;
    clients[idx]->lastAction.actionStamp = millis();

    int line = findLine(clients, idx);
    printUserName(line, clients[idx], clients[idx]->id == ownClientId);
}

void onClientLeftFinally(client_t** clients, int idx) {
    if (idx != -1) {
        if (waiting != nullptr) {
            delete clients[idx];
            clients[idx] = waiting;
            waiting = nullptr;
        }
        else {
            clients[idx]->id = 0;
            clients[idx]->lastAction.action = ACTION_NONE;
        }

        int line = redrawUserListPartial(clients, idx, ownClientId);
        for (; line < 14; line++) {
            printUserName(line, nullptr, false);
            drawUserState(line, 255);
        }
    }
}

void onClientTalkState(client_t** clients, uint16_t userId, uint8_t state) {
    int idx = findClient(clients, userId);
    if(idx != -1) {
        if(state) { clients[idx]->state |= 0b0001u; }
        else { clients[idx]->state &= 0b1110u; }

        int line = findLine(clients, idx);
        drawUserState(line, clients[idx]->state);
    }
}

void onClientMicState(client_t** clients, uint16_t userId, uint8_t state) {
    int idx = findClient(clients, userId);
    if(idx != -1) {
        if(state) { clients[idx]->state |= 0b0010u; }
        else { clients[idx]->state &= 0b1101u; }

        int line = findLine(clients, idx);
        drawUserState(line, clients[idx]->state);
    }
}

void onClientSoundState(client_t** clients, uint16_t userId, uint8_t state) {
    int idx = findClient(clients, userId);
    if(idx != -1) {
        if(state) { clients[idx]->state |= 0b0100u; }
        else { clients[idx]->state &= 0b1011u; }

        int line = findLine(clients, idx);
        drawUserState(line, clients[idx]->state);
    }
}

void onClientAwayState(client_t** clients, uint16_t userId, uint8_t state) {
    int idx = findClient(clients, userId);
    if(idx != -1) {
        if(state) { clients[idx]->state |= 0b1000u; }
        else { clients[idx]->state &= 0b0111u; }

        int line = findLine(clients, idx);
        drawUserState(line, clients[idx]->state);
    }
}

void onClientRename(client_t** clients, cmd_t* cmdIn) {
    int idx = findClient(clients, cmdIn->userID);
    if(idx != -1) {
        memcpy(clients[idx]->name, cmdIn->data, MAX_STR_LEN + 1);

        int line = findLine(clients, idx);
        printUserName(line, clients[idx], clients[idx]->id == ownClientId);
    }
}



int findPlaceForClient(client_t** clients, cmd_t* cmdIn) {
    if(findClient(clients, cmdIn->userID) != -1) { return -1; }

    for(int i = 0; i < 14; i++) {
        if(clients[i]->id == 0) {
            clients[i]->id = cmdIn->userID;
            clients[i]->talkPower = cmdIn->talkPower;
            clients[i]->state = cmdIn->state;
            memcpy(clients[i]->name, cmdIn->data, MAX_STR_LEN + 1);
            return i;
        }
    }
    return -1;
}

void tryAppendClient(client_t** clients, cmd_t* cmdIn) {
    int idx = findPlaceForClient(clients, cmdIn);
    if(idx != -1) {
        idx = sortClientList(clients);

        int line = findLine(clients, idx);
        printUserName(line, clients[idx], clients[idx]->id == ownClientId);

        redrawUserListPartial(clients, idx, ownClientId);
    }
    else {
        waiting = new client_t();
        waiting->id = cmdIn->userID;
        waiting->talkPower = cmdIn->talkPower;
        waiting->state = cmdIn->state;
        memcpy(waiting->name, cmdIn->data, MAX_STR_LEN + 1);
    }
}



void checkActions(client_t** clients) {
    for(int i = 0; i < 14; i++) {
        if(clients[i]->id != 0) {
            switch (clients[i]->lastAction.action)
            {
                case ACTION_JOIN: {
                    if(millis() - clients[i]->lastAction.actionStamp > JOIN_TIME) {
                        clients[i]->lastAction.action = ACTION_NONE;

                        int line = findLine(clients, i);
                        printUserName(line, clients[i], clients[i]->id == ownClientId);
                    }
                    break;
                }

                case ACTION_LEAVE: {
                    if(millis() - clients[i]->lastAction.actionStamp > LEAVE_TIME) {
                        onClientLeftFinally(clients, i);
                    }
                    break;
                }

                default: break;
            }
        }
    }
}



int findClient(client_t** clients, uint16_t clientId) {
    for(int i = 0; i < 14; i++) {
        if(clients[i]->id == clientId) { return i; }
    }
    return -1;
}

int findLine(client_t** clients, int idx) {
    int line = 0;
    for(int i = 0; i < idx; i++) {
        if(clients[i]->id != 0) { line++; }
    }
    return line;
}