#include "display.hpp"

#ifdef __IMXRT1062__
uint8_t data[16] = { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
ST7789 display = ST7789(TFT_RST, TFT_RD, TFT_WR, TFT_CS, TFT_DC, data, TFT_BL);
#elif defined(__SAMD21G18A__) || defined(__SAMD51__)
ST7789 display = ST7789(TFT_RST, TFT_RD, TFT_WR, TFT_CS, TFT_DC, &PORT->Group[PORTA], 6, TFT_BL);
#endif

void initDisplay() {
    display.begin(false);
    display.fillRect(0,  0, 240,  18, TFT_CYAN);
    display.fillRect(0, 18, 240,  18, TFT_LIGHTGREY);
    display.fillRect(0, 36, 240, 284, TFT_WHITE);
    display.drawRect(0, 36, 240, 284, TFT_ORANGE);
    display.drawRect(1, 37, 238, 282, TFT_ORANGE);
}

void displayOn() {
    display.setDisplayOn(true);
    display.setBrightness(180);
}

void displayOff() {
    display.setBrightness(0);
    display.setDisplayOn(false);
}



void printServerName(char* name) {
    display.fillRect(0, 5, 240, 8, TFT_CYAN);

    int xOff = (240 - (5 + ((int)strlen(name) * 6))) / 2;
    display.setCursor(xOff, 5);
    display.setTextColor(TFT_BLACK, TFT_CYAN);
    display.print(name);
}

void printChannelName(char* name) {
    display.fillRect(0, 23, 240, 8, TFT_LIGHTGREY);

    int xOff = (240 - (5 + ((int)strlen(name) * 6))) / 2;
    display.setCursor(xOff, 23);
    display.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
    display.print(name);
}



void redrawUserList(client_t** clients, uint16_t ownClientId) {
    int line = 0;
    for (int i = 0; i < 14; ++i) {
        if(clients[i]->id != 0) {
            printUserName(line, clients[i], clients[i]->id == ownClientId);
            drawUserState(line, clients[i]->state);
            line++;
        }
    }

    for (; line < 14; line++) {
        printUserName(line, nullptr, false);
        drawUserState(line, 255);
    }
}

int redrawUserListPartial(client_t** clients, int idx, uint16_t ownClientId) {
    int line = 0;
    for (int i = 0; i < 14; ++i) {
        if (clients[i]->id != 0) {
            if (i >= idx) {
                printUserName(line, clients[i], clients[i]->id == ownClientId);
                drawUserState(line, clients[i]->state);
            }
            line++;
        }
    }
    return line;
}

void printUserName(uint8_t line, client_t* client, bool isMe) {
    int yOff = (line * 20) + 44;
    bool validClient = client != nullptr && client->id != 0;
    uint16_t bgColor = ((line % 2) && validClient) ? ST7789::color565(0xEC, 0xEC, 0xEC) : TFT_WHITE;

    if(validClient) {
        if(isMe) { bgColor = TFT_YELLOW; }
        else if(client->lastAction.action == ACTION_JOIN) { bgColor = TFT_GREENYELLOW; }
        else if(client->lastAction.action == ACTION_LEAVE) { bgColor = TFT_RED; }
    }

    display.fillRect(22, yOff - 5, 215, 18, bgColor);

    if(validClient && strlen(client->name) > 0) {
        display.setCursor(26, yOff);
        display.setTextColor(TFT_BLACK, bgColor);
        display.print(client->name);
    }
}

void drawUserState(int line, uint8_t state) {
    int yOff = (line * 20) + 40;

    if(state == 255) { // 255 as the new state indicates that the old symbol is no longer needed (client left)
        display.fillRect(4, yOff, 16, 16, TFT_WHITE);
    }
    else if(state & 0b1000u) { //Away
        display.drawRGBBitmap(4, yOff, icons[ICON_AWAY], 16, 16);
    }
    else if(state & 0b0100u) { //Sound muted
        display.drawRGBBitmap(4, yOff, icons[ICON_SOUND_MUTED], 16, 16);
    }
    else if(state & 0b0010u) { //Mic muted
        display.drawRGBBitmap(4, yOff, icons[ICON_MIC_MUTED], 16, 16);
    }
    else if(state & 0b0001u) { //Talking
        display.drawRGBBitmap(4, yOff, icons[ICON_TALKING], 16, 16);
    }
    else if(state == 0) { //Silent
        display.drawRGBBitmap(4, yOff, icons[ICON_SILENT], 16, 16);
    }
}

void clearClientList() {
    display.fillRect(2, 38, 236, 280, TFT_WHITE);
}