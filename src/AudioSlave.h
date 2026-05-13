#ifndef AUDIO_SLAVE_H
#define AUDIO_SLAVE_H

#include <Arduino.h>

class AudioSlave
{
private:
    static AudioSlave *_activeInstance;

    static void receiveEventThunk(int bytes);
    static void requestEventThunk();

    int _i2cAddress;
    int _ledPin;
    volatile int _receivedValue;

public:
    AudioSlave(int i2cAddress = 9, int ledPin = LED_BUILTIN);

    void begin();
    void update();

    void receiveEvent(int bytes);
    void requestEvent();

    int getLastReceivedValue() const;
};

#endif