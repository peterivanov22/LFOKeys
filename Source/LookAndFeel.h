/*
  ==============================================================================

    LookAndFeel.h
    Created: 1 Nov 2024 1:10:16pm
    Author:  Peter Ivanov

  ==============================================================================
*/

class WubLookAndFeel : public LookAndFeel_V4
{
public:
    WubLookAndFeel()
    {
        setColour (Slider::thumbColourId, colourAccent);
        setColour (Slider::trackColourId, colourOff);
        setColour (Slider::backgroundColourId, colourOff);
        setColour (Slider::rotarySliderFillColourId, gradient[3]);
        setColour (TextButton::textColourOffId, Colours::white);
        setColour (TextButton::textColourOnId, Colours::white);
        setColour (Label::textColourId, Colours::white);
        
        //Typeface::Ptr tface = Typeface::createSystemTypefaceFor(BinaryData::MontserratExtraLight_ttf, BinaryData::MontserratExtraLight_ttfSize);
        //Typeface::Ptr tface = Typeface::createSystemTypefaceFor(BinaryData::MontserratLight_ttf, BinaryData::MontserratLight_ttfSize);
        
        //next 3 lines used in crispy
        //Typeface::Ptr tface = Typeface::createSystemTypefaceFor(Cambria, 16);
        //setDefaultSansSerifTypeface (tface);
        //LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(tface);
        
        //TODO: whats binary data from?
        
        LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Cambria");
    }
    
    /*
    public: Slider::SliderLayout getSliderLayout (Slider&) override
     {
     Slider::SliderLayout layout;

     layout.textBoxBounds = Rectangle<int> (0, 0, 70, 20);
     layout.sliderBounds = Rectangle<int> (8, 30, 284, 30);
     return layout;
     }
     */
    
    /*
    Label* createSliderTextBox (Slider& slider) override
        {
            Label* l = LookAndFeel_V4::createSliderTextBox (slider);
            l->setBorderSize (BorderSize<int> (0, 0, 0, 0));
            l->setColour (Label::outlineColourId, Colours::transparentWhite );
            l->setColour (Label::outlineWhenEditingColourId, Colours::transparentWhite );
            return l;
        }
     */
    
    
     void     drawBubble (Graphics &g, BubbleComponent &bubbleComponent, const Point< float > &positionOfTip, const Rectangle< float > &body) override
    {
        g.setColour (WubLookAndFeel::colourBackground);
        g.fillRect(body);
    }
    
    //==============================================================================
    int getSliderThumbRadius (Slider& slider) override
    {
        return 18;
//        return jmin (12, slider.isHorizontal() ? static_cast<int> (slider.getHeight() * 0.5f)
//                                               : static_cast<int> (slider.getWidth()  * 0.5f));
    }
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto radius = jmin (width / 2, height / 2) - 4.0f;
        auto centreX = x + width  * 0.5f;
        auto centreY = y + height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour( slider.findColour(Slider::rotarySliderFillColourId) );
        g.fillEllipse (rx, ry, rw, rw);
        
        Path p;
        auto pointerLength = radius * 0.4f;
        auto pointerThickness = pointerLength / 4;
        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform (AffineTransform::rotation (angle).translated (centreX, centreY));
        // pointer
        g.setColour (Colours::white);
        g.fillPath (p);
    }

    void drawButtonBackground (Graphics& g,
                                               Button& button,
                                               const Colour& backgroundColour,
                                               bool shouldDrawButtonAsHighlighted,
                                               bool shouldDrawButtonAsDown) override
    {
        auto cornerSize = 6.5567f;
        auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);
        
        if (button.getToggleState()){
            g.setColour (colourAccent);
            g.drawRoundedRectangle (bounds, cornerSize, 3.0f);
        }
    }
    /*
    juce::Slider::SliderLayout RotaryLookAndFeel::getSliderLayout(juce::Slider& slider){
        juce::Slider::SliderLayout layout = juce::LookAndFeel_V2::getSliderLayout(slider);

        return layout;
    }
     */
    

    
    
    juce::Array<juce::Colour> inline static const colors { juce::Colour (255, 232, 122),   // yellow
                                        juce::Colour (184, 137, 87),   // orange brown
                                        juce::Colour (117, 212, 208), //teal
                                        juce::Colour (249, 219, 245), //pastel pink
                                        juce::Colour (2, 48, 15), //dark olive green
                                        juce::Colour (150, 85, 5) //deep orange/brown
                                        //juce::Colour (73, 109, 58) //medium dark olive green (OLD)
    }; // blue
    
    juce::Array<juce::Colour> inline static const gradient{
        juce::Colour(251, 214, 65), //pastel yellow
        juce::Colour(226,175,48),
        juce::Colour(201,137,31),
        juce::Colour(175,100,14),
        juce::Colour(149,62,0) //orange brown
    };

    Colour inline static const offToggleColor = gradient[3] ;
    Colour inline static const onToggleColor = gradient[0] ;


    
    
    inline static const Colour
        colourAccent = Colour::fromHSV(109.0f/360.0f, 50.0f/100.0f, 90.0f/100.0f, 0xff),
        colourBackground = juce::Colour (64, 36, 5),
        //colourFont = Colour::fromHSV(0.0f, 0.0f, 0.8, 1.0),
        colourPanel = Colour::fromHSV(0.0f, 0.0f, 42.0f/100.0f, 0xff),
        colourRotary = Colour::fromHSV(0.0f, 0.0f, 70.0f/100.0f, 0xff),
        colourOff = Colour::fromHSV(0.0f, 0.0f, 0.45, 0x11),
        colourFont = Colours::white
    ;
    
private:

};

