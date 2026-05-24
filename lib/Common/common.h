#ifndef common_H_
#define common_H_

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define Touch_CS 7
#define Touch_IRQ 2

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

/*
0--------X------320, 0
|
Y
|
0, 240
*/

#define TOUCH_MIN_X 240
#define TOUCH_MAX_X 3700
#define TOUCH_MIN_Y 280
#define TOUCH_MAX_Y 3800

#define BUTTON_WIDTH 40
#define BUTTON_HEIGHT 40
#define BUTTON_X 270 // Darkmode + Exit
#define BUTTON_Y 190 // Darkmode + AllSensorScreen + SystemHealth
#define BUTTONallsens_X 10 // AllSensorScreen
#define BUTTONexit_Y 10 // Exit
#define BUTTONsyshealth_X 140 // SystemHealth

// Errorstate used by the SCD40
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

//extern int foregroundcolor;
//extern int backgroundcolor;
//extern bool darkmode;

#endif // common_H_