#include <Arduino.h>

const unsigned TIME_STEP = 3;

enum Pin : unsigned char {
    CLOCK = 2,
    BUS_0 = 3,
    BUS_1 = 4,
    BUS_2 = 5,
    BUS_3 = 6,
    BUS_4 = 7,
    BUS_5 = 8,
    BUS_6 = 9,
    BUS_7 = 10,
    LOAD = 11,
    ENABLE = 12
};

const uint8_t bus[8] = {
    Pin::BUS_0, 
    Pin::BUS_1, 
    Pin::BUS_2, 
    Pin::BUS_3, 
    Pin::BUS_4, 
    Pin::BUS_5, 
    Pin::BUS_6, 
    Pin::BUS_7
};

void set_bus_mode(int mode) {
    for (size_t it = 0; it < sizeof(bus); it++) {
        pinMode(bus[it], mode);
    }
}

void setup() {
    pinMode(Pin::CLOCK, OUTPUT);
    digitalWrite(Pin::CLOCK, LOW);
    pinMode(Pin::LOAD, OUTPUT);
    digitalWrite(Pin::LOAD, LOW);
    pinMode(Pin::ENABLE, OUTPUT);
    digitalWrite(Pin::ENABLE, LOW);

    set_bus_mode(OUTPUT);
    delay(TIME_STEP);
}

void write_to_bus(uint8_t value) {
    set_bus_mode(OUTPUT);

    for (size_t it = 0; it < sizeof(bus); it++) {
        digitalWrite(bus[it], bitRead(value, it));
    }

    delay(TIME_STEP);
    digitalWrite(Pin::LOAD, LOW);
    delay(TIME_STEP);
    digitalWrite(Pin::CLOCK, HIGH);
    delay(TIME_STEP);
    digitalWrite(Pin::LOAD, HIGH);
    delay(TIME_STEP);
    digitalWrite(Pin::CLOCK, LOW);
    delay(TIME_STEP);
}

uint8_t read_from_bus() {
    set_bus_mode(INPUT);
    digitalWrite(Pin::ENABLE, LOW);
    delay(TIME_STEP);

    uint8_t value = 0;
    for (size_t it = 0; it < sizeof(bus); it++) {
        if (digitalRead(bus[it])) {
            pinMode(bus[it], INPUT);
            bitSet(value, it);
        }
    }
    
    digitalWrite(Pin::ENABLE, HIGH);
    delay(TIME_STEP);

    return value;
}

void loop() {
    delay(5000);
    for (uint8_t n = 0; n <= 0xFF; n++) {
        write_to_bus(n);
        Serial.print("-> ");
        Serial.print(n, HEX);
        Serial.print("\n");
        
        uint8_t read_value = read_from_bus();
        Serial.print("<- ");
        Serial.print(read_value, HEX);
        if (read_value != n) {
            Serial.print(" (ERROR) ");
        }
        Serial.print("\n");
    }
}