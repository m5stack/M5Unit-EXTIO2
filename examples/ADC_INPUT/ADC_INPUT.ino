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
    canvas.setPaletteColor(1, ORANGE);
    while (!extio.begin(&Wire, 21, 22, 0x45)) {
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
    // extio.setAllPinMode(DIGITAL_INPUT_MODE);
    // extio.setAllPinMode(DIGITAL_OUTPUT_MODE);
    extio.setAllPinMode(ADC_INPUT_MODE);
    // extio.setAllPinMode(SERVO_CTL_MODE);
    // extio.setAllPinMode(RGB_LED_MODE);
}

char info[50];

void loop() {
    canvas.fillSprite(0);
    canvas.setTextSize(2);
    canvas.drawString("ADC INPUT MODE", 10, 10);
    canvas.drawString("FW VERSION: " + String(extio.getVersion()), 10, 40);
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t adc = extio.getAnalogInput(i, _12bit);
        Serial.printf("CH:%d ADC: %d", i, adc);
        canvas.drawRect(0, i * 20 + 75, 200, 15, 1);
        canvas.fillRect(0, i * 20 + 75, map(adc, 0, 4096, 0, 200), 15, 1);
        canvas.setCursor(220, i * 28 + 10);
        canvas.setTextSize(1);
        canvas.printf("CH:%d ADC: %d", i, adc);
    }
    canvas.pushSprite(0, 0);
    vTaskDelay(100);
}
