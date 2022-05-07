#include "M5_EXTIO2.h"

bool M5_EXTIO2::begin(TwoWire *wire, uint8_t sda, uint8_t scl, uint8_t addr) {
    _wire = wire;
    _addr = addr;
    _sda  = sda;
    _scl  = scl;
    _wire->begin(_sda, _scl);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

bool M5_EXTIO2::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                           uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    for (uint8_t i = 0; i < length; i++) {
        _wire->write(*(buffer + i));
    }
    if (_wire->endTransmission() == 0) return true;
    return false;
}

bool M5_EXTIO2::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                          uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    if (_wire->requestFrom(addr, length)) {
        for (uint8_t i = 0; i < length; i++) {
            buffer[index++] = _wire->read();
        }
        return true;
    }
    return false;
}

bool M5_EXTIO2::setAllPinMode(extio_io_mode_t mode) {
    uint8_t data[8];
    memset(data, mode, 8);

    return writeBytes(_addr, M5_EXTIO2_MODE_REG, data, 8);
    for (uint8_t i = 0; i < 8; i++) {
        delay(1);
        if (writeBytes(_addr, i, &data[i], 1) == false) return false;
        delay(9);
    }
    return true;
}

bool M5_EXTIO2::setDeviceAddr(uint8_t addr) {
    if (writeBytes(_addr, M5_EXTIO2_ADDRESS_REG, &addr, 1)) {
        _addr = addr;
        return true;
    } else {
        return false;
    }
}

uint8_t M5_EXTIO2::getVersion() {
    uint8_t data = 0;
    readBytes(_addr, M5_EXTIO2_FW_VERSION_REG, &data, 1);
    return data;
}

bool M5_EXTIO2::setDigitalOutput(uint8_t pin, uint8_t state) {
    uint8_t reg = pin + M5_EXTIO2_OUTPUT_CTL_REG;
    return writeBytes(_addr, reg, &state, 1);
}

bool M5_EXTIO2::setLEDColor(uint8_t pin, uint32_t color) {
    if (pin > 7) return false;
    uint8_t data[3] = {0};
    data[0]         = (color >> 16) & 0xff;
    data[1]         = (color >> 8) & 0xff;
    data[2]         = color & 0xff;
    uint8_t reg     = pin * 3 + M5_EXTIO2_RGB_24B_REG;
    return writeBytes(_addr, reg, data, 3);
}

bool M5_EXTIO2::setServoAngle(uint8_t pin, uint8_t angle) {
    uint8_t reg = pin + M5_EXTIO2_SERVO_ANGLE_8B_REG;
    return writeBytes(_addr, reg, &angle, 1);
}

bool M5_EXTIO2::setServoPulse(uint8_t pin, uint16_t pulse) {
    uint8_t data[2];
    uint8_t reg = pin * 2 + M5_EXTIO2_SERVO_PULSE_16B_REG;
    data[1]     = (pulse >> 8) & 0xff;
    data[0]     = pulse & 0xff;
    return writeBytes(_addr, reg, data, 2);
}

bool M5_EXTIO2::getDigitalInput(uint8_t pin) {
    uint8_t data;
    uint8_t reg = pin + M5_EXTIO2_DIGITAL_INPUT_REG;
    if (readBytes(_addr, reg, &data, 1)) {
        return data;
    }
    return 0;
}

uint16_t M5_EXTIO2::getAnalogInput(uint8_t pin, extio_anolog_read_mode_t bit) {
    if (bit == _8bit) {
        uint8_t data;
        uint8_t reg = pin + M5_EXTIO2_ANALOG_INPUT_8B_REG;
        if (readBytes(_addr, reg, &data, 1)) {
            return data;
        }
    } else {
        uint8_t data[2];
        uint8_t reg = pin * 2 + M5_EXTIO2_ANALOG_INPUT_12B_REG;
        if (readBytes(_addr, reg, data, 2)) {
            return (data[1] << 8) | data[0];
        }
    }
    return 0;
}
