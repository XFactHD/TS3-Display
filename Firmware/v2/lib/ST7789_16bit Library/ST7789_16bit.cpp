#include "ST7789_16bit.hpp"

#if defined(__SAMD21G18A__) || defined(__SAMD51__)
ST7789::ST7789(uint8_t nReset, uint8_t nRead, uint8_t nWrite, uint8_t nCS, uint8_t dc, PortGroup* dataBus, uint8_t dataOffset, bool dataMirrored, uint8_t brightness)
: Adafruit_GFX(TFT_WIDTH, TFT_HEIGHT), ledCtrl(brightness), dataOffset(dataOffset), dataMirrored(dataMirrored) {
  this->nReset = nReset;
  this->nRead = nRead;
  this->nWrite = nWrite;
  this->nCS = nCS;
  this->dc = dc;
  this->dataBus = dataBus;
  ledCtrl = brightness;
}
#elif defined(__IMXRT1062__)
ST7789::ST7789(uint8_t nReset, uint8_t nRead, uint8_t nWrite, uint8_t nCS, uint8_t dc, uint8_t* data, uint8_t brightness)
: Adafruit_GFX(TFT_WIDTH, TFT_HEIGHT), ledCtrl(brightness) {
  this->nReset = nReset;
  this->nRead = nRead;
  this->nWrite = nWrite;
  this->nCS = nCS;
  this->dc = dc;
  memcpy(this->dataBus, data, 16);
  ledCtrl = brightness;
}
#endif

void ST7789::begin(bool dispOn) {
    initGPIO();
    if (ledCtrl != 255) {
        pinMode(ledCtrl, OUTPUT);
    }
    setBrightness(0); //Start up pwm timer if applicable

    _ystart = _xstart = 0;
    _colstart  = _rowstart = 0;
    _height = 320;
    _width = 240;

    RST_LOW();
    delay(500);
    RST_HIGH(); //Leave reset
    delay(100);

    CS_LOW();

    writeCommand(CMD_SWRESET);
    delay(150);
    writeCommand(CMD_WAKEUP);
    delay(100);
    writeCommand(CMD_MADCTL);
    writeData(0x0000);
    writeCommand(CMD_COLMOD);
    writeData(0x0055);
    delay(10);
    writeCommand(CMD_INVON);
    delay(10);
    writeCommand(CMD_PORCTRK);
    writeData(0x000C);
    writeData(0x0000);
    writeData(0x0033);
    writeData(0x0033);
    delay(10);
    writeCommand(CMD_GCTRL);
    writeData(0x0035);
    delay(10);
    writeCommand(CMD_VCOMS);
    writeData(0x002B);
    delay(10);
    writeCommand(CMD_LCMCTRL);
    writeData(0x002C);
    delay(10);
    writeCommand(CMD_VDVVRHEN);
    writeData(0x0001);
    writeData(0x00FF);
    delay(10);
    writeCommand(CMD_VRHS);
    writeData(0x0011);
    delay(10);
    writeCommand(CMD_VDVS);
    writeData(0x0020);
    delay(10);
    writeCommand(CMD_FRCTRL2);
    writeData(0x000F);
    delay(10);
    writeCommand(CMD_PWCTRL1);
    writeData(0x00A4);
    writeData(0x00A1);
    delay(10);
    writeCommand(CMD_PVGAMCTRL);
    writeData(0x00D0);
    writeData(0x0000);
    writeData(0x0005);
    writeData(0x000E);
    writeData(0x0015);
    writeData(0x000D);
    writeData(0x0037);
    writeData(0x0043);
    writeData(0x0047);
    writeData(0x0009);
    writeData(0x0015);
    writeData(0x0012);
    writeData(0x0016);
    writeData(0x0019);
    delay(10);
    writeCommand(CMD_NVGAMCTRL);
    writeData(0x00D0);
    writeData(0x0000);
    writeData(0x0005);
    writeData(0x000D);
    writeData(0x000C);
    writeData(0x0006);
    writeData(0x002D);
    writeData(0x0044);
    writeData(0x0040);
    writeData(0x000E);
    writeData(0x001C);
    writeData(0x0018);
    writeData(0x0016);
    writeData(0x0019);
    delay(10);
    setAddressWindow(0, 240, 0, 320);
    writeCommand(CMD_RAM_WRITE);
    for(int i = 0; i < 240*320; i++) { writeData(TFT_BLACK); }
    delay(10);
    writeCommand(CMD_NORON);
    delay(10);

    if (dispOn) {
        writeCommand(CMD_DISPON);
        delay(10);
    }
  
    CS_HIGH();
}

#if defined(__SAMD21G18A__) || defined(__SAMD51__)
void ST7789::initGPIO() {
    EPortType port;
    uint32_t pin;
  
    if(nReset != 255) { //Reset pin
        port = g_APinDescription[nReset].ulPort;
        pin = g_APinDescription[nReset].ulPin;
        maskNReset = (1ul << pin);
        regNResetSet = &PORT->Group[port].OUTSET.reg;
        regNResetReset = &PORT->Group[port].OUTCLR.reg;
        PORT->Group[port].DIRSET.reg = maskNReset;
        *regNResetReset = maskNReset; //Default: hold in reset
    }

    if(nRead != 255) { //Read pin
        port = g_APinDescription[nRead].ulPort;
        pin = g_APinDescription[nRead].ulPin;
        maskNRead = (1ul << pin);
        regNReadSet = &PORT->Group[port].OUTSET.reg;
        regNReadReset = &PORT->Group[port].OUTCLR.reg;
        PORT->Group[port].DIRSET.reg = maskNRead;
        *regNReadSet = maskNRead; //Default: read disabled
    }

    //Write pin
    port = g_APinDescription[nWrite].ulPort;
    pin = g_APinDescription[nWrite].ulPin;
    maskNWrite = (1ul << pin);
    regNWriteSet = &PORT->Group[port].OUTSET.reg;
    regNWriteReset = &PORT->Group[port].OUTCLR.reg;
    PORT->Group[port].DIRSET.reg = maskNWrite;
    *regNWriteSet = maskNWrite; //Default: write disabled

    //Chip select pin
    port = g_APinDescription[nCS].ulPort;
    pin = g_APinDescription[nCS].ulPin;
    maskNChipSelect = (1ul << pin);
    regNChipSelectSet = &PORT->Group[port].OUTSET.reg;
    regNChipSelectReset = &PORT->Group[port].OUTCLR.reg;
    PORT->Group[port].DIRSET.reg = maskNChipSelect;
    *regNChipSelectSet = maskNChipSelect; //Default: chip deselected

    //Data/command pin
    port = g_APinDescription[dc].ulPort;
    pin = g_APinDescription[dc].ulPin;
    maskDataCommand = (1ul << pin);
    regDataCommandSet = &PORT->Group[port].OUTSET.reg;
    regDataCommandReset = &PORT->Group[port].OUTCLR.reg;
    PORT->Group[port].DIRSET.reg = maskDataCommand;
    *regDataCommandSet = maskDataCommand; //Default: data register selected

    //Data bus
    maskData = 0xFFFFu << dataOffset;
    regDataDirSet = &dataBus->DIRSET.reg;
    *regDataDirSet = 0xFFFFFFFFu & maskData;
    regDataDirReset = &dataBus->DIRCLR.reg;
    regDataSet = &dataBus->OUTSET.reg;
    regDataReset = &dataBus->OUTCLR.reg;
    regDataRead = &dataBus->IN.reg;
}
#elif defined(__IMXRT1062__)
void ST7789::initGPIO() {
    if(nReset != 255) {
        pinMode(nReset, OUTPUT);
        digitalWriteFast(nReset, HIGH); //Default: hold in reset
    }

    if(nRead != 255) {
        pinMode(nRead, OUTPUT);
        digitalWriteFast(nRead, HIGH); //Default: read disabled
    }

    pinMode(nWrite, OUTPUT);
    digitalWriteFast(nWrite, HIGH); //Default: write disabled

    pinMode(nCS, OUTPUT);
    digitalWriteFast(nCS, HIGH); //Default: chip deselected

    pinMode(dc, OUTPUT);
    digitalWriteFast(dc, HIGH); //Default: data register selected

    for (uint8_t pin : dataBus) { pinMode(pin, OUTPUT); }
}
#endif

void ST7789::startWrite() {
    CS_LOW();
}

void ST7789::endWrite() {
    CS_HIGH();
}

/** Graphics primitives */

void ST7789::writePixel(int16_t x, int16_t y, uint16_t color) {
    if(x < 0 || x >= _width || y < 0 || y >= _height) { return; }

    int16_t t;
    switch(rotation) {
        case 1:
            t = x;
            x = TFT_WIDTH  - 1 - y;
            y = t;
            break;
        case 2:
            x = TFT_WIDTH  - 1 - x;
            y = TFT_HEIGHT - 1 - y;
            break;
        case 3:
            t = x;
            x = y;
            y = TFT_HEIGHT - 1 - t;
            break;
    }

    setAddressWindow(x, x, y, y);

    writeCommand(CMD_RAM_WRITE);
    writeData(color);
}

void ST7789::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    int16_t temp;
    switch (rotation)
    {
        case 1:
        {
            temp = w;
            w = h;
            h = temp;

            temp = x;
            x = TFT_WIDTH - w - y;
            y = temp;
            break;
        }
        case 2:
        {
            x = TFT_WIDTH  - w - x;
            y = TFT_HEIGHT - h - y;
            break;
        }
        case 3:
        {
            temp = w;
            w = h;
            h = temp;

            temp = x;
            x = y;
            y = TFT_HEIGHT - h - temp;
            break;
        }
        default:
            break;
    }

    if(x < 0) { w += x; x = 0; }
    else if(x > TFT_WIDTH) { return; }
    if(x + w > TFT_WIDTH) { w = TFT_WIDTH - x; }
    if(y < 0) { h += y; y = 0; }
    else if(y > TFT_HEIGHT) { return; }
    if(y + h > TFT_HEIGHT) { h = TFT_HEIGHT - y; }

    setAddressWindow(x, x + w - 1, y, y + h - 1);

    writeCommand(CMD_RAM_WRITE);
    for(int i = 0; i < w * h; i++) {
        writeData(color);
    }
}

void ST7789::drawPixel(int16_t x, int16_t y, uint16_t color) {
    startWrite();
    writePixel(x, y, color);
    endWrite();
}

void ST7789::fillScreen(uint16_t color) {
    CS_LOW();

    setAddressWindow(0, 239, 0, 319);

    writeCommand(CMD_RAM_WRITE);
    for(int i = 0; i < 320 * 240; i++) {
        writeData(color);
    }

    CS_HIGH();
}

void ST7789::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    startWrite();
    writeFillRect(x, y, w, h, color);
    endWrite();
}

void ST7789::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if(y < 0 || y > _height) { return; }
	if(x < 0) { w += x; x = 0; }
	else if(x > _width) { return; }
	if(x + w > _width) { w = _width - x; }

    startWrite();
    writeFillRect(x, y, w, 1, color);
    endWrite();
}

void ST7789::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if(x < 0 || x > _width) { return; }
	if(y < 0) { h += y; y = 0; }
	else if(y > _height) { return; }
	if(y + h > _height) { h = _height - y; }

    startWrite();
    writeFillRect(x, y, 1, h, color);
    endWrite();
}

void ST7789::drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h) {
    bool invX = false;
    bool invY = false;

    int16_t temp;
    switch (rotation)
    {
        case 1:
        {
            temp = w;
            w = h;
            h = temp;

            temp = x;
            x = TFT_WIDTH - w - y;
            y = temp;

            invX = true;
            break;
        }
        case 2:
        {
            x = TFT_WIDTH  - w - x;
            y = TFT_HEIGHT - h - y;

            invX = true;
            invY = true;
            break;
        }
        case 3:
        {
            temp = w;
            w = h;
            h = temp;

            temp = x;
            x = y;
            y = TFT_HEIGHT - h - temp;

            invY = true;
            break;
        }
        default:
            break;
    }

    if(x < 0 || y < 0 || x >= TFT_WIDTH || y >= TFT_HEIGHT || x + w > TFT_WIDTH || y + h > TFT_HEIGHT) { return; }

    CS_LOW();

    setAddressWindow(x, x + w - 1, y, y + h - 1);

    writeCommand(CMD_RAM_WRITE);
    for (int pY = 0; pY < h; pY++) {
        for(int pX = 0; pX < w; pX++) {
            int iX = invX ? (w - 1 - pX) : pX;
            int iY = invY ? (h - 1 - pY) : pY;
            int idx = iX + (iY * w);
            writeData(pgm_read_word(&bitmap[idx]));
        }
    }

    CS_HIGH();
}

void ST7789::setRotation(uint8_t rot) {
    //CS_LOW();
    //writeCommand(CMD_MADCTL);

    rotation = rot % 4; // can't be higher than 3
    switch (rotation) {
        case 0:
            //writeData(CFG_MADCTL_MX | CFG_MADCTL_MY | CFG_MADCTL_RGB);

            _xstart = _colstart;
            _ystart = _rowstart;
            _width = TFT_WIDTH;
            _height = TFT_HEIGHT;
            break;
        case 1:
            //writeData(CFG_MADCTL_MY | CFG_MADCTL_MV | CFG_MADCTL_RGB);

            _ystart = _colstart;
            _xstart = _rowstart;
            _width = TFT_HEIGHT;
            _height = TFT_WIDTH;
            break;
        case 2:
            //writeData(CFG_MADCTL_RGB);
 
            _xstart = _colstart;
            _ystart = _rowstart;
            _width = TFT_WIDTH;
            _height = TFT_HEIGHT;
            break;
        case 3:
            //writeData(CFG_MADCTL_MX | CFG_MADCTL_MV | CFG_MADCTL_RGB);

            _ystart = _colstart;
            _xstart = _rowstart;
            _width = TFT_HEIGHT;
            _height = TFT_WIDTH;
            break;
    }
  
    //CS_HIGH();
}

/** Address window for pixel data */

void ST7789::setAddressWindow(uint16_t xStart, uint16_t xEnd, uint16_t yStart, uint16_t yEnd) {
    writeCommand(CMD_X_ADDRESS);
    writeData((xStart >> 8u) & 0x00FFu);
    writeData(xStart & 0x00FFu);
    writeData((xEnd >> 8u) & 0x00FFu);
    writeData(xEnd & 0x00FFu);

    writeCommand(CMD_Y_ADDRESS);
    writeData((yStart >> 8u) & 0x00FFu);
    writeData(yStart & 0x00FFu);
    writeData((yEnd >> 8u) & 0x00FFu);
    writeData(yEnd & 0x00FFu);
}

/** Display status */

void ST7789::setDisplayOn(bool on) {
    CS_LOW();
    writeCommand(on ? CMD_DISPON : CMD_DISPOFF);
    CS_HIGH();
}

/** High level bus I/O */

void ST7789::writeCommand(uint16_t cmd) {
    DC_LOW();
    writeToBus(cmd);
    DC_HIGH();
}

void ST7789::writeData(uint16_t data) {
    writeToBus(data);
}

__unused uint32_t ST7789::readID() {
    uint32_t id;
    uint32_t buffer;

    writeCommand(0x0004);

#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regDataDirSet = (0xFFFFu << dataOffset) & maskData;
#elif defined(__IMXRT1062__)
    for (uint8_t pin : dataBus) { pinMode(pin, INPUT); }
#endif

    RD_LOW();
    RD_HIGH();

    id = readBus() & 0xFFu;

    buffer = readBus() & 0xFFu;
    id |= (buffer << 8u);

    buffer = readBus() & 0xFFu;
    id |= (buffer << 16u);

#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regDataDirReset = (0xFFFFu << dataOffset) & maskData;
#elif defined(__IMXRT1062__)
    for (uint8_t pin : dataBus) { pinMode(pin, OUTPUT); }
#endif

    return id;
}

/** Low level bus I/O */
void ST7789::writeToBus(uint16_t data) {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    if (dataMirrored) {
        data = __RBIT(data) >> 16;
    }

    WR_LOW();

    *regDataSet = (data << dataOffset) & maskData;
    *regDataReset = ((data ^ 0xFFFFu) << dataOffset) & maskData;

    WR_HIGH();
#elif defined(__IMXRT1062__)
    WR_LOW();
    //delayMicroseconds(1);
    for(int i = 0; i < 16; i++) { digitalWriteFast(this->dataBus[i], bitRead(data, i)); }
    //delayMicroseconds(1);
    WR_HIGH();
    //delayMicroseconds(1);
#endif
}

uint16_t ST7789::readBus() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    RD_LOW();
    RD_HIGH();
    return (uint16_t)((*regDataRead >> dataOffset) & 0xFFFFu);
#elif defined(__IMXRT1062__)
    return 0; //TODO: implement for teensy 4
#endif
}

/** Control line control */

inline void ST7789::RST_HIGH() {
  if(nReset == 255) { return; }
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regNResetSet = maskNReset;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nReset, HIGH);
#endif
}

inline void ST7789::RST_LOW() {
  if(nReset == 255) { return; }
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regNResetReset = maskNReset;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nReset, LOW);
#endif
}

inline void ST7789::CS_HIGH() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
	  *regNChipSelectSet = maskNChipSelect;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nCS, HIGH);
#endif
}

inline void ST7789::CS_LOW() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
	  *regNChipSelectReset = maskNChipSelect;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nCS, LOW);
#endif
}

inline void ST7789::DC_HIGH() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regDataCommandSet = maskDataCommand;
#elif defined(__IMXRT1062__)
    digitalWriteFast(dc, HIGH);
#endif
}

inline void ST7789::DC_LOW() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regDataCommandReset = maskDataCommand;
#elif defined(__IMXRT1062__)
    digitalWriteFast(dc, LOW);
#endif
}

inline void ST7789::RD_HIGH() {
  if(nRead == 255) { return; }
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regNReadSet = maskNRead;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nRead, HIGH);
#endif
}

inline void ST7789::RD_LOW() {
  if(nRead == 255) { return; }
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regNReadReset  = maskNRead;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nRead, LOW);
#endif
}

inline void ST7789::WR_HIGH() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regNWriteSet = maskNWrite;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nWrite, HIGH);
#endif
}

inline void ST7789::WR_LOW() {
#if defined(__SAMD21G18A__) || defined(__SAMD51__)
    *regNWriteReset = maskNWrite;
#elif defined(__IMXRT1062__)
    digitalWriteFast(nWrite, LOW);
#endif
}

void ST7789::setBrightness(uint32_t value) const {
  if(ledCtrl != 255) {
    analogWrite(ledCtrl, value);
  }
}