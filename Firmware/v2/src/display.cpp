#include "display.hpp"

#ifdef __IMXRT1062__
uint8_t dataPins[16] = { 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
ST7789 display = ST7789(TFT_RST, TFT_RD, TFT_WR, TFT_CS, TFT_DC, dataPins, TFT_BL);
#elif defined(__SAMD21G18A__) || defined(__SAMD51__)
#ifdef PCB_V1
ST7789 display = ST7789(TFT_RST, TFT_RD, TFT_WR, TFT_CS, TFT_DC, &PORT->Group[PORTA], 6, false, TFT_BL); //Initial PCB design
#else
ST7789 display = ST7789(TFT_RST, TFT_RD, TFT_WR, TFT_CS, TFT_DC, &PORT->Group[PORTA], 4, true, TFT_BL); //New PCB design
#endif
#endif

bool dispOn = false;
bool darkMode = false;
uint8_t brightness = 255;
uint16_t colorServerBar = 0;
uint16_t colorChannelBar = 0;
uint16_t colorUserFrame = TFT_ORANGE;
uint16_t colorUserBackground = 0;
uint16_t colorEntryBackground = 0;
uint16_t colorSelfBackground = 0;
uint16_t colorJoinBackground = TFT_GREENYELLOW;
uint16_t colorLeaveBackground = TFT_RED;
uint16_t colorText = 0;

void initDisplay() {
    display.begin(false);
#ifdef PCB_V1
    display.setRotation(2);
#endif

    configureColors();
    drawBackground();
}

bool switchDarkMode(bool dark) {
    if (dark != darkMode) {
        configureColors();
        drawBackground();

        return true;
    }
    return false;
}

void setDisplayBrightness(uint8_t percent) {
    auto factor = (float)percent;
    brightness = (uint32_t)(factor * 255.0);

    if (dispOn) {
        display.setBrightness(brightness);
    }
}

void configureColors() {
    if (darkMode) {
        colorServerBar = TFT_NAVY;
        colorChannelBar = TFT_DARKGREY;
        colorUserBackground = ST7789::color565(0x08, 0x08, 0x08);
        colorEntryBackground = ST7789::color565(0x1B, 0x1B, 0x1B);
        colorSelfBackground = TFT_DARKCYAN;
        colorText = TFT_WHITE;
    }
    else {
        colorServerBar = TFT_CYAN;
        colorChannelBar = TFT_LIGHTGREY;
        colorUserBackground = TFT_WHITE;
        colorEntryBackground = ST7789::color565(0xEC, 0xEC, 0xEC);
        colorSelfBackground = TFT_YELLOW;
        colorText = TFT_BLACK;
    }
}

void drawBackground() {
    display.fillRect(0, 0, 240, 18, colorServerBar);
    display.fillRect(0, 18, 240, 18, colorChannelBar);
    display.fillRect(0, 36, 240, 284, colorUserBackground);
    display.drawRect(0, 36, 240, 284, colorUserFrame);
    display.drawRect(1, 37, 238, 282, colorUserFrame);
}

void displayOn() {
    display.setDisplayOn(true);
    display.setBrightness(brightness);
    dispOn = true;
}

void displayOff() {
    display.setBrightness(0);
    display.setDisplayOn(false);
    dispOn = false;
}



void printServerName(char* name) {
    display.fillRect(0, 5, 240, 8, colorServerBar);

    int xOff = (240 - (5 + ((int)strlen(name) * 6))) / 2;
    display.setCursor(xOff, 5);
    display.setTextColor(colorText, colorServerBar);
    display.print(name);
}

void printChannelName(char* name) {
    display.fillRect(0, 23, 240, 8, colorChannelBar);

    int xOff = (240 - (5 + ((int)strlen(name) * 6))) / 2;
    display.setCursor(xOff, 23);
    display.setTextColor(colorText, colorChannelBar);
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
    uint16_t bgColor = ((line % 2) && validClient) ? colorEntryBackground : colorUserBackground;

    if(validClient) {
        if(isMe) { bgColor = colorSelfBackground; }
        else if(client->lastAction.action == ACTION_JOIN) { bgColor = colorJoinBackground; }
        else if(client->lastAction.action == ACTION_LEAVE) { bgColor = colorLeaveBackground; }
    }

    display.fillRect(22, yOff - 5, 215, 18, bgColor);

    if(validClient && strlen(client->name) > 0) {
        display.setCursor(26, yOff);
        display.setTextColor(colorText, bgColor);
        display.print(client->name);
    }
}

void drawUserState(int line, uint8_t state) {
    int yOff = (line * 20) + 40;

    if(state == 255) { // 255 as the new state indicates that the old symbol is no longer needed (client left)
        display.fillRect(4, yOff, 16, 16, colorUserBackground);
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
    display.fillRect(2, 38, 236, 280, colorUserBackground);
}