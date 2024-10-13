/*
  ==============================================================================

    LFO.cpp
    Created: 9 Aug 2024 12:41:21pm
    Author:  Peter Ivanov

  ==============================================================================
*/

#include "LFO.h"



LFO::LFO(AudioProcessorValueTreeState & a):
    apvts(a)
{
  
    //set up lfo shape and frequency
    oscillator.initialise ([] (float x) { return std::sin(x); }, 128);
    oscillator.setFrequency (2000.0f);
    
    gain.setGainLinear(1.0f);

    
}


void LFO::prepare(const dsp::ProcessSpec& spec)
{

    //Below is to update LFO less frequently
    //oscillator.prepare ({ spec.sampleRate / lfoUpdateRate, spec.maximumBlockSize, spec.numChannels }); // [4
    
    oscillator.prepare(spec);
    gain.prepare(spec);
}



void LFO::process(const dsp::ProcessContextReplacing<float>& context)
{

    //my attempt below
    auto lfoOut = oscillator.processSample(0.0f);
    auto lfoVolume = juce::jmap (lfoOut, -1.0f, 1.0f, 0.0f, 1.0f);
    gain.setGainLinear(lfoVolume);
    gain.process(context);

}

float LFO::processSample (int channel, float inputValue)
{
    return 0.0f;
}
/*
float LFO::processSample (int channel, float inputValue)
{
    //compressor processSample
    
    // Ballistics filter with peak rectifier
    auto env = envelopeFilter.processSample (channel, inputValue);

    // VCA
    auto gain = (env < threshold) ? static_cast<SampleType> (1.0)
                                  : std::pow (env * thresholdInverse, ratioInverse - static_cast<SampleType> (1.0));

    // Output
    return gain * inputValue;
}
*/


void LFO::reset()
{
    oscillator.reset();
    gain.reset();

}

void LFO::parameterChanged(const String& parameterID, float newValue )
{
    
    //if (parameterID == Parameters::ID_DISTORTION) {
    //    setWaveshaperFunction(newValue);

}


void LFO::updateParameters()
{
    //example line below
    //dryWetMix.setDryDecibels(*parameters.getRawParameterValue(Params::ID_DRY_GAIN));
}
    
