/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Engine.h"
#include "Parameters.h"
#include "EnvelopeEditor.h"
#include "SystemMidiInputHandler.h"



//==============================================================================
/**
*/
class WubDaddyAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    WubDaddyAudioProcessor();
    ~WubDaddyAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
    // Encapsulate this better in production <- This is what the JUCE guys say, not sure
    AudioProcessorValueTreeState apvts;
    
    //We probably don't want to pass entire envelop editor to engine
    //But i guess we should define it here and initialize in constructor
    //We want to pass it to editor
    
    //we just want the msegVector that's 
    EnvelopeEditor envEd, envEd2;
    
    //problem intiiailizing before initializer, then passing it to engine
    //is there benefit to unique_ptr? Why does parameterLayout use unique_ptr?
    
    std::vector<std::unique_ptr<EnvelopeEditor>> envEds;
    
    
    
    EnvelopeEditor envEditorArray [5];
    
    
    AudioPlayHead* playHead;
    AudioPlayHead::PositionInfo hostInfo;
    //std::vector<EnvelopeEditor> envEdVector; {EnvelopeEditor(), EnvelopeEditor()};

    //envEds.push_back(std::make_unique<EnvelopeEditor>());
    //envEds.push_back(std::make_unique<EnvelopeEditor>());

    void createEditorLayout()
    {
        //std::vector<std::unique_ptr<EnvelopeEditor>> envEds;

        envEds.push_back(std::make_unique<EnvelopeEditor>());
        envEds.push_back(std::make_unique<EnvelopeEditor>());
        
        //return envEds;
    }

    //envEds.pushBack(envEd);
    //envEds.pushBack(envEd2);

    Engine engine;
    
    //TODO: add undoManager
    
    double mHostBPM;
    
private:
    SystemMidiInputHandler midiHandler;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WubDaddyAudioProcessor)
};
