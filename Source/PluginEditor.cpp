/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WubDaddyAudioProcessorEditor::WubDaddyAudioProcessorEditor (WubDaddyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), apvts(p.apvts), envEds(p.envEds)
{
    
    
    
    for (auto&& envEd : envEds ){
        addAndMakeVisible(*envEd);
    }
    
    //TODO:  make frequency sliders logarithmic

    for (int i = 0; i < maxEnvelopes; i++){
        volumeAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::volumeStringArray[i], volumeSliderArray[i]));
        rateAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::rateStringArray[i], rateSliderArray[i]));
        eqMinAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::EQMinStringArray[i], eqSliderMinArray[i]));
        eqMaxAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::EQMaxStringArray[i], eqSliderMaxArray[i]));
        
        volumeButtonAttachmentArray[i].reset(new ButtonAttachment(apvts, Parameters::volumeToggleStringArray[i], volumeButtonArray[i]));
        eqButtonAttachmentArray[i].reset(new ButtonAttachment(apvts, Parameters::EQToggleStringArray[i], eqButtonArray[i]));
        globalButtonAttachmentArray[i].reset(new ButtonAttachment(apvts, Parameters::globalToggleStringArray[i], globalButtonArray[i]));
        
        globalButtonArray[i].setRadioGroupId(1);
        
        
        volumeSliderArray[i].setSliderStyle(Slider::Rotary);
        volumeSliderArray[i].setRange(0.0,1.0);
        
        eqSliderMinArray[i].setSliderStyle(Slider::Rotary);
        eqSliderMinArray[i].setRange(0.0, 20000.0);
        eqSliderMinArray[i].setSkewFactor(0.25);

        eqSliderMaxArray[i].setSliderStyle(Slider::Rotary);
        eqSliderMaxArray[i].setRange(0.0, 20000.0);
        eqSliderMaxArray[i].setSkewFactor(0.25);
        
        rateSliderArray[i].setSliderStyle(Slider::Rotary);
        rateSliderArray[i].setRange(0.0, 1.0, 0.25);
        
        rateSliderArray[i].textFromValueFunction = [](double value)
        {
            if (value == 0)
                return juce::String(1);
            if (value == 0.25)
                return juce::String("1/2");
            if (value == 0.5)
                return juce::String("1/4");
            if (value == 0.75)
                return juce::String("1/8");
            if (value == 1)
                return juce::String("1/16");
            else
                return String("IDK");
        };

        rateSliderArray[i].setValue(0.5);

        
        rateSliderArray[i].setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 10);
        volumeSliderArray[i].setTextBoxStyle(juce::Slider::NoTextBox, true, 200, 30);

        
        eqSliderMinArray[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 10);
        eqSliderMaxArray[i].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 10);
        
        
        
        addAndMakeVisible(volumeButtonArray[i]);
        addAndMakeVisible(eqButtonArray[i]);
        addAndMakeVisible(globalButtonArray[i]);


        addAndMakeVisible(volumeSliderArray[i]);
        addAndMakeVisible(eqSliderMinArray[i]);
        addAndMakeVisible(eqSliderMaxArray[i]);
        addAndMakeVisible(rateSliderArray[i]);


        //labels below this

        volumeLabelArray[i].setText("Volume", juce::dontSendNotification);
        volumeLabelArray[i].attachToComponent(&volumeSliderArray[i], false);

        eqMinLabelArray[i].setText("Low Cutoff", juce::dontSendNotification);
        eqMinLabelArray[i].attachToComponent(&eqSliderMinArray[i], false);

        eqMaxLabelArray[i].setText("High Cutoff", juce::dontSendNotification);
        eqMaxLabelArray[i].attachToComponent(&eqSliderMaxArray[i], false);
        
        addAndMakeVisible(volumeLabelArray[i]);
        addAndMakeVisible(eqMinLabelArray[i]);
        addAndMakeVisible(eqMaxLabelArray[i]);
        
    }

    
    //change initial values for certain parameters
    rateSliderArray[1].setValue(0.75);
    rateSliderArray[2].setValue(0);


    



    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 400);
    
}

WubDaddyAudioProcessorEditor::~WubDaddyAudioProcessorEditor()
{
}

//==============================================================================
void WubDaddyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    //below "clicks" on the plugin so it has focus, so it receives keypresses
    //TODO: probably a better/ more robust way to do this
    /*
    if (!this->hasKeyboardFocus(false) && this->isVisible())
    {
      this->grabKeyboardFocus();
    }
    */
    

}

void WubDaddyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto length = envEds.size();
    
    
    Rectangle<int> boundsArray [5];
    
    Rectangle<int> bottomArray [5];
    Rectangle<int> topArray [5];
    Rectangle<int> realTopArray [5];


    Rectangle<int> volumeArray [5];
    Rectangle<int> rateArray [5];

    
    Rectangle<int> eqMinArray [5];
    Rectangle<int> eqMaxArray [5];



   
    
    //chop the screen into 3 horizontal strips (top is top 1/5, bounds is middle 2/5, bottom is bot 2/5)
   
    juce::Rectangle<int> bounds = getLocalBounds();
    
    
    juce::Rectangle<int> bottom = bounds.removeFromBottom(9*bounds.getHeight()/20);
    
    
    juce::Rectangle<int> top = bounds.removeFromTop(2*bounds.getHeight()/11);

    
    //draw the global toggle buttons on top 5
    
    //next divides the top strip into 5 equal size parts
    
    for (int i=0; i<5; i++){
        realTopArray[i]=top.removeFromLeft(top.getWidth()/(5-i));
        
        globalButtonArray[i].setBounds(realTopArray[i]);
    }
    
    
    
    //draw the lfos in middle strip
    
    for (int i=0; i<5; i++){
        boundsArray[i]=bounds.removeFromLeft(bounds.getWidth()/(5-i));
    }
    
    
    auto itEnv = envEds.begin();
    int i = 0;
    while(itEnv != envEds.end())
    {
        //(*itEnv)->setBounds(*itBounds);
        (*itEnv)->setBounds(boundsArray[i]);
        ++itEnv;
        ++i;
        //++itBounds;
    }
    

    //draw everything else on bottom half


    
    for (int i=0; i<5; i++){
        bottomArray[i]=bottom.removeFromLeft(bottom.getWidth()/(5-i));
    }
    
        
    for (int i=0; i<5; i++){
        topArray[i]=bottomArray[i].removeFromTop(bottomArray[i].getHeight()/2);
    }
    
    
    for (int i=0; i<5; i++){
        rateArray[i]=topArray[i].removeFromLeft(topArray[i].getWidth()/3);
    }
    
    for (int i=0; i<5; i++){
        volumeArray[i]=topArray[i].removeFromLeft(topArray[i].getWidth()/2);
    }
    
    for (int i=0; i<5; i++){
        eqMinArray[i]=bottomArray[i].removeFromLeft(bottomArray[i].getWidth()/3);
    }
    
    for (int i=0; i<5; i++){
        eqMaxArray[i]=bottomArray[i].removeFromLeft(bottomArray[i].getWidth()/2);
    }
    
    for(int i =0; i< 5; i++){
        rateSliderArray[i].setBounds(rateArray[i]);
        volumeSliderArray[i].setBounds(volumeArray[i]);
        volumeButtonArray[i].setBounds(topArray[i]);
        
        eqSliderMinArray[i].setBounds(eqMinArray[i]);
        eqSliderMaxArray[i].setBounds(eqMaxArray[i]);
        eqButtonArray[i].setBounds(bottomArray[i]);
    }

    
    


}


//key press logic to turn on/off global toggles

bool WubDaddyAudioProcessorEditor::keyPressed(const KeyPress &k) {
    if (k.getTextCharacter() == 'a') {
        globalButtonArray[0].triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 's') {
        globalButtonArray[1].triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 'd') {
        globalButtonArray[2].triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 'f') {
        globalButtonArray[3].triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 'g') {
        globalButtonArray[4].triggerClick();
        return true;
    }

    return false;
}
