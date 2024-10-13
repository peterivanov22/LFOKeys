/*
  ==============================================================================

    Parameters.h
    Created: 25 Jul 2024 9:25:21pm
    Author:  Peter Ivanov

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Parameters
{
public:
    static const int maxEnvelopes = 5;
    
    inline static const String
        ID_EQMIN = "eqmin",
        ID_EQMAX = "eqmax",
        ID_VOLUME = "volume",
        ID_RATE = "rate",
        ID_GLOBAL_TOGGLE = "globaltoggle",
        ID_VOLUME_TOGGLE = "volumetoggle",
        ID_EQ_TOGGLE = "eqtoggle";
;

    inline static String EQMinStringArray[maxEnvelopes];
    inline static String EQMaxStringArray[maxEnvelopes];
    inline static String volumeStringArray[maxEnvelopes];
    inline static String rateStringArray[maxEnvelopes];
    inline static String globalToggleStringArray[maxEnvelopes];
    inline static String volumeToggleStringArray[maxEnvelopes];
    inline static String EQToggleStringArray[maxEnvelopes];



    
    //ID_MIX = "mix",
    //ID_GAIN = "gain";
        

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;
        
        for (int i=0; i< maxEnvelopes; i++){
            
            EQMinStringArray[i] = ID_EQMIN + String(i);
            EQMaxStringArray[i] = ID_EQMAX + String(i);
            volumeStringArray[i] = ID_VOLUME + String(i);
            rateStringArray[i] = ID_RATE + String(i);
            globalToggleStringArray[i] = ID_GLOBAL_TOGGLE + String(i);
            volumeToggleStringArray[i] = ID_VOLUME_TOGGLE + String(i);
            EQToggleStringArray[i] = ID_EQ_TOGGLE + String(i);
            
            
            params.push_back(std::make_unique<juce::AudioParameterFloat> (volumeStringArray[i], "Volume", 0.0f, 1.0f, 1.0f));
            params.push_back(std::make_unique<juce::AudioParameterFloat> (EQMinStringArray[i], "Low FQ", 1.0f, 20000.0f, 1.0f));
            params.push_back(std::make_unique<juce::AudioParameterFloat> (EQMaxStringArray[i], "High FQ", 1.0f, 20000.0f, 20000.0f));
            params.push_back(std::make_unique<juce::AudioParameterFloat> (rateStringArray[i],"Rate", 0.0f, 1.0f, 0.0f));
            params.push_back(std::make_unique<juce::AudioParameterBool> (globalToggleStringArray[i],"Global Toggle", false));
            params.push_back(std::make_unique<juce::AudioParameterBool> (volumeToggleStringArray[i],"Volume Toggle", true));
            params.push_back(std::make_unique<juce::AudioParameterBool> (EQToggleStringArray[i],"EQ Toggle", true));

        }
 

        return { params.begin(), params.end() };
    }
    
};
