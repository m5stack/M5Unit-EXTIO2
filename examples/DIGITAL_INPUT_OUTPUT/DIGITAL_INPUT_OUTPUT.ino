#include <M5Stack.h>
#include <M5GFX.h>
#include "M5_EXTIO2.h"

M5GFX display;
M5Canvas canvas(&display);
M5_EXTIO2 extio;

extio_io_mode_t mode = DIGITAL_OUTPUT_MODE;

void btnTask(void *pvParameters) {
    while (1) {
        if (M5.BtnA.wasPressed()) {
            if (mode == DIGITAL_INPUT_MODE) {
                mode = DIGITAL_OUTPUT_MODE;
            } else {
                mode = DIGITAL_INPUT_MODE;
            }
        }
        M5.update();
        vTaskDelay(80);
    }
}

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(2);
    canvas.setPaletteColor(1, ORANGE);
    while (!extio.begin(&Wire, 21, 22, 0x45)) {
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
    // extio.setAllPinMode(DIGITAL_INPUT_MODE);
    extio.setAllPinMode(DIGITAL_OUTPUT_MODE);
    // extio.setAllPinMode(ADC_INPUT_MODE);
    // extio.setAllPinMode(SERVO_CTL_MODE);
    // extio.setAllPinMode(RGB_LED_MODE);

    xTaskCreatePinnedToCore(btnTask, "btnTask"  // A name just for humans
                            ,
                            4096  // This stack size can be checked & adjusted
                                  // by reading the Stack Highwater
                            ,
                            NULL,
                            1  // Priority, with 3 (configMAX_PRIORITIES - 1)
                               // being the highest, and 0 being the lowest.
                            ,
                            NULL, 0);
}

char info[50];

void loop() {
    if (mode == DIGITAL_INPUT_MODE) {
        canvas.fillSprite(0);
        canvas.drawString("DIGITAL INPUT MODE", 10, 10);
        canvas.drawString("FW VERSION: " + String(extio.getVersion()), 10, 40);
        for (uint8_t i = 0; i < 8; i++) {
            if (extio.getDigitalInput(i)) {
                canvas.fillRect(i * 30 + 30, 145, 28, 30, 1);
            } else {
                canvas.drawRect(i * 30 + 30, 145, 28, 30, 1);
            }
        }
        canvas.pushSprite(0, 0);
    } else if (mode == DIGITAL_OUTPUT_MODE) {
        canvas.fillSprite(0);
        canvas.drawString("DIGITAL OUTPUT MODE", 10, 10);
        canvas.drawString("FW VERSION: " + String(extio.getVersion()), 10, 40);
        for (uint8_t i = 0; i < 8; i++) {
            extio.setDigitalOutput(i, HIGH);
            canvas.drawRect(i * 30 + 30, 145, 28, 30, 1);
            canvas.pushSprite(0, 0);
            vTaskDelay(100);
        }
        for (uint8_t i = 0; i < 8; i++) {
            extio.setDigitalOutput(i, LOW);
            canvas.fillRect(i * 30 + 30, 145, 28, 30, 1);
            canvas.pushSprite(0, 0);
            vTaskDelay(100);
        }
        canvas.pushSprite(0, 0);
    }
    extio.setAllPinMode(mode);
    vTaskDelay(100);
}
