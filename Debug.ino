
#ifdef DEBUG

    inline void printSensorConfiguration() {
        // Print the sensor configuration
        Serial.println("Sensor configuration:");
        for (byte i = 0; i < SENSOR_COUNT; i++) {
            Sensor* sensor = &sensors[i];

            Serial.print(getSensorName(sensor));
            Serial.print(" (");
            Serial.print(sensor->pin);
            Serial.print(")");
            Serial.print("\t");
            Serial.print("threshold: ");
            Serial.print(sensor->threshold);
            Serial.print("\t");
            Serial.print("min velocity: ");
            Serial.print(sensor->minVelocity);
            Serial.print("\t");
            Serial.print("max velocity: ");
            Serial.print(sensor->maxVelocity);
            Serial.print("\t");
            Serial.print("sensibility: ");
            Serial.print(sensor->sensibility);
            Serial.println();
        }
    }

    char* getSensorName(Sensor* sensor) {
        switch(sensor->pin) {
            case SENSOR_TOM_BLUE_PIN: return "TOM_BLUE";
            case SENSOR_TOM_RED_PIN: return "TOM_RED";
            case SENSOR_TOM_GREEN_PIN: return "TOM_GREEN";
            case SENSOR_CRASH_ORANGE_PIN: return "CRASH_ORANGE";
            case SENSOR_CRASH_YELLOW_PIN: return "CRASH_YELLOW";
            case SENSOR_KICK_PIN: return "KICK";
            default: return "Unknown";
        }
    }

    inline void printSensorTrigger(Sensor* sensor, int value, byte velocity) {
        Serial.print(getSensorName(sensor));

        if (sensor->state == HIGH) {
            Serial.print(" triggered -> ");
        } else {
            Serial.print(" released -> ");
        }

        Serial.print("\t");
        Serial.print("value: ");
        Serial.print(value);

        // Prints the velocity percentage
        if (sensor->state == HIGH) {
            Serial.print("\t");
            Serial.print("strength: ");
            int strength = (int) mapf(velocity, 0.0f, 127.0f, 0.0f, 100.0f);
            Serial.print(strength);
            Serial.print("%");
        }

        Serial.println();
    }
#endif

#if defined(DEBUG) && defined(DEBUG_PIN)
    inline void debugSinglePin() {
        int count = 1;
        int sum = 0;
        for (int i = 0; i < count; i++) {
            sum += analogRead(DEBUG_PIN);
        };
        short int avg = sum / count;

        Serial.println(avg);
    }
#endif