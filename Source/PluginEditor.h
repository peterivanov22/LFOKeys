/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"

//#include "EnvelopeEditor.h"


//==============================================================================
/**
*/
class WubDaddyAudioProcessorEditor  : public juce::AudioProcessorEditor, public Button::Listener
{
public:
    WubDaddyAudioProcessorEditor (WubDaddyAudioProcessor&);
    ~WubDaddyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked (Button *button) override;

    

protected:

    
private:
    static const int maxEnvelopes = 5;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WubDaddyAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& apvts;
    
    WubLookAndFeel wubLookAndFeel;

    
    Slider eqSliderArray[maxEnvelopes];
    Slider volumeSliderArray[maxEnvelopes];
    Slider eqSliderMinArray [maxEnvelopes], eqSliderMaxArray[maxEnvelopes], rateSliderArray[maxEnvelopes];
        
    //ShapeButton volumeButtonArray[maxEnvelopes];
    //ShapeButton eqButtonArray[maxEnvelopes];
    //ShapeButton globalButtonArray[maxEnvelopes];
    
    std::vector<std::unique_ptr<juce::ShapeButton>> volumeButtonArray;
    std::vector<std::unique_ptr<juce::ShapeButton>> eqButtonArray;
    std::vector<std::unique_ptr<juce::ShapeButton>> globalButtonArray;
    
    //std::vector<ShapeButton> volumeButtonArray{ ShapeButton("", Colours::red, Colours::grey, Colours::green),ShapeButton("", Colours::red, Colours::grey, Colours::green), ShapeButton("", Colours::red, Colours::grey, Colours::green), ShapeButton("", Colours::red, Colours::grey, Colours::green), ShapeButton("", Colours::red, Colours::grey, Colours::green) };
    //std::vector<ShapeButton> eqButtonArray;
    //std::vector<ShapeButton> globalButtonArray;
    //std::vector<Object> objects(10, Object("foo", "bar"));
    
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    std::unique_ptr<SliderAttachment> eqAttachmentArray[maxEnvelopes], eqMinAttachmentArray[maxEnvelopes], eqMaxAttachmentArray[maxEnvelopes],
        volumeAttachmentArray[maxEnvelopes], rateAttachmentArray[maxEnvelopes];
    
    std::unique_ptr<ButtonAttachment> volumeButtonAttachmentArray[maxEnvelopes], eqButtonAttachmentArray[maxEnvelopes], globalButtonAttachmentArray[maxEnvelopes];

    Label eqMinLabelArray[maxEnvelopes], eqMaxLabelArray[maxEnvelopes], volumeLabelArray[maxEnvelopes], rateLabelArray[maxEnvelopes];
   // EnvelopeEditor& envEd;
    //EnvelopeEditor& envEd2;
    
    std::vector<std::unique_ptr<EnvelopeEditor>>& envEds;
    
    Rectangle<float> borderArray[5];
    Rectangle<float> volumeBorderArray[5];
    Rectangle<float> eqBorderArray[5];

    
    
    bool keyPressed(const KeyPress &k) override;


    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WubDaddyAudioProcessorEditor)
};
