struct Sensor {
    byte pin;
    byte state;
    long lastTrigger;
    short int threshold;
    byte note;
    byte minVelocity;
    byte maxVelocity;
    byte delay;
    float sensibility;
};