/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#include <tcMenu.h>
#include "colorTftEthernet32_menu.h"
#include "ThemeCoolBlueTraditional.h"

// Global variable declarations

const  ConnectorLocalInfo applicationInfo = { "Ada32 Ethernet", "22813e5e-88b1-42d5-9601-4831b2be369b" };

Adafruit_ST7735 gfx(6, 7, 3);
AdafruitDrawable gfxDrawable(&gfx);
GraphicsDeviceRenderer renderer(30, applicationInfo.name, &gfxDrawable);
EthernetServer server(3333);

// Global Menu Item declarations

const AnyMenuInfo minfoTakeDisplay = { "Take display", 17, 0xffff, 0, onTakeDisplay };
ActionMenuItem menuTakeDisplay(&minfoTakeDisplay, NULL);
const AnyMenuInfo minfoSaveItem = { "Save item", 23, 0xffff, 0, onSaveItem };
ActionMenuItem menuSaveItem(&minfoSaveItem, NULL);
RENDERING_CALLBACK_NAME_INVOKE(fnRomTextRtCall, textItemRenderFn, "Rom Text", -1, NO_CALLBACK)
TextMenuItem menuRomText(fnRomTextRtCall, 21, 10, &menuSaveItem);
ScrollChoiceMenuItem menuRomLocation(24, fnRomLocationRtCall, 0, 10, &menuRomText);
RENDERING_CALLBACK_NAME_INVOKE(fnRomChoiceRtCall, enumItemRenderFn, "Rom Choice", 14, NO_CALLBACK)
ScrollChoiceMenuItem menuRomChoice(25, fnRomChoiceRtCall, 0, 1024, 10, 10, &menuRomLocation);
RENDERING_CALLBACK_NAME_INVOKE(fnRomValuesRtCall, backSubItemRenderFn, "Rom Values", -1, NO_CALLBACK)
const SubMenuInfo minfoRomValues = { "Rom Values", 20, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackRomValues(fnRomValuesRtCall, &menuRomChoice);
SubMenuItem menuRomValues(&minfoRomValues, &menuBackRomValues, &menuTakeDisplay);
RENDERING_CALLBACK_NAME_INVOKE(fnIpAddressRtCall, ipAddressRenderFn, "Ip Address", 10, NO_CALLBACK)
IpAddressMenuItem menuIpAddress(fnIpAddressRtCall, 15, NULL);
RENDERING_CALLBACK_NAME_INVOKE(fnConnectivityRtCall, backSubItemRenderFn, "Connectivity", -1, NO_CALLBACK)
const SubMenuInfo minfoConnectivity = { "Connectivity", 14, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackConnectivity(fnConnectivityRtCall, &menuIpAddress);
SubMenuItem menuConnectivity(&minfoConnectivity, &menuBackConnectivity, &menuRomValues);
const FloatMenuInfo minfoVoltA1 = { "Volt A1", 9, 0xffff, 2, NO_CALLBACK };
FloatMenuItem menuVoltA1(&minfoVoltA1, NULL);
const FloatMenuInfo minfoVoltA0 = { "Volt A0", 8, 0xffff, 2, NO_CALLBACK };
FloatMenuItem menuVoltA0(&minfoVoltA0, &menuVoltA1);
RENDERING_CALLBACK_NAME_INVOKE(fnStatusRtCall, backSubItemRenderFn, "Status", -1, NO_CALLBACK)
const SubMenuInfo minfoStatus = { "Status", 7, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackStatus(fnStatusRtCall, &menuVoltA0);
SubMenuItem menuStatus(&minfoStatus, &menuBackStatus, &menuConnectivity);
RENDERING_CALLBACK_NAME_INVOKE(fnRGBRtCall, rgbAlphaItemRenderFn, "RGB", 16, onRgbChanged)
Rgb32MenuItem menuRGB(26, fnRGBRtCall, false, NULL);
const BooleanMenuInfo minfoTempCheck = { "Temp Check", 13, 9, 1, NO_CALLBACK, NAMING_ON_OFF };
BooleanMenuItem menuTempCheck(&minfoTempCheck, false, &menuRGB);
const AnyMenuInfo minfoHiddenItem = { "Hidden item", 16, 0xffff, 0, NO_CALLBACK };
ActionMenuItem menuHiddenItem(&minfoHiddenItem, &menuTempCheck);
const BooleanMenuInfo minfoSCircuitProtect = { "S-Circuit Protect", 12, 8, 1, NO_CALLBACK, NAMING_ON_OFF };
BooleanMenuItem menuSCircuitProtect(&minfoSCircuitProtect, false, &menuHiddenItem);
RENDERING_CALLBACK_NAME_INVOKE(fnAdvancedRtCall, backSubItemRenderFn, "Advanced", -1, NO_CALLBACK)
const SubMenuInfo minfoAdvanced = { "Advanced", 11, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackAdvanced(fnAdvancedRtCall, &menuSCircuitProtect);
SubMenuItem menuAdvanced(&minfoAdvanced, &menuBackAdvanced, NULL);
const AnyMenuInfo minfoSaveAll = { "Save all", 10, 0xffff, 0, onSaveRom };
ActionMenuItem menuSaveAll(&minfoSaveAll, &menuAdvanced);
const BooleanMenuInfo minfoPwrDelay = { "Pwr Delay", 5, 0xffff, 1, NO_CALLBACK, NAMING_YES_NO };
BooleanMenuItem menuPwrDelay(&minfoPwrDelay, false, &menuSaveAll);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsRtCall, backSubItemRenderFn, "Settings", -1, NO_CALLBACK)
const SubMenuInfo minfoSettings = { "Settings", 4, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackSettings(fnSettingsRtCall, &menuPwrDelay);
SubMenuItem menuSettings(&minfoSettings, &menuBackSettings, &menuStatus);
const char enumStrLimit_0[] = "Current";
const char enumStrLimit_1[] = "Voltage";
const char* const enumStrLimit[]  = { enumStrLimit_0, enumStrLimit_1 };
const EnumMenuInfo minfoLimit = { "Limit", 3, 6, 1, onLimitMode, enumStrLimit };
EnumMenuItem menuLimit(&minfoLimit, 0, &menuSettings);
const AnalogMenuInfo minfoCurrent = { "Current", 2, 4, 255, onCurrentChange, 0, 100, "A" };
AnalogMenuItem menuCurrent(&minfoCurrent, 0, &menuLimit);
const AnalogMenuInfo minfoVoltage = { "Voltage", 1, 2, 255, onVoltageChange, -128, 2, "V" };
AnalogMenuItem menuVoltage(&minfoVoltage, 0, &menuCurrent);

// Set up code

void setupMenu() {
    gfx.initR(INITR_BLACKTAB);
    gfx.setRotation(1);
    renderer.setUpdatesPerSecond(5);
    switches.initialiseInterrupt(io8574, true);
    menuMgr.initForEncoder(&renderer, &menuVoltage, 7, 6, 5);
    remoteServer.begin(&server, &applicationInfo);
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_FIRST_ROW);
    renderer.setUseSliderForAnalog(true);
    installCoolBlueTraditionalTheme(renderer, MenuFontDef(nullptr, 1), MenuFontDef(nullptr, 1), true);
}

