#include <Wire.h>
#include <SPI.h>
#include <Adafruit_MCP9808.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_INA260.h>
#include <XPT2046_Touchscreen.h>
#include "common.h"

#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 40
#define BUTTON_X 270
#define BUTTON_Y 190
#define BUTTON1_X 10
#define BUTTON2_Y 10

int Foregroundcolor = 0xFFFF;
int Backgroundcolor = 0x0000;
bool Darkmode = true;

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_INA260 INA260_5V = Adafruit_INA260();
Adafruit_INA260 INA260_3V = Adafruit_INA260();
XPT2046_Touchscreen ts(TS_CS, TS_IRQ);

GFXcanvas1 MCP9808canvas(84, 14);  // Creates a 1bit canvas which is the size of 7 Characters at size 2 (1 Character size 1 = 6x8; size 2 = 12x16) But we dont need the extra space under a Character because we only want to print numbers
GFXcanvas1 INA260_5Vcanvas(120, 48);
GFXcanvas1 INA260_3Vcanvas(120, 48);

void DrawMainScreen() {

  tft.drawRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, Foregroundcolor);
  tft.drawRect(BUTTON1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, Foregroundcolor);
  tft.setTextColor(Foregroundcolor);

  tft.setCursor(215, BUTTON_Y);
  tft.print("Switch");
  tft.setCursor(215, BUTTON_Y + 9);
  tft.print("Darkmode");

  tft.setCursor(57, BUTTON_Y);
  tft.print("Debugscreen");

  MCP9808canvas.fillScreen(0);       // Clears the canvas
  MCP9808canvas.setCursor(0, 0);     
  MCP9808canvas.setTextSize(2);
  MCP9808canvas.print(tempsensor.readTempC());
  MCP9808canvas.write(0xF8);
  MCP9808canvas.print("C");

  // Command    (XPos, YPos, Content of canvas,       canvas width,          canvas height,          Foreground-, Backgroundcolor);
  tft.drawBitmap(118, 120, MCP9808canvas.getBuffer(), MCP9808canvas.width(), MCP9808canvas.height(), Foregroundcolor, Backgroundcolor);   // The canvas exists purely in memory at this point, to get it printed on the screen we draw it as a bitmap. At first we say where the canvas is placed on the actual screen, after that we get the content and the dimensions of the canvas. In the end we give the Forground the color white and the Background black (In the case we need more colors we would need to use a 8bit or 16bit canvas)
}

void DrawDebugScreen() {

  sensor_t sensor;
  tempsensor.getSensor(&sensor);

  tft.fillScreen(Backgroundcolor);

  tft.setCursor(10, 10);
  tft.setTextColor(Foregroundcolor);
  tft.setTextSize(2);
  tft.print("DEBUGSCREEN");

  tft.setCursor(46, 55);
  tft.print("5V");
  tft.setCursor(214, 55);
  tft.print("3,3V");
  tft.drawFastHLine(10, 75, 300, Foregroundcolor);

  tft.drawRect(BUTTON_X, BUTTON2_Y, BUTTON_WIDTH, BUTTON_HEIGHT, Foregroundcolor);
  tft.setCursor(BUTTON_X - 31, BUTTON2_Y);
  tft.setTextSize(1);
  tft.print("Exit");

  tft.setCursor(39, 150);
  tft.print("MCP9808");
  tft.setCursor(116, 150);
  tft.print("INA260 3V  5V");
  tft.setCursor(233, 150);
  tft.print("ILI9341");
  tft.drawFastHLine(10, 160, 300, Foregroundcolor);
  tft.drawFastVLine(175, 150, 52, Foregroundcolor);

  tft.setCursor(10, 164);
  tft.print("Name: ");
  tft.print(sensor.name);
  tft.setCursor(10, 174);
  tft.print("Ver:  ");
  tft.print(sensor.version);
  tft.setCursor(10, 184);
  tft.print("ID:   ");
  tft.print(sensor.sensor_id);
  tft.setCursor(10, 194);
  tft.print("Max:  ");
  tft.print(sensor.max_value); tft.write(0xF8); tft.print("C");
  tft.setCursor(10, 204);
  tft.print("Min:  ");
  tft.print(sensor.min_value); tft.write(0xF8); tft.print("C");
  tft.setCursor(10, 214);
  tft.print("Res:  ");
  tft.print(sensor.resolution); tft.write(0xF8); tft.print("C");

  tft.setCursor(102, 164);
  tft.print("Mode:    ");
  tft.print(INA260_3V.getMode());
  tft.setCursor(180, 164);
  tft.print(INA260_5V.getMode());
  tft.setCursor(102, 174);
  tft.print("AvgCount:");
  tft.print(INA260_3V.getAveragingCount());
  tft.setCursor(180, 174);
  tft.print(INA260_5V.getAveragingCount());
  tft.setCursor(102, 184);
  tft.print("A Conv t:");
  tft.print(INA260_3V.getCurrentConversionTime()); tft.print("ms");
  tft.setCursor(180, 184);
  tft.print(INA260_5V.getCurrentConversionTime()); tft.print("ms");
  tft.setCursor(102, 194);
  tft.print("V Conv t:");
  tft.print(INA260_3V.getVoltageConversionTime()); tft.print("ms");
  tft.setCursor(180, 194);
  tft.print(INA260_5V.getVoltageConversionTime()); tft.print("ms");

  tft.setCursor(210, 164);
  tft.print("Power Mode: 0x");
  tft.print(tft.readcommand8(ILI9341_RDMODE));
  tft.setCursor(210, 174);
  tft.print("MADCTL Mode:0x");
  tft.print(tft.readcommand8(ILI9341_RDMADCTL));
  tft.setCursor(210, 184);
  tft.print("PixelFormat:0x");
  tft.print(tft.readcommand8(ILI9341_RDPIXFMT));
  tft.setCursor(210, 194);
  tft.print("ImageFormat:0x");
  tft.print(tft.readcommand8(ILI9341_RDIMGFMT));
  tft.setCursor(210, 204);
  tft.print("Self Diag:  0x");
  tft.print(tft.readcommand8(ILI9341_RDSELFDIAG));

  while (true) {
  TS_Point touchpoint = ts.getPoint();

  int mappedX = map(touchpoint.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, SCREEN_WIDTH);
  int mappedY = map(touchpoint.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT);

  INA260_5Vcanvas.fillScreen(0);
  INA260_5Vcanvas.setCursor(0, 0);
  INA260_5Vcanvas.setTextSize(2);
  INA260_5Vcanvas.print(INA260_5V.readBusVoltage()); INA260_5Vcanvas.print(" mV");
  INA260_5Vcanvas.setCursor(0, 17);
  INA260_5Vcanvas.print(INA260_5V.readCurrent()); INA260_5Vcanvas.print(" mA");
  INA260_5Vcanvas.setCursor(0, 34);
  INA260_5Vcanvas.print(INA260_5V.readPower()); INA260_5Vcanvas.print(" mW");

  INA260_3Vcanvas.fillScreen(0);
  INA260_3Vcanvas.setCursor(0, 0);
  INA260_3Vcanvas.setTextSize(2);
  INA260_3Vcanvas.print(INA260_3V.readBusVoltage()); INA260_3Vcanvas.print(" mV");
  INA260_3Vcanvas.setCursor(0, 17);
  INA260_3Vcanvas.print(INA260_3V.readCurrent()); INA260_3Vcanvas.print(" mA");
  INA260_3Vcanvas.setCursor(0, 34);
  INA260_3Vcanvas.print(INA260_3V.readPower()); INA260_3Vcanvas.print(" mW");

  tft.drawBitmap(10, 80, INA260_5Vcanvas.getBuffer(), INA260_5Vcanvas.width(), INA260_5Vcanvas.height(), Foregroundcolor, Backgroundcolor);
  tft.drawBitmap(190, 80, INA260_3Vcanvas.getBuffer(), INA260_3Vcanvas.width(), INA260_3Vcanvas.height(), Foregroundcolor, Backgroundcolor);

    if (mappedX >= BUTTON_X && mappedX <= BUTTON_X + BUTTON_WIDTH && mappedY >= BUTTON2_Y && mappedY <= BUTTON2_Y + BUTTON_HEIGHT) {
      tft.fillScreen(Backgroundcolor);
      break;
    }
  }
}

void checkButtons() {

  TS_Point touchpoint = ts.getPoint();

  int mappedX = map(touchpoint.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, SCREEN_WIDTH);
  int mappedY = map(touchpoint.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT);
/*
  Serial.print("X: ");
  Serial.print(mappedX);
  Serial.print(" Y: ");
  Serial.print(mappedY);
  Serial.println();
*/
  if (Darkmode == true && mappedX >= BUTTON_X && mappedX <= BUTTON_X + BUTTON_WIDTH && mappedY >= BUTTON_Y && mappedY <= BUTTON_Y + BUTTON_HEIGHT) {
    Darkmode = false;
    Foregroundcolor = 0x0000;
    Backgroundcolor = 0xFFFF;
    tft.fillScreen(Backgroundcolor);
  } else if (Darkmode == false && mappedX >= BUTTON_X && mappedX <= BUTTON_X + BUTTON_WIDTH && mappedY >= BUTTON_Y && mappedY <= BUTTON_Y + BUTTON_HEIGHT) {
    Darkmode = true;
    Foregroundcolor = 0xFFFF;
    Backgroundcolor = 0x0000;
    tft.fillScreen(Backgroundcolor);
  }

  if (mappedX >= BUTTON1_X && mappedX <= BUTTON1_X + BUTTON_WIDTH && mappedY >= BUTTON_Y && mappedY <= BUTTON_Y + BUTTON_HEIGHT) {
    DrawDebugScreen();
  }
}

void setup() {

  Serial.begin(9600);
  while (!Serial);

  Serial.println("MCP9808 Init:");
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808");
    while (1);
  }
  
  Serial.println("INA260_5V Init:");
  if (!INA260_5V.begin(0x41)) {                 // Pull up the A0 jumper to Vcc to change the address
    Serial.println("Couldn't find INA260_5V");
    while (1);
  } 
  
  Serial.println("INA260_3V Init:");
  if (!INA260_3V.begin(0x40)) {
    Serial.println("Couldn't find INA260_3V");
    while (1);
  }

  Serial.println("Found MCP9808!");
  Serial.println("Found INA260_5V!");
  Serial.println("Found INA260_3V!");

  tempsensor.setResolution(2);
  tempsensor.wake();

  tft.begin();
  tft.setRotation(3);
  tft.cp437(true);
  tft.fillScreen(0x0000);     // Fills the Screen always black
  MCP9808canvas.setTextWrap(false);  // In case the Text on the canvas exceeds the canvas it doesnt get wrapped around
  MCP9808canvas.cp437(true);
  INA260_5Vcanvas.setTextWrap(false);
  INA260_3Vcanvas.setTextWrap(false);

  ts.begin();
  ts.setRotation(1);

}

void loop() {

  DrawMainScreen();

  if (ts.touched()) {
    checkButtons();
  }

}