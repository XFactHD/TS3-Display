#include <display.hpp>

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
#ifdef USE_CANVAS
  GFXcanvas16 nameCanvas = GFXcanvas16(215, 18);
#endif

void initDisplay() {
  pinMode(TFT_BL, OUTPUT);

  display.begin();
  display.fillRect(0, 0, 240, 18, ILI9341_CYAN);
  display.fillRect(0, 18, 240, 18, ILI9341_LIGHTGREY);
  display.fillRect(0, 36, 240, 284, ILI9341_WHITE);
  display.drawRect(0, 36, 240, 284, ILI9341_ORANGE);
  display.drawRect(1, 37, 238, 282, ILI9341_ORANGE);
}

void displayOn() {
    display.SPI_CS_LOW();
    display.writeCommand(0x11);
    display.SPI_CS_HIGH();
    delay(5);
    analogWrite(TFT_BL, 120);
}

void displayOff() {
    analogWrite(TFT_BL, 0);
    display.SPI_CS_LOW();
    display.writeCommand(0x10);
    display.SPI_CS_HIGH();
}



void printServerName(char* name) {
    display.fillRect(0, 5, 240, 8, ILI9341_CYAN);

    int xOff = (240 - (5 + ((int)strlen(name) * 6))) / 2;
    display.setCursor(xOff, 5);
    display.setTextColor(ILI9341_BLACK, ILI9341_CYAN);
    display.print(name);
}

void printChannelName(char* name) {
    display.fillRect(0, 23, 240, 8, ILI9341_LIGHTGREY);

    int xOff = (240 - (5 + ((int)strlen(name) * 6))) / 2;
    display.setCursor(xOff, 23);
    display.setTextColor(ILI9341_BLACK, ILI9341_LIGHTGREY);
    display.print(name);
}



void redrawUserList(client_t** clients, uint16_t ownClientId) {
    int line = 0;
    for (int i = 0; i < 14; ++i) {
        if(clients[i]->id != 0) {
            printUserName(line, clients[i], clients[i]->id == ownClientId);
            drawUserState(line, 255, clients[i]->state);
            line++;
        }
    }

    for (; line < 14; line++) {
        printUserName(line, nullptr, false);
        drawUserState(line, 255, 255);
    }
}

int redrawUserListPartial(client_t** clients, int idx, uint16_t ownClientId) {
    int line = 0;
    for (int i = 0; i < 14; ++i) {
        if (clients[i]->id != 0) {
            if (i >= idx) {
                printUserName(line, clients[i], clients[i]->id == ownClientId);
                drawUserState(line, 255, clients[i]->state);
            }
            line++;
        }
    }
    return line;
}

void printUserName(uint8_t line, client_t* client, bool isMe) {
    int yOff = (line * 20) + 44;
    bool validClient = client != nullptr && client->id != 0;
    uint16_t bgColor = ((line % 2) && validClient) ? display.color565(0xEC, 0xEC, 0xEC) : ILI9341_WHITE;

    if(validClient) {
        if(isMe) { bgColor = ILI9341_YELLOW; }
        else if(client->lastAction.action == ACTION_JOIN) { bgColor = ILI9341_GREENYELLOW; }
        else if(client->lastAction.action == ACTION_LEAVE) { bgColor = ILI9341_RED; }
    }

    display.fillRect(22, yOff - 5, 215, 18, bgColor);

    if(validClient && strlen(client->name) > 0) {
        display.setCursor(26, yOff);
        display.setTextColor(ILI9341_BLACK, bgColor);
        display.print(client->name);
    }
}

void drawUserState(int line, uint8_t oldState, uint8_t state) {
    int yOff = (line * 20) + 44;

    if(((state & B0001) && oldState > 1) || (state == 0 && oldState > 1) || (state & B0010) || (state & B0100) || (state & B1000) || oldState == 255) {
        display.fillRect(4, yOff - 4, 16, 16, ILI9341_WHITE);
    }

    if(state == 255) { return; } // 255 as the new state indicates that the old symbol is no longer needed (client left)

    if(state & B1000) { //Away
        //Corpus
        display.fillRect(5, yOff + 4, 13, 7, ILI9341_BLUE);

        //Hanger
        display.drawLine( 7, yOff + 3, 11, yOff - 1, ILI9341_BLACK);
        display.drawLine(11, yOff - 1, 15, yOff + 3, ILI9341_BLACK);

        //Cross
        display.drawLine( 9, yOff + 5, 13, yOff + 9, ILI9341_WHITE);
        display.drawLine(13, yOff + 5,  9, yOff + 9, ILI9341_WHITE);
    }
    else if(state & B0100) { //Sound muted
        display.fillRect(5, yOff + 1, 8, 6, ILI9341_BLACK);
        display.fillTriangle(7, yOff + 3, 12, yOff - 2, 12, yOff + 9, ILI9341_BLACK);

        display.drawPixel(14, yOff + 1, ILI9341_BLACK);
        display.drawPixel(14, yOff + 2, ILI9341_BLACK);
        display.drawPixel(15, yOff + 3, ILI9341_BLACK);
        display.drawPixel(15, yOff + 4, ILI9341_BLACK);
        display.drawPixel(14, yOff + 5, ILI9341_BLACK);
        display.drawPixel(14, yOff + 6, ILI9341_BLACK);

        display.drawPixel(14, yOff - 2, ILI9341_BLACK);
        display.drawPixel(15, yOff - 1, ILI9341_BLACK);
        display.drawPixel(16, yOff + 0, ILI9341_BLACK);
        display.drawPixel(16, yOff + 1, ILI9341_BLACK);
        display.drawPixel(17, yOff + 2, ILI9341_BLACK);
        display.drawPixel(17, yOff + 3, ILI9341_BLACK);
        display.drawPixel(17, yOff + 4, ILI9341_BLACK);
        display.drawPixel(17, yOff + 5, ILI9341_BLACK);
        display.drawPixel(16, yOff + 6, ILI9341_BLACK);
        display.drawPixel(16, yOff + 7, ILI9341_BLACK);
        display.drawPixel(15, yOff + 8, ILI9341_BLACK);
        display.drawPixel(14, yOff + 9, ILI9341_BLACK);

        display.drawLine(6, yOff - 3, 18, yOff +  9, ILI9341_RED);
        display.drawLine(5, yOff - 2, 17, yOff + 10, ILI9341_RED);
        display.drawLine(5, yOff - 3, 18, yOff + 10, ILI9341_RED);
    }
    else if(state & B0010) { //Mic muted
        display.fillRoundRect(9, yOff - 3, 6, 9, 2, ILI9341_BLACK);
        display.drawFastVLine( 7, yOff + 4,  3, ILI9341_BLACK);
        display.drawFastVLine(16, yOff + 5,  3, ILI9341_BLACK);
        display.drawFastHLine( 7, yOff + 7, 10, ILI9341_BLACK);
        display.fillRect(11, yOff + 8, 2, 3, ILI9341_BLACK);

        display.drawLine(6, yOff - 3, 18, yOff +  9, ILI9341_RED);
        display.drawLine(5, yOff - 2, 17, yOff + 10, ILI9341_RED);
        display.drawLine(5, yOff - 3, 18, yOff + 10, ILI9341_RED);
    }
    else if(state & B0001) { //Talking
        display.fillRoundRect(5, yOff - 3, 14, 14, 4, ILI9341_LIGHTBLUE);
        display.drawRoundRect(5, yOff - 3, 14, 14, 4, ILI9341_BLUE);
    }
    else if(state == 0) { //Silent
        display.fillRoundRect(5, yOff - 3, 14, 14, 4, ILI9341_NAVY);
        display.drawRoundRect(5, yOff - 3, 14, 14, 4, ILI9341_BLUE);
    }
}

void clearClientList() {
    display.fillRect(2, 38, 236, 280, ILI9341_WHITE);
}