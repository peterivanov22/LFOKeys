/*
  ==============================================================================

    SystemMidiInputHandler.h
    Created: 16 Oct 2024 9:45:30pm
    Author:  Michael Defranco

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SystemMidiInputHandler : public juce::MidiInputCallback
{
public:
    SystemMidiInputHandler();
    ~SystemMidiInputHandler();

    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void startListening();
    void stopListening();

private:
    juce::OwnedArray<juce::MidiInput> midiInputs;
};
