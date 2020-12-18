#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_ILI9341.h>

#define MAX_STR_LEN 35

#include <client.hpp>

#define TFT_CS  9
#define TFT_DC  A1
#define TFT_RST A0
#define TFT_BL  10

#define ILI9341_LIGHTBLUE   0x025F  ///<   0,  73, 255 0049FF 00000000 01001001 11111111 0000001001011111

//#define USE_CANVAS

class client_t;

void initDisplay();
void displayOn();
void displayOff();

void printServerName(char* name);
void printChannelName(char* name);
void redrawUserList(client_t** clients, uint16_t ownClientId);
int redrawUserListPartial(client_t** clients, int idx, uint16_t ownClientId);

void printUserName(uint8_t line, client_t* client, bool isMe);
void drawUserState(int line, uint8_t oldState, uint8_t state);
void clearClientList();

#endif