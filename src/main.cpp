#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <SensirionI2cScd4x.h>
#include <XPT2046_Touchscreen.h>
#include <common.h>
#include <cstdint>

SensirionI2cScd4x scd40;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(Touch_CS, Touch_IRQ);

GFXcanvas1 SCD40canvas(7 * 12, 3 * 16);
/* max 7 characters at size 2 (12px) and 3 rows at size 2 (16px)
XX.XX°C
XX.XX%
XXXXppm
*/

// ------------------------------------------------------
// Global variables
// ------------------------------------------------------

uint16_t foregroundcolor = 0xFFFF;
uint16_t backgroundcolor = 0x0000;
bool darkmode = true;
static char errorMessage[64];
static int16_t error;
unsigned long ts = 0;

// ------------------------------------------------------
// State structure
// ------------------------------------------------------

struct State;

// Function pointer type:
// Pointer to a function returning State*
typedef State *(*StateHandler)(void);

// State object
struct State {
  StateHandler handler;
  const char *name;
};

// ------------------------------------------------------
// Forward declarations
// ------------------------------------------------------

void PrintUint64(uint64_t &value);
State *mainscreenHandler(void);
State *systemstatusscreenHandler(void);
State *airqualitymonitorscreenHandler(void);
State *touchcheckingHandler(void);

// ------------------------------------------------------
// State objects
// ------------------------------------------------------

State mainScreenState = {
    mainscreenHandler, 
    "MAIN_SCREEN"
};
State systemStatusScreenState = {
    systemstatusscreenHandler, 
    "SYSTEMSTATUS_SCREEN"
};
State airQualityMonitorScreenState = {
    airqualitymonitorscreenHandler, 
    "AIRQUALITYMONITOR_SCREEN"
};
State touchCheckingState = {
    touchcheckingHandler, 
    "TOUCHCHECKING"
};

// Current active state
State *currentState = &mainScreenState;

// ------------------------------------------------------
// Helper functions
// ------------------------------------------------------

/* Updates the readings of the SCD40 Sensor if data is ready to be received*/
void updateSCD40() {}

/* Updates all the sensor and display IC data if in the systemstatus screen*/
void readSensorStats() {}

/* Maps the touchpointcoordinates to the screen's pixels*/
void touchMapping(uint16_t touchpointX, uint16_t touchpointY, uint16_t *pMappedX, uint16_t *pMappedY) {
  *pMappedX = map(touchpointX, TOUCH_MIN_X, TOUCH_MAX_X, 0, SCREEN_WIDTH);
  *pMappedY = map(touchpointY, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT);
}

/* Prints all the static text and lines on the selected screen*/
void printStaticText() {
  if (currentState == &mainScreenState) {
    tft.setTextSize(1);
    tft.drawRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, Foregroundcolor);          // Darkmode
    tft.drawRect(BUTTONsyshealth_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, Foregroundcolor); // System Status

    tft.setTextColor(Foregroundcolor);
    tft.setCursor(215, BUTTON_Y);
    tft.print("Switch");
    tft.setCursor(215, BUTTON_Y + 9);
    tft.print("Darkmode");

    tft.setCursor(BUTTONsyshealth_X - 13, BUTTON_Y - 13);
    tft.print("System Status");
  } else if (currentState == &systemStatusScreenState) {

  } else if (currentState == &airQualityMonitorScreenState) {
  
  }
}

/* Reverses the front- and background colors*/
void switchDarkmode() {}

/* Either reads or writes the given data from or to the SD-Card*/
void readWriteSDcard() {}

// ------------------------------------------------------
// MAIN_SCREEN State
// ------------------------------------------------------

State *mainscreenHandler() {
    printStaticText();
    updateSCD40();

    return &mainScreenState;
}

// ------------------------------------------------------
// SYSTEMSTATUS_SCREEN State
// ------------------------------------------------------

State *systemstatusscreenHandler() {
    printStaticText();
    readSensorStats();

    return &systemStatusScreenState;
}

// ------------------------------------------------------
// AIRQUALITYMONITOR_SCREEN State
// ------------------------------------------------------

State *airqualitymonitorscreenHandler() {
    printStaticText();
    updateSCD40();

    return &airQualityMonitorScreenState;
}

// ------------------------------------------------------
// AIRQUALITYMONITOR_SCREEN State
// ------------------------------------------------------

State *touchcheckingHandler() {
  uint16_t mappedX, mappedY;
  TS_Point touchpoint = touch.getpoint();  
  touchMapping(touchpoint.x, touchpoint.y, &mappedX, &mappedY);

    return &touchCheckingState;
}

// ------------------------------------------------------
// State machine update
// ------------------------------------------------------

void fsmupdate() {
    State *previousState = currentState;

    currentState = currentState->handler();

    if (currentState != previousState) {
        Serial.print("State changed to: ");
        Serial.println(currentState->name);
    }
}

void setup() {
  // --------Bus Setup--------

  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  // --------SCD40 Setup--------

  uint64_t serialNumber = 0;
  scd40.begin(Wire, SCD40_I2C_ADDR_62);
  delay(30);
  
  // Stops the sensor from taking measurements
  error = scd40.stopPeriodicMeasurement();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }

  // Reloads sensor data from the EEPROM
  error = scd40.reinit();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute reinit(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  }

  // Read out information about the SCD40
  error = scd40.getSerialNumber(serialNumber);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  Serial.print("serial number: ");
  PrintUint64(serialNumber);
  Serial.println();

  // Start periodic measurements (5sec interval)
  error = scd40.startPeriodicMeasurement();
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }

  // --------TFT-Screen Setup--------

  tft.begin();
  tft.setRotation(3);
  tft.cp437(true);
  tft.fillScreen(0x0000); // Fills the Screen black

  // --------Canvas Setup--------

  SCD40canvas.setTextWrap(false); // In case the Text on the canvas exceeds the canvas it doesnt get wrapped around into the next line
  SCD40canvas.cp437(true);

  // --------XPT2046 Setup--------

  touch.begin();
  touch.setRotation(1);
}

void loop() {
    fsmupdate();
}

void PrintUint64(uint64_t &value) {
  Serial.print("0x");
  Serial.print((uint32_t)(value >> 32), HEX);
  Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}