/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//#include "EnvelopeEditor.h"


//==============================================================================
/**
*/
class WubDaddyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WubDaddyAudioProcessorEditor (WubDaddyAudioProcessor&);
    ~WubDaddyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    
    

protected:

    
private:
    static const int maxEnvelopes = 5;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WubDaddyAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& apvts;
    
    
    Slider eqSliderArray[maxEnvelopes];
    Slider volumeSliderArray[maxEnvelopes];
    Slider eqSliderMinArray [maxEnvelopes], eqSliderMaxArray[maxEnvelopes], rateSliderArray[maxEnvelopes];
    
    ToggleButton volumeButtonArray[maxEnvelopes], eqButtonArray[maxEnvelopes], globalButtonArray[maxEnvelopes];
    

    
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    std::unique_ptr<SliderAttachment> eqAttachmentArray[maxEnvelopes], eqMinAttachmentArray[maxEnvelopes], eqMaxAttachmentArray[maxEnvelopes],
        volumeAttachmentArray[maxEnvelopes], rateAttachmentArray[maxEnvelopes];
    
    std::unique_ptr<ButtonAttachment> volumeButtonAttachmentArray[maxEnvelopes], eqButtonAttachmentArray[maxEnvelopes], globalButtonAttachmentArray[maxEnvelopes];

    Label eqMinLabelArray[maxEnvelopes], eqMaxLabelArray[maxEnvelopes], volumeLabelArray[maxEnvelopes], rateLabelArray[maxEnvelopes];
   // EnvelopeEditor& envEd;
    //EnvelopeEditor& envEd2;
    
    std::vector<std::unique_ptr<EnvelopeEditor>>& envEds;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WubDaddyAudioProcessorEditor)
};
