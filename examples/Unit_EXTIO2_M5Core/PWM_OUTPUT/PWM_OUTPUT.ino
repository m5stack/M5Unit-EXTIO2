/*
*******************************************************************************
* Copyright (c) 2025 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套 M5Core 示例源代码
*
* Visit for more information: https://docs.m5stack.com/en/unit/extio2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/extio2
*
* Product: EXT.IO2.
* Date: 2025/12/25
*******************************************************************************
  PWM output
*/
#include <M5Unified.h>
#include "M5_EXTIO2.h"

// AtomS3-Lite I2C pins
#define I2C_SDA 2
#define I2C_SCL 1

M5_EXTIO2 extio;

// Configuration
const uint8_t PWM_PIN_1 = 0;  // First fan - 33% speed
const uint8_t PWM_PIN_2 = 1;  // Second fan - 66% speed
const uint8_t DUTY_PIN_1 = 33;  // 33% duty cycle
const uint8_t DUTY_PIN_2 = 66;  // 66% duty cycle

void setup() {
    M5.begin();
    Serial.begin(115200);
    delay(1000);  // Wait for serial monitor to connect
    
    Serial.println("\n\n=================================");
    Serial.println("EXTIO2 PWM Fan Control Test");
    Serial.println("=================================");
    
    // Initialize EXTIO2 with AtomS3-Lite I2C pins
    Serial.print("Initializing EXTIO2...");
    if (!extio.begin(&Wire, I2C_SDA, I2C_SCL, 0x45)) {
        Serial.println(" FAILED!");
        Serial.println("ERROR: Cannot connect to EXTIO2");
        Serial.println("Check I2C connections and address");
        while (1) {
            delay(1000);
        }
    }
    Serial.println(" OK");
    
    // Check firmware version
    uint8_t version = extio.getVersion();
    Serial.printf("Firmware Version: V%d\n", version);
    
    if (version < 3) {
        Serial.println("WARNING: PWM mode requires firmware V3 or higher!");
        Serial.println("Current firmware may not support PWM mode.");
    }
    
    Serial.println("\n--- Configuring PWM Mode ---");
    
    // Set pins to PWM mode
    Serial.printf("Setting Pin %d to PWM mode...", PWM_PIN_1);
    if (extio.setPinMode(PWM_PIN_1, PWM_MODE)) {
        Serial.println(" OK");
    } else {
        Serial.println(" FAILED");
    }
    
    Serial.printf("Setting Pin %d to PWM mode...", PWM_PIN_2);
    if (extio.setPinMode(PWM_PIN_2, PWM_MODE)) {
        Serial.println(" OK");
    } else {
        Serial.println(" FAILED");
    }
    
    // Set PWM frequency to 2KHz (suitable for fan control)
    Serial.print("Setting PWM frequency to 2KHz...");
    if (extio.setPwmFrequency(PWM_2KHZ)) {
        Serial.println(" OK");
    } else {
        Serial.println(" FAILED");
    }
    
    Serial.println("\n--- Setting Duty Cycles ---");
    
    // Set duty cycle for Pin 0 (33%)
    Serial.printf("Setting Pin %d duty cycle to %d%%...", PWM_PIN_1, DUTY_PIN_1);
    if (extio.setPwmDutyCycle(PWM_PIN_1, DUTY_PIN_1)) {
        Serial.println(" OK");
    } else {
        Serial.println(" FAILED");
    }
    
    // Set duty cycle for Pin 1 (66%)
    Serial.printf("Setting Pin %d duty cycle to %d%%...", PWM_PIN_2, DUTY_PIN_2);
    if (extio.setPwmDutyCycle(PWM_PIN_2, DUTY_PIN_2)) {
        Serial.println(" OK");
    } else {
        Serial.println(" FAILED");
    }
    
    Serial.println("\n=================================");
    Serial.println("PWM Configuration Complete!");
    Serial.println("=================================");
    Serial.printf("Pin %d: %d%% duty @ 2KHz\n", PWM_PIN_1, DUTY_PIN_1);
    Serial.printf("Pin %d: %d%% duty @ 2KHz\n", PWM_PIN_2, DUTY_PIN_2);
    Serial.println("\nFans should now be running at");
    Serial.println("their configured speeds.");
    Serial.println("=================================\n");
}

void loop() {
    // PWM is running continuously - no action needed in loop
    // You can monitor or adjust if needed
    
    delay(5000);
    
    // Optional: Print status every 5 seconds
    Serial.println("Status: PWM running");
    Serial.printf("  Pin %d: %d%% @ 2KHz\n", PWM_PIN_1, DUTY_PIN_1);
    Serial.printf("  Pin %d: %d%% @ 2KHz\n", PWM_PIN_2, DUTY_PIN_2);
}
