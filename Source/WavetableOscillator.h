/*
  ==============================================================================

    WavetableOscillator.h
    Created: 14 Aug 2024 5:54:17pm
    Author:  Peter Ivanov

  ==============================================================================
*/
#include <JuceHeader.h>

#pragma once

class WavetableOscillator
{
public:
    
    WavetableOscillator (AudioSampleBuffer& wavetableToUse)
        : wavetable (wavetableToUse)
    {
        //what is point of below?
        //jassert (wavetable.getNumChannels() == 1);
    }
    
    void setWavetable(AudioSampleBuffer& wavetableToUse)
    {
        //wavetable = wavetableToUse;
    }
    
    void prepare (const dsp::ProcessSpec& spec) noexcept
    {
        sampleRate = (float) spec.sampleRate;
        //rampBuffer.resize ((int) spec.maximumBlockSize);
        //reset();
    }
    
    
    void setFrequency (float frequency)
    {
        auto tableSizeOverSampleRate = (float) wavetable.getNumSamples() / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }

    //oriignal implementation
    forcedinline float getNextSample() noexcept
    {
        auto tableSize = (unsigned int) wavetable.getNumSamples();
 
        auto index0 = (unsigned int) currentIndex;              // [6]
        auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1;
 
        auto frac = currentIndex - (float) index0;              // [7]
 
        auto* table = wavetable.getReadPointer(0);             // [8]
        auto value0 = table[index0];
        auto value1 = table[index1];
 
        auto currentSample = value0 + frac * (value1 - value0); // [9]
 
        if ((currentIndex += tableDelta) > (float) tableSize)   // [10]
            currentIndex -= (float) tableSize;
 
        return currentSample;
    }
    
    
    forcedinline float getSample(double position) noexcept
    {
        auto tableSize = (unsigned int) wavetable.getNumSamples();
        auto relativePosition = position*tableSize;
        
        
        
        auto index0 = (unsigned int) relativePosition;              // [6]
        auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1;
 
        auto frac = relativePosition - (float) index0;              // [7]
 
        auto* table = wavetable.getReadPointer(0);             // [8]
        auto value0 = table[index0];
        auto value1 = table[index1];
 
        auto currentSample = value0 + frac * (value1 - value0); // [9]
 
 
        return currentSample;
    }
    
    

private:
    const AudioSampleBuffer& wavetable;
    float currentIndex = 0.0f, tableDelta = 0.0f;
    
    float sampleRate;
    float frequency;
};


