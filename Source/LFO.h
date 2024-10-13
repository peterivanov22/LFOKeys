/*
  ==============================================================================

    LFO.h
    Created: 9 Aug 2024 12:41:21pm
    Author:  Peter Ivanov

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



class LFO : public dsp::ProcessorBase, public AudioProcessorValueTreeState::Listener
{
public:
    LFO(AudioProcessorValueTreeState & a);
   
    void prepare(const dsp::ProcessSpec& spec) override;
    
    void process(const dsp::ProcessContextReplacing<float>& context) override;
    
    float processSample(int channel, float inputValue);

    void reset() override;
    
    void parameterChanged(const juce::String& parameterID, float newValue ) override;
    
    //void setWaveshaperFunction(float pd);
    
private:
    
    AudioProcessorValueTreeState& apvts;
    //AudioProcessor& processor;

    
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    
    

    
    static constexpr size_t lfoUpdateRate = 100;
    size_t lfoUpdateCounter = lfoUpdateRate;
    juce::dsp::Oscillator<float> oscillator;   // [1]

    
    dsp::Gain<float> gain;

    
    void updateParameters();

};
 
