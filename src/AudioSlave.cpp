#include "AudioSlave.h"

#include <Wire.h>

AudioSlave *AudioSlave::_activeInstance = nullptr;

AudioSlave::AudioSlave(int i2cAddress, int ledPin)
    : _i2cAddress(i2cAddress), _ledPin(ledPin), _receivedValue(0)
{
}

void AudioSlave::receiveEventThunk(int bytes)
{
    if (_activeInstance != nullptr)
    {
        _activeInstance->receiveEvent(bytes);
    }
}

void AudioSlave::requestEventThunk()
{
    if (_activeInstance != nullptr)
    {
        _activeInstance->requestEvent();
    }
}

void AudioSlave::begin()
{
    pinMode(_ledPin, OUTPUT);
    digitalWrite(_ledPin, LOW);

    _activeInstance = this;
    Wire.begin(_i2cAddress);
    Wire.onReceive(AudioSlave::receiveEventThunk);
    Wire.onRequest(AudioSlave::requestEventThunk);
}

void AudioSlave::update()
{
    if (_receivedValue == 0)
    {
        digitalWrite(_ledPin, HIGH);
        delay(200);
        digitalWrite(_ledPin, LOW);
        delay(200);
    }
}

void AudioSlave::receiveEvent(int bytes)
{
    if (bytes <= 0)
    {
        return;
    }

    while (Wire.available())
    {
        _receivedValue = static_cast<int8_t>(Wire.read());
    }
}

void AudioSlave::requestEvent()
{
    Wire.write(static_cast<uint8_t>(static_cast<int8_t>(_receivedValue)));
}

int AudioSlave::getLastReceivedValue() const
{
    return _receivedValue;
}
