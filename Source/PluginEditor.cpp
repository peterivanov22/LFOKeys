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
    
    setLookAndFeel(&wubLookAndFeel);

    


    for (auto&& envEd : envEds ){
        addAndMakeVisible(*envEd);
    }
    
    //TODO:  make frequency sliders logarithmic



    
    for (int i = 0; i < maxEnvelopes; i++){
        volumeAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::volumeStringArray[i], volumeSliderArray[i]));
        rateAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::rateStringArray[i], rateSliderArray[i]));
        eqMinAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::EQMinStringArray[i], eqSliderMinArray[i]));
        eqMaxAttachmentArray[i].reset(new SliderAttachment(apvts, Parameters::EQMaxStringArray[i], eqSliderMaxArray[i]));
        

        
        
        volumeButtonArray.push_back(std::make_unique<juce::ShapeButton>("NONAME",WubLookAndFeel::offToggleColor,WubLookAndFeel::offToggleColor, WubLookAndFeel::offToggleColor));
        eqButtonArray.push_back(std::make_unique<juce::ShapeButton>("NONAME",WubLookAndFeel::offToggleColor,WubLookAndFeel::offToggleColor,WubLookAndFeel::offToggleColor));
        globalButtonArray.push_back(std::make_unique<juce::ShapeButton>("NONAME",WubLookAndFeel::offToggleColor,WubLookAndFeel::offToggleColor,WubLookAndFeel::offToggleColor));
        
        
        //below are magic values
        float largeSize= 1000.0f;
        float smallSize= 0.0000005f;
        float cornerSize = 0.00000005f;
        float cornerSizeLarge = 200.0f;


        Path smallPath, largePath;
        smallPath.addRoundedRectangle(0, 0, smallSize, smallSize, cornerSize);
        largePath.addRoundedRectangle(0, 0, 7.0*largeSize, largeSize, cornerSizeLarge);
        
        //setting the shapes of the toggle buttons
        volumeButtonArray[i]->setShape(smallPath, false, true, true);
        eqButtonArray[i]->setShape(smallPath, false, true, true);
        globalButtonArray[i]->setShape(largePath, false, true, true);
        
        
        volumeButtonAttachmentArray[i].reset(new ButtonAttachment(apvts, Parameters::volumeToggleStringArray[i], *volumeButtonArray[i]));
        eqButtonAttachmentArray[i].reset(new ButtonAttachment(apvts, Parameters::EQToggleStringArray[i], *eqButtonArray[i]));
        globalButtonAttachmentArray[i].reset(new ButtonAttachment(apvts, Parameters::globalToggleStringArray[i], *globalButtonArray[i]));
        
        volumeButtonArray[i]->addListener(this);
        eqButtonArray[i]->addListener(this);
        globalButtonArray[i]->addListener(this);

        

        volumeButtonArray[i]->setClickingTogglesState(true);
        eqButtonArray[i]->setClickingTogglesState(true);
        globalButtonArray[i]->setClickingTogglesState(true);
        

        volumeButtonArray[i]->shouldUseOnColours(true);
        eqButtonArray[i]->shouldUseOnColours(true);
        globalButtonArray[i]->shouldUseOnColours(true);
        
        volumeButtonArray[i]->setOnColours(WubLookAndFeel::onToggleColor, WubLookAndFeel::onToggleColor, WubLookAndFeel::onToggleColor);
        eqButtonArray[i]->setOnColours(WubLookAndFeel::onToggleColor, WubLookAndFeel::onToggleColor, WubLookAndFeel::onToggleColor);
        globalButtonArray[i]->setOnColours(WubLookAndFeel::onToggleColor, WubLookAndFeel::onToggleColor, WubLookAndFeel::onToggleColor);

        

        globalButtonArray[i]->setRadioGroupId(1);
        
        
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
                return juce::String("1");
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

        //rateSliderArray[i].setValue(0.5);

        eqSliderMinArray[i].textFromValueFunction = [](double value)
        {
            int temp = int (value);
            String res =  String(temp) + "hz";
            return res;
        };
        
        eqSliderMaxArray[i].textFromValueFunction = [](double value)
        {
            int temp = int (value);
            String res =  String(temp) + "hz";
            return res;
        };
        
        rateSliderArray[i].setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 10);
        //volumeSliderArray[i].setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 10);
        volumeSliderArray[i].setTextBoxStyle (Slider::NoTextBox, false, 50, 10);
        //volumeSliderArray[i].setJustificationType(horizontallyCentred);
        //volumeSliderArray[i].setLookAndFeel(&wubLookAndFeel);
        
        eqSliderMinArray[i].setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 10);
        eqSliderMaxArray[i].setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 10);
        
        rateSliderArray[i].setPopupDisplayEnabled(true, true, this);
        

        eqSliderMinArray[i].setPopupDisplayEnabled(true, true, this);
        eqSliderMaxArray[i].setPopupDisplayEnabled(true, true, this);
        
        addAndMakeVisible(*volumeButtonArray[i]);
        addAndMakeVisible(*eqButtonArray[i]);
        addAndMakeVisible(*globalButtonArray[i]);


        addAndMakeVisible(volumeSliderArray[i]);
        addAndMakeVisible(eqSliderMinArray[i]);
        addAndMakeVisible(eqSliderMaxArray[i]);
        addAndMakeVisible(rateSliderArray[i]);


        //labels below this

        rateLabelArray[i].setText("rate", juce::dontSendNotification);
        rateLabelArray[i].setJustificationType(Justification::horizontallyCentred);

        volumeLabelArray[i].setText("vol", juce::dontSendNotification);
        
        
        //volumeLabelArray[i].setBorderSize(juce::BorderSize<int> (0));
        volumeLabelArray[i].setJustificationType(Justification::horizontallyCentred);
        //volumeLabelArray[i].attachToComponent(&volumeSliderArray[i], false);

        eqMinLabelArray[i].setText("lowcut", juce::dontSendNotification);
        eqMinLabelArray[i].setJustificationType(Justification::horizontallyCentred);
        //eqMinLabelArray[i].attachToComponent(&eqSliderMinArray[i], false);

        eqMaxLabelArray[i].setText("hicut", juce::dontSendNotification);
        eqMaxLabelArray[i].setJustificationType(Justification::horizontallyCentred);
        //eqMaxLabelArray[i].attachToComponent(&eqSliderMaxArray[i], false);
        
        addAndMakeVisible(rateLabelArray[i]);
        addAndMakeVisible(volumeLabelArray[i]);
        addAndMakeVisible(eqMinLabelArray[i]);
        addAndMakeVisible(eqMaxLabelArray[i]);
        
    }

    


    



    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    

    setResizable(true, true);

    double appWidth = 1100;
    double appHeight = 400;


    const float ratio =  (appWidth/appHeight);
    getConstrainer ()->setFixedAspectRatio (ratio);
    setResizeLimits (550, 200, 2200, 800);

        

    setSize (appWidth, appHeight);
}

WubDaddyAudioProcessorEditor::~WubDaddyAudioProcessorEditor()
{
    setLookAndFeel(nullptr);

}

//==============================================================================
void WubDaddyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.fillAll(WubLookAndFeel::gradient[4]);
    //g.fillAll(juce::Colour (69, 56, 42) ); //grey brown
    g.fillAll(WubLookAndFeel::colourBackground);  //dark organge brown FAVR
    //g.fillAll(juce::Colour (59, 34, 7) ); //darker brwon FAVE
    //g.fillAll(juce::Colour (43, 23, 3) ); //darkerer brwon

    //g.fillAll(Colours::black);
    
    
    g.setColour (WubLookAndFeel::onToggleColor);
    g.setFont (15.0f);

    //below "clicks" on the plugin so it has focus, so it receives keypresses
    //TODO: probably a better/ more robust way to do this
    
    if (!this->hasKeyboardFocus(false) && this->isVisible())
    {
      this->grabKeyboardFocus();
    }
    
    
    
    
    
    //g.strokePath(p, PathStrokeType(4.0f));
    //g.drawRoundedRectangle(borderArray[0],1.0f,3.0f);
    for (int i=0; i<5; i++){
        if(globalButtonArray[i]->getToggleState()){
            g.drawRoundedRectangle(borderArray[i], 15.0f, 4.0f);
        }
        if (volumeButtonArray[i]->getToggleState()){
            g.drawRoundedRectangle(volumeBorderArray[i], 10.0f, 0.75f);
        }
        if (eqButtonArray[i]->getToggleState()){
            g.drawRoundedRectangle(eqBorderArray[i], 10.0f, 0.75f);
        }


    }
    

}

void WubDaddyAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto length = envEds.size();
    
    
    
    
    Rectangle<int> boundsArray [maxEnvelopes];
    
    
    Rectangle<int> verticalStripArray[maxEnvelopes];
    Rectangle<int> bottomArray [maxEnvelopes];
    Rectangle<int> topArray [maxEnvelopes];
    Rectangle<int> realTopArray [maxEnvelopes];


    Rectangle<int> volumeArray [maxEnvelopes];
    Rectangle<int> rateArray [maxEnvelopes];

    
    Rectangle<int> eqMinArray [maxEnvelopes];
    Rectangle<int> eqMaxArray [maxEnvelopes];
    


   
    
    //chop the screen into 3 horizontal strips (top is top 1/5, bounds is middle 2/5, bottom is bot 2/5)
   
    juce::Rectangle<int> bounds = getLocalBounds();
    
    int sideMargin = bounds.getWidth()/100;
    int topMargin = bounds.getHeight()/25;
    
    //padding around exterior of window
    bounds.removeFromLeft(sideMargin);
    //bounds.removeFromRight(sideMargin);
    bounds.removeFromTop(topMargin/2);
    bounds.removeFromBottom(topMargin/2);
    
    
    for (int i=0; i<maxEnvelopes;i++){
        verticalStripArray[i] = bounds.removeFromLeft(bounds.getWidth()/(5-i));
        verticalStripArray[i].removeFromRight(sideMargin);
        
        
        int windowSize = 45*bounds.getHeight()/100;
        int globalToggleSize = 10*bounds.getHeight()/100;
        
        bottomArray[i] = verticalStripArray[i].removeFromBottom(windowSize);
        realTopArray[i] = verticalStripArray[i].removeFromTop(globalToggleSize);
        //

        
        borderArray[i] =  (realTopArray[i].getUnion(verticalStripArray[i].getUnion(bottomArray[i]))).toFloat();
        
        int reduceFactor = 2;
        
        realTopArray[i].removeFromTop(topMargin/2);
        realTopArray[i].removeFromLeft(sideMargin);
        realTopArray[i].removeFromRight(sideMargin);

        
        
        verticalStripArray[i].removeFromLeft(sideMargin);
        verticalStripArray[i].removeFromRight(sideMargin);
        
        bottomArray[i].removeFromLeft(sideMargin);
        bottomArray[i].removeFromRight(sideMargin);
        
        globalButtonArray[i]->setBounds(realTopArray[i]);
        envEds[i]->setBounds(verticalStripArray[i]);


    }
    



    
    
    //draw everything else on bottom half

    
        
    for (int i=0; i<5; i++){
        topArray[i]=bottomArray[i].removeFromTop(bottomArray[i].getHeight()/2);
        
        int knobSize = topArray[i].getWidth()/3;
        
        rateArray[i]=topArray[i].removeFromLeft(knobSize);
        volumeArray[i]=topArray[i].removeFromLeft(knobSize);


        float marginFactor = 2;
        
        rateArray[i].removeFromBottom(topMargin/marginFactor);
        volumeArray[i].removeFromBottom(topMargin/marginFactor);
        topArray[i].removeFromBottom(topMargin/marginFactor);
        
        volumeBorderArray[i] = volumeArray[i].getUnion(topArray[i]).toFloat();

        
        // bottom half
        eqMinArray[i]=bottomArray[i].removeFromLeft(knobSize);
        eqMaxArray[i]=bottomArray[i].removeFromLeft(knobSize);
        
        

        
        eqMinArray[i].removeFromBottom(topMargin/1.75);
        eqMaxArray[i].removeFromBottom(topMargin/1.75);
        bottomArray[i].removeFromBottom(topMargin/1.75);
        
        eqBorderArray[i] =  (eqMinArray[i].getUnion(eqMaxArray[i].getUnion(bottomArray[i]))).toFloat();

    }
    
    
 
    for(int i =0; i< 5; i++){
        
        //play with
        int labelSize = volumeArray[i].getHeight()/3.5;
        int labelMargin = labelSize/3;

        
        Rectangle<int> temp = rateArray[i].removeFromTop(labelSize);
        temp.removeFromTop(labelMargin);
        
        rateSliderArray[i].setBounds(rateArray[i]);
        rateLabelArray[i].setBounds(temp);
        
        temp = volumeArray[i].removeFromTop(labelSize);
        temp.removeFromTop(labelMargin);

        volumeSliderArray[i].setBounds(volumeArray[i]);
        volumeLabelArray[i].setBounds(temp);
        //volumeLabelArray[i].setBorderSize(BorderSize<int>(0));
        
        //float reduceSize = 14.0f;
        int reduceHeightSize = topArray[i].getHeight()/4;
        int reduceWidthSize = topArray[i].getWidth()/4;

        //topArray[i].reduce(reduceSize,reduceSize);
        topArray[i].removeFromTop(reduceHeightSize);
        topArray[i].removeFromBottom(reduceHeightSize);
        topArray[i].removeFromLeft(reduceWidthSize);
        topArray[i].removeFromRight(reduceWidthSize);

        volumeButtonArray[i]->setBounds(topArray[i]);


        
        
        temp = eqMinArray[i].removeFromTop(labelSize);
        temp.removeFromTop(labelMargin);
        
        eqSliderMinArray[i].setBounds(eqMinArray[i]);
        eqMinLabelArray[i].setBounds(temp);
        
        temp = eqMaxArray[i].removeFromTop(labelSize);
        temp.removeFromTop(labelMargin);
        
        eqSliderMaxArray[i].setBounds(eqMaxArray[i]);
        eqMaxLabelArray[i].setBounds(temp);

        //bottomArray[i].reduce(reduceSize,reduceSize);
        bottomArray[i].removeFromTop(reduceHeightSize);
        bottomArray[i].removeFromBottom(reduceHeightSize);
        bottomArray[i].removeFromLeft(reduceWidthSize);
        bottomArray[i].removeFromRight(reduceWidthSize);
        //bottomArray[i].removeFromTop()
        
        
        eqButtonArray[i]->setBounds(bottomArray[i]);
    }

}






//key press logic to turn on/off global toggles

bool WubDaddyAudioProcessorEditor::keyPressed(const KeyPress &k) {
    if (k.getTextCharacter() == 'a' || k.getTextCharacter() == 'A' ) {
        globalButtonArray[0]->triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 's'|| k.getTextCharacter() == 'S') {
        globalButtonArray[1]->triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 'd' || k.getTextCharacter() == 'D') {
        globalButtonArray[2]->triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 'f'|| k.getTextCharacter() == 'F') {
        globalButtonArray[3]->triggerClick();
        return true;
    }
    if (k.getTextCharacter() == 'g' || k.getTextCharacter() == 'G') {
        globalButtonArray[4]->triggerClick();
        return true;
    }

    return false;
}


void WubDaddyAudioProcessorEditor::buttonClicked (Button *button)
{
    repaint();
}
