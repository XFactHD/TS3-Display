#ifndef CMD_H
#define CMD_H

#include <Arduino.h>

#include <display.hpp>

#define CMD_CONNECT           0x10  // We connected to a server (own userId)
#define CMD_DISCONNECT        0x11  // We left the server (no args)
#define CMD_SERVER_NAME       0x20  // New servers name (server name)
#define CMD_CHANNEL_SWITCH    0x30  // We switch channel (channel name)
#define CMD_CHANNEL_JOIN      0x31  // Someone joined our channel (userId, state, user name)
#define CMD_CHANNEL_APPEND    0x32  // Someone who is already in our channel, can now be shown on the display
#define CMD_CHANNEL_LEAVE     0x33  // Someone left our channel (userId)
#define CMD_CLIENT_NAME       0x40  // Used for clients in new channel after channel switch (userId, state, user name)
#define CMD_CLIENT_TALK       0x41  // Someone starts/stops speaking (userId, status)
#define CMD_CLIENT_MIC_MUTE   0x42  // Someone muted/unmuted their mic (userId, status)
#define CMD_CLIENT_SOUND_MUTE 0x43  // Someone muted/unmuted their sound (userId, status)
#define CMD_CLIENT_AWAY		  0x44	// Someone went away or came back (userId, status)
#define CMD_CLIENT_RENAME     0x45  // Someone changed their name
#define CMD_DISP_ON           0x50  // Switch display backlight on
#define CMD_DISP_OFF          0x51  // Switch display backlight off
#define CMD_CHECK_INTEGRITY   0x52	// Check integrity of the data on the display
#define CMD_REBUILD_LIST	  0x53	// Clear the client list on the display for rebuilding
#define CMD_REBUILD_DONE	  0x54	// Rebuild done, trigger redraw
#define CMD_ACK               0x60  // Command acknowledge

typedef struct {
  uint8_t cmdID;
  uint16_t userID;
  uint32_t talkPower;
  uint8_t state;
  char data[MAX_STR_LEN + 1];
} cmd_t;

#endif