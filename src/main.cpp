#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <SensirionI2cScd4x.h>
#include <common.h>

SensirionI2cScd4x scd40;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(Touch_CS, Touch_IRQ);

GFXcanvas1 SCD40canvas(7 * 12, 3 * 16);

uint8_t foregroundcolor = 0xFFFF;
uint8_t backgroundcolor = 0x0000;
bool darkmode = true;

void PrintUint64(uint64_t &value);

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

static char errorMessage[64];
static int16_t error;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    Wire.begin();

    uint64_t serialnumber = 0;

    scd40.begin(Wire, SCD40_I2C_ADDR_62);

    uint64_t serialNumber = 0;
    delay(30);
    // Ensure scd40 is in clean state
    error = scd40.wakeUp();
    if (error != NO_ERROR)
    {
        Serial.print("Error trying to execute wakeUp(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    error = scd40.stopPeriodicMeasurement();
    if (error != NO_ERROR)
    {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    error = scd40.reinit();
    if (error != NO_ERROR)
    {
        Serial.print("Error trying to execute reinit(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
    }
    // Read out information about the scd40
    error = scd40.getSerialNumber(serialNumber);
    if (error != NO_ERROR)
    {
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
    if (error != NO_ERROR)
    {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }

    tft.begin();
    tft.setRotation(3);
    tft.cp437(true);
    tft.fillScreen(0x0000);         // Fills the Screen black

    SCD40canvas.setTextWrap(false); // In case the Text on the canvas exceeds the canvas it doesnt get wrapped around into the next line
    SCD40canvas.cp437(true);

    ts.begin();
    ts.setRotation(1);
}

void loop()
{
}

void PrintUint64(uint64_t &value)
{
    Serial.print("0x");
    Serial.print((uint32_t)(value >> 32), HEX);
    Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}