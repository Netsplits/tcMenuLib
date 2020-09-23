#include "colorTftEthernet32_menu.h"
#include <IoAbstractionWire.h>
#include <EepromAbstractionWire.h>
#include <AnalogDeviceAbstraction.h>
#include <RemoteAuthentication.h>
#include <RemoteMenuItem.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Ethernet.h>
#include <SPI.h>

// contains the graphical widget title components.
#include "stockIcons/wifiAndConnectionIcons16x12.h"

/*
 * Shows how to use adagraphics with a TFT panel and an ethernet module.
 * This is a 32 bit example, which by default targets 32 bit devices.
 * Assumed board for this is a SAMD based MKR board.
 * 
 * For more details see the README.md file in this directory.
 */

// we are going to allow control of the menu over local area network
// so therefore must configure ethernet..
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// we want to customise the colours and menu spacing. So we create this config object too
// which we initialise during the setup method.
AdaColorGfxMenuConfig colorConfig;

// We also store/restore state from an i2c EEPROM chip
I2cAt24Eeprom eeprom(0x50, PAGESIZE_AT24C128);

// we want to authenticate connections, the easiest and quickest way is to use the EEPROM
// authenticator where pairing requests add a new item into the EEPROM. Any authentication
// requests are then handled by looking in the EEPROM.
EepromAuthenticatorManager authManager;

// then we setup the IO expander that the also set up in the designer for input.
IoAbstractionRef io8574 = ioFrom8574(0x20, 0); // on addr 0x20 and interrupt pin 0

// and we create an analog device with enhanced range because we are using a 32bit board.
ArduinoAnalogDevice analogDevice(12, 10);

// Here we create two additional menus, that will be added manually to handle the connectivity
// status and authentication keys. In a future version these will be added to th desinger.
RemoteMenuItem menuRemoteMonitor(1001, 2);
EepromAuthenicationInfoMenuItem menuAuthKeyMgr(1002, &authManager, &menuRemoteMonitor);

// We add a title widget that shows when a user is connected to the device. Connection icons
// are in the standard icon set we included at the top.
TitleWidget connectedWidget(iconsConnection, 2, 16, 12);

//
// here we provide a completely custom configuration of color and spacing
//
void prepareCustomConfiguration() {
    // first we set the spacing around title, items and widgets. The make padding function follows the
    // same standard as CSS. Top, right, bottom, left.
	makePadding(colorConfig.titlePadding, 6, 3, 6, 3); // top, right, bottom & left
	makePadding(colorConfig.itemPadding, 3, 3, 3, 3);   // top, right, bottom & left
	makePadding(colorConfig.widgetPadding, 6, 8, 0, 3);// top, right, bottom & left

    // and then the foreground, background and font of the title
	colorConfig.bgTitleColor = RGB(50, 100, 200);
	colorConfig.fgTitleColor = RGB(0, 0, 0);
	colorConfig.titleFont = &FreeSans9pt7b;
	colorConfig.titleBottomMargin = 10; // the space between title and items.

    // and then the colours for items.
	colorConfig.bgItemColor = RGB(0, 0, 0);
	colorConfig.fgItemColor = RGB(222, 222, 222);
	colorConfig.itemFont = NULL;

    // and when items are selected.
	colorConfig.bgSelectColor = RGB(0, 50, 200);
	colorConfig.fgSelectColor = RGB(255, 255, 255);
	colorConfig.widgetColor = RGB(30, 30, 30);

    // you can set the size scaling of the fonts
	colorConfig.titleFontMagnification = 1;
	colorConfig.itemFontMagnification = 1;

    // and if you really want to, provide alternative bitmaps for the edit / active icon
    // otherwise both icons must be set to NULL.
    //colorConfig.editIcon = myEditIcon;
    //colorConfig.activeIcon = myActiveIcon;
    //colorConfig.editIconWidth = myEditWidth;
    //colorConfig.editIconHeight = myEditheight;
    colorConfig.editIcon = NULL;
    colorConfig.activeIcon = NULL;
}

// when there's a change in communication status (client connects for example) this gets called.
void onCommsChange(CommunicationInfo info) {
    if(info.remoteNo == 0) {
        connectedWidget.setCurrentState(info.connected ? 1 : 0);
    }
    // this relies on logging in IoAbstraction's ioLogging.h, to turn it on visit the file for instructions.
    serdebugF4("Comms notify (rNo, con, enum)", info.remoteNo, info.connected, info.errorMode);
}

void setup() {
    // we used an i2c device (io8574) so must initialise wire too
    Wire.begin();

    // now we enable authentication using EEPROM authentication. Where the EEPROM is
    // queried for authentication requests, and any additional pairs are stored there too.
    // first we initialise the authManager, then pass it to the class.
    // Always call BEFORE setupMenu()
    authManager.initialise(&eeprom, 100);
    remoteServer.setAuthenticator(&authManager);

    // Here we add two additional menus for managing the connectivity and authentication keys.
    // In the future, there will be an option to autogenerate these from the designer.
    menuIpAddress.setNext(&menuAuthKeyMgr);
    menuRemoteMonitor.addConnector(remoteServer.getRemoteConnector(0));
    menuRemoteMonitor.registerCommsNotification(onCommsChange);
    menuAuthKeyMgr.setLocalOnly(true);

    // and set up the dac on the 32 bit board.
    analogDevice.initPin(A0, DIR_OUT);
    analogDevice.initPin(A1, DIR_IN);

    // we have our own graphics configuration. it must be initialised
    // before calling setupMenu..
    prepareCustomConfiguration();

    // here we make the menuitem "hidden item" invisible. It will not be displayed.
    // here it is done before setupMenu is called, so there's no need to refresh the
    // display. If it's done after initialisation, the menu must be reset by calling
    // menuMgr.setCurrentMenu(getRoot());
    menuHiddenItem.setVisible(false);

    // set up the widget to appear in the title.
    renderer.setFirstWidget(&connectedWidget);

    // set up the menu
    setupMenu();

    // and then load back the previous state
    menuMgr.load(eeprom);

    // spin up the Ethernet library
    byte* rawIp = menuIpAddress.getIpAddress();
    IPAddress ipAddr(rawIp[0], rawIp[1], rawIp[2], rawIp[3]);
    Ethernet.begin(mac, ipAddr);

    char sz[20];
    menuIpAddress.copyValue(sz, sizeof(sz));
    Serial.print("Ethernet available on ");Serial.println(sz);

    taskManager.scheduleFixedRate(2250, [] {
        Serial.print(".");
        float a1Value = analogDevice.getCurrentFloat(A1);
        menuVoltA1.setFloatValue(a1Value * 3.3);
    });
}

void loop() {
    taskManager.runLoop();
}

void writeToDac() {
    float volts = menuVoltage.getAsFloatingPointValue();
    float curr = menuCurrent.getAsFloatingPointValue();
    
    float total = (volts / 64.0) * (curr / 2.0);
    analogDevice.setCurrentFloat(A0, (unsigned int)total);
    menuVoltA0.setFloatValue(total);
}

void CALLBACK_FUNCTION onVoltageChange(int /*id*/) {
    writeToDac();
}

void CALLBACK_FUNCTION onCurrentChange(int /*id*/) {
    writeToDac();
}

void CALLBACK_FUNCTION onLimitMode(int /*id*/) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION onSaveRom(int /*id*/) {
    // save out the state, in a real system we could detect power down for this.
    menuMgr.save(eeprom);
}

bool starting = true;

void myRenderCallback(unsigned int encoderVal, RenderPressMode pressType) {
    if(pressType == RPRESS_HELD)
    {
        // if the encoder / select button is held, we go back to the menu.
        renderer.giveBackDisplay();
    }
    else if(starting)
    {
        // you need to handle the clearing and preparation of the display when you're first called.
        // the easiest way is to set a flag such as this and then prepare the display.
        starting = false;
        switches.getEncoder()->changePrecision(1000, 500);
        gfx.setCursor(0, 0);
        gfx.fillRect(0, 0, gfx.width(), gfx.height(), BLACK);
        gfx.setFont(nullptr);
        gfx.setTextSize(2);
        gfx.print("Encoder ");
    }
    else
    {
        GFXcanvas1 canvas(100, 20);
        canvas.fillScreen(BLACK);
        canvas.setCursor(0,0);
        canvas.print(encoderVal);
        canvas.setTextSize(2);
        gfx.drawBitmap(0, 35, canvas.getBuffer(), 100, 20, WHITE, BLACK);
    }
}

void CALLBACK_FUNCTION onTakeDisplay(int id) {
    starting = true;
    renderer.takeOverDisplay(myRenderCallback);
}
