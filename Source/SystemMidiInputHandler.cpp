//
//  SystemMidiInputHandler.cpp
//  WubDaddy
//
//  Created by Michael Defranco on 10/16/24.
//

#include "SystemMidiInputHandler.h"

SystemMidiInputHandler::SystemMidiInputHandler()
{
    // Constructor
}

SystemMidiInputHandler::~SystemMidiInputHandler()
{
    stopListening();
}

void SystemMidiInputHandler::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        int noteNumber = message.getNoteNumber();
        DBG("MIDI Note On: " << noteNumber);

        // Map the MIDI note to the corresponding globalButtonArray index
        switch (noteNumber)
        {
            case 24: // C1
                globalButtonArray[0].triggerClick();
                break;
            case 25: // C#1
                globalButtonArray[1].triggerClick();
                break;
            case 26: // D1
                globalButtonArray[2].triggerClick();
                break;
            case 28: // E#1
                globalButtonArray[3].triggerClick();
                break;
            default:
                break;
        }
    }
    else if (message.isNoteOff())
    {
        DBG("MIDI Note Off: " << message.getNoteNumber());
    }
    else
    {
        DBG("MIDI Message: " << message.getDescription());
    }
}

void SystemMidiInputHandler::startListening()
{
    auto midiDevices = juce::MidiInput::getAvailableDevices();
    if (!midiDevices.isEmpty())
    {
        for (const auto& device : midiDevices)
        {
            auto midiInput = juce::MidiInput::openDevice(device.identifier, this);
            if (midiInput != nullptr)
            {
                midiInput->start();
                midiInputs.add(std::move(midiInput));
                DBG("Listening for MIDI on: " << device.name);
            }
        }
    }
}

void SystemMidiInputHandler::stopListening()
{
    for (auto* input : midiInputs)
    {
        input->stop();
    }
    midiInputs.clear();
}
