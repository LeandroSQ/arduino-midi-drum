// #define DEBUG
// #define DEBUG_PIN A6

#include <Arduino.h>
#include "Sensor.h"

#ifndef DEBUG
    #include "MIDIUSB.h"
#endif

// Definitions
#define SENSOR_TOM_BLUE_PIN A0
#define SENSOR_TOM_RED_PIN A1
#define SENSOR_TOM_GREEN_PIN A7 /* DIGITAL 6*/

#define SENSOR_CRASH_ORANGE_PIN A2
#define SENSOR_CRASH_YELLOW_PIN A3

#define SENSOR_KICK_PIN A6 /* DIGITAL 4 */

#define SENSOR_COUNT 6
#define MIDI_CHANNEL 0

// Variables
// Define the sensors
Sensor* sensors = new Sensor[SENSOR_COUNT] {
    // PIN                             THRESHOLD  NOTE          VELOCITY  DELAY  SENSIBILITY
    { SENSOR_TOM_BLUE_PIN    , LOW, 0, 90,        50/* D2  */,  30, 127,  120,   2.0f },
    { SENSOR_TOM_RED_PIN     , LOW, 0, 90,        38/* D1  */,  30, 127,  120,   2.0f },
    { SENSOR_TOM_GREEN_PIN   , LOW, 0, 90,        45/* A1  */,  30, 127,  120,   2.0f },
    { SENSOR_CRASH_ORANGE_PIN, LOW, 0, 200,       49/* C#2 */,  80, 127,  100,   3.0f },
    { SENSOR_CRASH_YELLOW_PIN, LOW, 0, 80,        42/* F#1 */,  20, 127,  100,   3.0f },
    { SENSOR_KICK_PIN        , LOW, 0, 110,       36/* C1  */,  80, 127,  240,   1.0f }
};

// Determines whether there is MIDI events to be sent
bool isMidiDirty = false;

// Methods
void setup() {
    #ifdef DEBUG
        // Setup serial communication
        Serial.begin(9600);
    #endif

    // Setup the sensors
    for (byte i = 0; i < SENSOR_COUNT; i++) pinMode(sensors[i].pin, INPUT);

    #ifdef DEBUG
        delay(1000);
        printSensorConfiguration();
    #endif
}

void loop() {
    #if defined(DEBUG) && defined(DEBUG_PIN)
        // Debug only one pin, for testing with Serial plotter
        debugSinglePin();
    #else
        // Check sensors
        for (byte i = 0; i < SENSOR_COUNT; i++) handleSensor(&sensors[i]);

        #ifndef DEBUG
            // If anything on the bus, send it
            if (isMidiDirty) {
                MidiUSB.flush();
                isMidiDirty = false;
            }
        #endif

        // Wait for the next cycle
        delayMicroseconds(10);
    #endif
}

inline void handleSensor(Sensor* sensor) {
    // Read the sensor
    int value = analogRead(sensor->pin);
    // Calculate the time since the last trigger
    long timeSinceLastTrigger = millis() - sensor->lastTrigger;

    // Check if the sensor state has changed
    if (
        (isSensorTriggered(sensor, value, timeSinceLastTrigger) && sensor->state == LOW) ||
        (isSensorReleased(sensor, value, timeSinceLastTrigger) && sensor->state == HIGH)
    ) {
        // Change the sensor state
        sensor->state = !sensor->state;

        // Calculate the velocity
        byte velocity = calculateVelocity(sensor, value);

        // Handle the change
        #ifdef DEBUG
            // Print with serial debug
            printSensorTrigger(sensor, value, velocity);
        #else
            // Send the MIDI packet
            sendMIDIPacket(sensor->note, velocity);
        #endif

        // Update the last trigger time
        if (sensor->state == HIGH) sensor->lastTrigger = millis();
    }
}