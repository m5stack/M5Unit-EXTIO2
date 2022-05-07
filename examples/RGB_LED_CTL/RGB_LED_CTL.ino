#include <M5Stack.h>
#include <M5GFX.h>
#include "M5_EXTIO2.h"

M5GFX display;
M5Canvas canvas(&display);
M5_EXTIO2 extio;

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setPaletteColor(1, GREEN);
    while (!extio.begin(&Wire, 21, 22, 0x45)) {
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
    // extio.setAllPinMode(DIGITAL_INPUT_MODE);
    // extio.setAllPinMode(DIGITAL_OUTPUT_MODE);
    // extio.setAllPinMode(ADC_INPUT_MODE);
    // extio.setAllPinMode(SERVO_CTL_MODE);
    extio.setAllPinMode(RGB_LED_MODE);
}

void loop() {
    for (uint8_t m = 0; m < 3; m++) {
        canvas.fillSprite(0);
        canvas.setTextSize(2);
        canvas.drawString("RGB LED MODE", 10, 10);
        canvas.drawString("FW VERSION: " + String(extio.getVersion()), 10, 40);
        for (uint8_t i = 0; i < 8; i++) {
            extio.setLEDColor(i, 0xff << (m * 8));
        }
        canvas.setCursor(10, 120);
        canvas.printf("COLOR: 0x%X", 0xff << (m * 8));
        canvas.pushSprite(0, 0);
        vTaskDelay(800);
    }
}
