#include "AudioSlave.h"

#include <Wire.h>
#include "Globals.h"

namespace
{
    struct AudioRequestMapEntry
    {
        const char *id;
        int8_t code;
    };

    static constexpr AudioRequestMapEntry kAudioRequestMap[] = {
        {"notice", 1},
        {"alert_slowBuildLong", 2},
        {"alert_immediateLong", 3},
        {"alert_simple", 4},
        {"reminder", 5},
    };
} // namespace

AudioSlave *AudioSlave::_activeInstance = nullptr;

AudioSlave::AudioSlave(int i2cAddress, int ledPin)
    : _i2cAddress(i2cAddress), _ledPin(ledPin), _receivedValue(0), _requestedEventCode(0)
{
}

bool AudioSlave::tryMapRequestIdToCode(const String &requestId, int8_t &code)
{
    for (const AudioRequestMapEntry &entry : kAudioRequestMap)
    {
        if (requestId.equals(entry.id))
        {
            code = entry.code;
            return true;
        }
    }

    return false;
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

bool AudioSlave::requestEvent(const String &requestId)
{
    Serial.println(String("Request ID ") + requestId);
    int8_t mappedCode = 0;

    if (!AudioSlave::tryMapRequestIdToCode(requestId, mappedCode))
    {
        return false;
    }

    _requestedEventCode = mappedCode;
    requestEvent();

    return true;
}

void AudioSlave::requestEvent()
{
    Wire.write(static_cast<uint8_t>(static_cast<int8_t>(_requestedEventCode)));
}

int AudioSlave::getLastReceivedValue() const
{
    return _receivedValue;
}
