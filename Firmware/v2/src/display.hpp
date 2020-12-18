#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include "ST7789_16bit.hpp"

#include "client.hpp"
#include "icons.hpp"

#define TFT_RST 0
#define TFT_RD  17
#define TFT_WR  18
#define TFT_CS  20
#define TFT_DC  19
#define TFT_BL  255 //TODO: set properly on production hardware

class client_t;

void initDisplay();
void displayOn();
void displayOff();

void printServerName(char* name);
void printChannelName(char* name);
void redrawUserList(client_t** clients, uint16_t ownClientId);
int redrawUserListPartial(client_t** clients, int idx, uint16_t ownClientId);

void printUserName(uint8_t line, client_t* client, bool isMe);
void drawUserState(int line, uint8_t state);
void clearClientList();

#endif //DISPLAY_HPP
