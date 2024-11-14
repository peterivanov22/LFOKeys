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
            String dummy = "Dummy" + String(i);
            
            float rateTemp, minTemp, maxTemp;
            
            if (i==1){
                rateTemp = 0.75f;
            }
            else if (i==2){
                rateTemp = 0.25f;
            }
            else if(i==3){
                rateTemp = 0.0f;
            }
            else{
                rateTemp = 0.5f;
            }
            params.push_back(std::make_unique<juce::AudioParameterFloat> (rateStringArray[i],"Rate", 0.0f, 1.0f, rateTemp));

            params.push_back(std::make_unique<juce::AudioParameterFloat> (volumeStringArray[i], "Volume", 0.0f, 1.0f, 1.0f));
            
            
            //eqSliderMinArray[1].setValue(625);
            //eqSliderMinArray[3].setValue(350);
            //eqSliderMaxArray[2].setValue(1250);

            if (i==1)
                minTemp = 625.0f;
            else if (i ==3)
                minTemp = 350.0f;
            else
                minTemp = 1.0f;
            
            params.push_back(std::make_unique<juce::AudioParameterFloat> (EQMinStringArray[i], "LowCut", 1.0f, 20000.0f, minTemp));
            
            if (i==2)
                maxTemp =1250;
            else
                maxTemp = 20000.0f;
            
            params.push_back(std::make_unique<juce::AudioParameterFloat> (EQMaxStringArray[i], "HighCut", 1.0f, 20000.0f, maxTemp));



            bool temp;

            if (i==0)
                temp = true;
            else
                temp = false;
            params.push_back(std::make_unique<juce::AudioParameterBool> (globalToggleStringArray[i],"Global Toggle", temp));
            
            if (i==2 || i==3){
                temp = false;
            }
            else
                temp = true;
            
            params.push_back(std::make_unique<juce::AudioParameterBool> (volumeToggleStringArray[i],"Volume Toggle", temp));
            
            if(i==0 || i==1){
                temp =false;
            }
            else
                temp = true;
            params.push_back(std::make_unique<juce::AudioParameterBool> (EQToggleStringArray[i],"EQ Toggle", temp));
            
            //add below  so theres 8 parameters so they are lined up in ableton (columns of 4)
            params.push_back(std::make_unique<juce::AudioParameterBool> (dummy,"Dummy ", false));
        }
 

        return { params.begin(), params.end() };
    }
    
};
