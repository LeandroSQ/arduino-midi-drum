bool isSensorTriggered(Sensor* sensor, int value, long timeSinceLastTrigger) {
    return value >= sensor->threshold && timeSinceLastTrigger >= sensor->delay;
}

bool isSensorReleased(Sensor* sensor, int value, long timeSinceLastTrigger) {
    return value <= sensor->threshold / 2 || timeSinceLastTrigger >= sensor->delay * 2;
}

#define mapAndClamp(value, in_min, in_max, out_min, out_max) constrain(map(value, in_min, in_max, out_min, out_max), out_min, out_max)

#define mapf(value, in_min, in_max, out_min, out_max) (((value - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min)

#define constrainf(value, min, max) ((value < min) ? min : ((value > max) ? max : value))

inline float interpolateOutQuart(Sensor* sensor, int value) {
    // Normalize velocities
    float normalizedMinVelocity = sensor->minVelocity / 127.0f;
    float normalizedMaxVelocity = sensor->maxVelocity / 127.0f;

    // Normalize the sensor value
    float normalized = mapf(value, (float) sensor->threshold, 1023.0f, normalizedMinVelocity, normalizedMaxVelocity);

    // Interpolate it
    float interpolated = 1.0f - powf(1.0f - normalized, sensor->sensibility);

    // Return it in the range [0f, 127f]
    return constrainf(interpolated * 127.0f, 0.0f, 127.0f);
}

inline byte calculateVelocity(Sensor* sensor, int value) {
    if (sensor->sensibility <= 1.0f) {// Linear sensibility
        return (byte) mapAndClamp(value, sensor->threshold, 1023, sensor->minVelocity, sensor->maxVelocity);
    } else {// Quadratic sensibility
        return (byte) interpolateOutQuart(sensor, value);
    }
}


#ifndef DEBUG
    inline void sendMIDIPacket(byte note, byte velocity) {
        midiEventPacket_t event;

        if (velocity <= 0) {
            // Turn NOTE OFF
            event = { 0x08, 0x80 | MIDI_CHANNEL, note, 0 };
        } else {
            // Turn NOTE ON
            event = { 0x09, 0x90 | MIDI_CHANNEL, note, velocity };
        }

        // Enqueue the event
        MidiUSB.sendMIDI(event);

        // Notify that the MIDI bus is dirty
        isMidiDirty = true;
    }
#endif