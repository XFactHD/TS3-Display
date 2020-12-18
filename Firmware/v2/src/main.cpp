#include <Arduino.h>
//#include <avr/power.h>
//#include <avr/sleep.h>

#include "display.hpp"

//#define DEBUG

client_t* clients[14];
uint16_t ownClientId = 0;

//EMPTY_INTERRUPT(PCINT2_vect);

void sleepUntilSerialInterrupt() {
    //Disable interrupts until ready to sleep
    //noInterrupts();

    //Disable UART
    //UCSR0B &= ~bit (RXEN0);  // disable receiver
    //UCSR0B &= ~bit (TXEN0);  // disable transmitter

    //Configure interrupt
    //PCMSK2 |= bit(PCINT16); // want pin 0
    //PCIFR  |= bit(PCIF2);   // clear any outstanding interrupts
    //PCICR  |= bit(PCIE2);   // enable pin change interrupts for D0 to D7

    //Configure sleep, reenable interrupt and zZz
    //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    //interrupts();
    //sleep_mode();

    //If the CPU wakes up here, then a UART message was received and an acknowledge must be sent
    //PCICR  &= ~bit(PCIE2);   // disable pin change interrupts for D0 to D7

    //Enable UART
    //UCSR0B |= bit (RXEN0);  // enable receiver
    //UCSR0B |= bit (TXEN0);  // enable transmitter

    //TODO: reimplement properly
    while (!Serial.available()) { delay(10); } //Wait for serial message
    Serial.read(); //Clear receive buffer
}

void performTest() {
    static unsigned long lastAction = 0;
    static uint8_t states[] = { 0, 1, 2, 4, 8 };

    if (millis() - lastAction > 5000) {
        lastAction = millis();

        Serial.println("Test routine");

        bool add = rand() % 2;
        if (add) {
            uint16_t id;
            do { id = rand(); } while (id == 0);

            uint8_t state = states[rand() % 5];

            cmd_t cmd = { 0, id, 0, state, "" };
            sprintf(cmd.data, "TestUser%d", id);
            onClientJoined(clients, &cmd);
            Serial.println("User added");
        }
        else {
            int pos = rand() % 14;
            if (clients[pos]->id != 0) {
                onClientLeft(clients, clients[pos]->id);
                Serial.println("User removed");
            }
        }

        Serial.println();
    }
}

void printCanvas(GFXcanvas16* canvas) {
    for (int i = 0; i < 16 * 16; ++i) {
        if (i % 16 == 0) { Serial.println(); }

        Serial.printf("0x%04X, ", canvas->getBuffer()[i]);
    }
    Serial.println();
}

void printIcons() {
    GFXcanvas16 display = GFXcanvas16(16, 16);

    { //Away
        display.fillScreen(TFT_WHITE);

        //Corpus
        display.fillRect(1, 8, 13, 7, TFT_BLUE);

        //Hanger
        display.drawLine(3, 7,  7, 3, TFT_BLACK);
        display.drawLine(7, 3, 11, 7, TFT_BLACK);

        //Cross
        display.drawLine(5, 9, 9, 13, TFT_WHITE);
        display.drawLine(9, 9, 5, 13, TFT_WHITE);

        printCanvas(&display);
    }

    { //Sound muted
        display.fillScreen(TFT_WHITE);

        display.fillRect(1, 5, 8, 6, TFT_BLACK);
        display.fillTriangle(3, 7, 8, 2, 8, 13, TFT_BLACK);

        display.drawPixel(10, 5, TFT_BLACK);
        display.drawPixel(10, 6, TFT_BLACK);
        display.drawPixel(11, 7, TFT_BLACK);
        display.drawPixel(11, 8, TFT_BLACK);
        display.drawPixel(10, 9, TFT_BLACK);
        display.drawPixel(10, 10, TFT_BLACK);

        display.drawPixel(10, 2, TFT_BLACK);
        display.drawPixel(11, 3, TFT_BLACK);
        display.drawPixel(12, 4, TFT_BLACK);
        display.drawPixel(12, 5, TFT_BLACK);
        display.drawPixel(13, 6, TFT_BLACK);
        display.drawPixel(13, 7, TFT_BLACK);
        display.drawPixel(13, 8, TFT_BLACK);
        display.drawPixel(13, 9, TFT_BLACK);
        display.drawPixel(12, 10, TFT_BLACK);
        display.drawPixel(12, 11, TFT_BLACK);
        display.drawPixel(11, 12, TFT_BLACK);
        display.drawPixel(10, 13, TFT_BLACK);

        display.drawLine(2, 1, 14, 13, TFT_RED);
        display.drawLine(1, 2, 13, 14, TFT_RED);
        display.drawLine(1, 1, 14, 14, TFT_RED);

        printCanvas(&display);
    }

    { //Mic muted
        display.fillScreen(TFT_WHITE);

        display.fillRoundRect(5, 1, 6, 9, 2, TFT_BLACK);
        display.drawFastVLine( 3, 8,  3, TFT_BLACK);
        display.drawFastVLine(12, 9,  3, TFT_BLACK);
        display.drawFastHLine( 3, 11, 10, TFT_BLACK);
        display.fillRect(7, 12, 2, 3, TFT_BLACK);

        display.drawLine(2, 1, 14, 13, TFT_RED);
        display.drawLine(1, 2, 13, 14, TFT_RED);
        display.drawLine(1, 1, 14, 14, TFT_RED);

        printCanvas(&display);
    }

    { //Talking
        display.fillScreen(TFT_WHITE);

        display.fillRoundRect(1, 1, 14, 14, 4, TFT_LIGHTBLUE);
        display.drawRoundRect(1, 1, 14, 14, 4, TFT_BLUE);

        printCanvas(&display);
    }

    { //Silent
        display.fillScreen(TFT_WHITE);

        display.fillRoundRect(1, 1, 14, 14, 4, TFT_NAVY);
        display.drawRoundRect(1, 1, 14, 14, 4, TFT_BLUE);

        printCanvas(&display);
    }
}

void setup() {
    //Disable unused peripherals to save power
    //power_adc_disable();
    //power_timer2_disable(); //Timer 1 can't be disabled, used by PWM for display backlight
    //power_twi_disable();

    Serial.begin(1000000);

    //Initialize client array
    for(auto& client : clients) { client = new client_t(); }

    initDisplay();

    printServerName((char*)"No server");
    printChannelName((char*)"No channel");

    displayOff();
    sleepUntilSerialInterrupt();
    Serial.write(CMD_ACK);

    //printIcons();
}

void loop() {
    if((size_t)Serial.available() >= sizeof(cmd_t)) {
        cmd_t cmdIn = { };
        Serial.readBytes((char*) &cmdIn, sizeof(cmd_t));

        switch (cmdIn.cmdID) {
            case CMD_CONNECT: {
                ownClientId = cmdIn.userID;
                break;
            }

            case CMD_DISCONNECT: {
                ownClientId = 0;
                for(auto & client : clients) { client->id = 0; } // Reset client list
                printServerName((char*)"No server");
                printChannelName((char*)"No channel");
                clearClientList();
                break;
            }

            case CMD_SERVER_NAME: {
                printServerName(cmdIn.data);
                break;
            }

            case CMD_CHANNEL_SWITCH: {
                printChannelName(cmdIn.data);
                for(auto & client : clients) { client->id = 0; } // Reset client list
                clearClientList();
                break;
            }

            case CMD_CHANNEL_JOIN: {
                onClientJoined(clients, &cmdIn);
                break;
            }

            case CMD_CHANNEL_APPEND: {
                tryAppendClient(clients, &cmdIn);
                break;
            }

            case CMD_CHANNEL_LEAVE: {
                onClientLeft(clients, cmdIn.userID);
                break;
            }

            case CMD_CLIENT_TALK: {
                onClientTalkState(clients, cmdIn.userID, cmdIn.state);
                break;
            }

            case CMD_CLIENT_MIC_MUTE: {
                onClientMicState(clients, cmdIn.userID, cmdIn.state);
                break;
            }

            case CMD_CLIENT_SOUND_MUTE: {
                onClientSoundState(clients, cmdIn.userID, cmdIn.state);
                break;
            }

            case CMD_CLIENT_AWAY: {
                onClientAwayState(clients, cmdIn.userID, cmdIn.state);
                break;
            }

            case CMD_CLIENT_RENAME: {
                onClientRename(clients, &cmdIn);
                break;
            }

            case CMD_DISP_ON: {
                displayOn();
                break;
            }

            case CMD_DISP_OFF: {
                displayOff();
                Serial.write(CMD_ACK);
                delay(10);
                sleepUntilSerialInterrupt();
                break;
            }
        }

        Serial.write(CMD_ACK);
    }

#ifdef DEBUG
    performTest();
#endif

    checkActions(clients);
}