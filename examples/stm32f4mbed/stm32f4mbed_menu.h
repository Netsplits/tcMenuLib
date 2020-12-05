/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
*/

#ifndef MENU_GENERATED_CODE_H
#define MENU_GENERATED_CODE_H

#include <mbed.h>
#include <tcMenu.h>
#include <RuntimeMenuItem.h>
#include "Adafruit_SSD1306.h"
#include "tcMenuAdaFruitGfx.h"
#include "MBedEthernetTransport.h"
#include <RemoteConnector.h>
#include <ScrollChoiceMenuItem.h>
#include <EditableLargeNumberMenuItem.h>

void setupMenu();  // forward reference of the menu setup function.
extern const PROGMEM ConnectorLocalInfo applicationInfo;  // defines the app info to the linker.

// Global variables that need exporting

extern SPI spi;
extern Adafruit_SSD1306_Spi gfx;
extern AdaColorGfxMenuConfig gfxConfig;
extern AdaFruitGfxMenuRenderer renderer;

// Callback functions must always include CALLBACK_FUNCTION after the return type
#define CALLBACK_FUNCTION

// Global Menu Item exports

extern TextMenuItem menuEdit;
extern AnalogMenuItem menuCommits;
extern IpAddressMenuItem menuIP;
extern SubMenuItem menuConnectivity;
extern FloatMenuItem menuAvgTemp;
int fnCountingListRtCall(RuntimeMenuItem * item, uint8_t row, RenderFnMode mode, char * buffer, int bufferSize);
extern ListRuntimeMenuItem menuCountingList;
extern ScrollChoiceMenuItem menuChoices;
extern SubMenuItem menuOther;
extern Rgb32MenuItem menuRGB;
void CALLBACK_FUNCTION onFrequencyChanged(int id);
extern EditableLargeNumberMenuItem menuFrequency;
extern BooleanMenuItem menuPower;
void CALLBACK_FUNCTION onFoodChange(int id);
extern EnumMenuItem menuFoods;
void CALLBACK_FUNCTION onTenthsChaned(int id);
extern AnalogMenuItem menuTenths;
extern SubMenuItem menuEditing;
extern TimeFormattedMenuItem menuRTCTime;
extern DateFormattedMenuItem menuRTCDate;

#endif // MENU_GENERATED_CODE_H
