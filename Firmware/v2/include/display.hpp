#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include "ST7789_16bit.hpp"

#include "client.hpp"
#include "icons.hpp"

#define PCB_V1

#ifdef __IMXRT1062__
#define TFT_RST 0
#define TFT_RD  17
#define TFT_WR  18
#define TFT_CS  20
#define TFT_DC  19
#define TFT_BL  255
#elif defined(PCB_V1)
#define TFT_RST 31 //PB23
#define TFT_RD  255
#define TFT_WR  18 //PA05
#define TFT_CS  15 //PB08
#define TFT_DC  16 //PB09
#define TFT_BL  30 //PB22
#else
#define TFT_RST 15 //PB08
#define TFT_RD  255
#define TFT_WR  11 //PA20
#define TFT_CS  20 //PA22
#define TFT_DC  12 //PA21
#define TFT_BL  15 //PB08
#endif

class client_t;

void initDisplay();
bool switchDarkMode(bool dark);
void setDisplayBrightness(uint8_t percent);
void configureColors();
void drawBackground();
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
