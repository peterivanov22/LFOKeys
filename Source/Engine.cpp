/*
  ==============================================================================

    Engine.cpp
    Created: 26 Jul 2024 1:19:26pm
    Author:  Peter Ivanov

  ==============================================================================
*/

#include "Engine.h"
#include "Parameters.h"



Engine::Engine(AudioProcessorValueTreeState & a, AudioProcessor & p, std::vector<std::unique_ptr<EnvelopeEditor>>& envEds):
envEds(envEds),
env(EnvelopeEditor::MSEG_DEFAULT_PIXELS_WIDTH),
apvts(a),
processor(p)

{
    //probably should find better place to initialize envEds then here
    //the integers set initial waveforms, which are defined in EnvelopeEditor.cpp
    envEds.push_back(std::make_unique<EnvelopeEditor>(0));
    envEds.push_back(std::make_unique<EnvelopeEditor>(1));
    envEds.push_back(std::make_unique<EnvelopeEditor>(2));
    envEds.push_back(std::make_unique<EnvelopeEditor>(3));
    envEds.push_back(std::make_unique<EnvelopeEditor>(4));
    
    //apvts.state.setProperty (myCoolParameterID, var(5), nullptr);
    //myCoolParameter.referTo (pluginState.state.getPropertyAsValue (myCoolParameterID, nullptr));


    // 4th envelop causing error with negative numbers
    //envEds.push_back(std::make_unique<EnvelopeEditor>(4));

    int i = 0;
    for (auto&& envEd : envEds ){
        envEd->addChangeListener(this);
        setMSEGTable(*envEd,msegTableArray[i]);
        waveOscArray[i] = new WavetableOscillator(msegTableArray[i]);
        i++;
    }
    
    
    //initialize parameters
    for (int i=0;i<maxEnvelopes;i++){
        apvts.addParameterListener(Parameters::volumeStringArray[i], this);
        apvts.addParameterListener(Parameters::EQMinStringArray[i], this);
        apvts.addParameterListener(Parameters::EQMaxStringArray[i], this);
        apvts.addParameterListener(Parameters::rateStringArray[i], this);
        apvts.addParameterListener(Parameters::volumeToggleStringArray[i], this);
        apvts.addParameterListener(Parameters::EQToggleStringArray[i], this);
        apvts.addParameterListener(Parameters::globalToggleStringArray[i], this);
        


        //initialize variables
        updateRate((float) *apvts.getRawParameterValue(Parameters::rateStringArray[i]), i);

        userVolumeArray[i] = 1.0f - *apvts.getRawParameterValue(Parameters::volumeStringArray[i]);
        userEQMinArray[i]= (double) *apvts.getRawParameterValue(Parameters::EQMinStringArray[i]);
        userEQMaxArray[i]= (double) *apvts.getRawParameterValue(Parameters::EQMaxStringArray[i]);
        
        globalToggleArray[i] = (bool) *apvts.getRawParameterValue(Parameters::globalToggleStringArray[i]);
        volumeToggleArray[i] = (bool) *apvts.getRawParameterValue(Parameters::volumeToggleStringArray[i]);
        EQToggleArray[i] = (bool) *apvts.getRawParameterValue(Parameters::EQToggleStringArray[i]);

        
        lowPassArray[i].setEnabled(EQToggleArray[i]);
        highPassArray[i].setEnabled(EQToggleArray[i]);

        if(globalToggleArray[i]){
            m_rate = rateArray[i];
        }
        
        //do we need below here?
    }
}


void Engine::prepare(const dsp::ProcessSpec& spec)
{
    
    for (int i=0;i<maxEnvelopes;i++){
        gainArray[i].prepare(spec);
        lowPassArray[i].prepare(spec);
        highPassArray[i].prepare(spec);
        
        lowPassArray[i].setMode(dsp::LadderFilterMode::LPF12);
        highPassArray[i].setMode(dsp::LadderFilterMode::HPF12);
        
        //gainArray[i].setGainLinear(1);
        
        //below smooths out the clicks
        gainArray[i].setRampDurationSeconds(0.05);

        waveOscArray[i]->prepare(spec);
    }

    //prob shouldnt live here
    sampleRate = spec.sampleRate;
}


void Engine::process(const dsp::ProcessContextReplacing<float>& context){

}

    
void Engine::process(const dsp::ProcessContextReplacing<float>& context, juce::MidiBuffer& midiMessages){

    //convert each lfo waverform into volume/eq modulation
    for (int i=0; i< maxEnvelopes; i++){
        
        auto lfoOut = waveOscArray[i]->getSample(m_lfo);    // [5]
        
        //sanitize lfo out
        //TODO: understand if this is ever hit, and why?
        //would get regular occurences of super loud value, why?
        //is it a rate problem since it happens regularly?
        if (lfoOut > 1)
            lfoOut = 1.0;
        if (lfoOut < 0)
            lfoOut = 0.0;
        
        lfoVolumeArray[i] = juce::jmap (lfoOut, 0.0f, 1.0f, userVolumeArray[i], 1.0f);
        lfoEQArray[i] = juce::jmap (lfoOut, 0.0f, 1.0f, userEQMinArray[i], userEQMaxArray[i]);
        
        highPassArray[i].setCutoffFrequencyHz(userEQMinArray[i]);
        lowPassArray[i].setCutoffFrequencyHz(lfoEQArray[i]);
        
        if (volumeToggleArray[i]){
            //gainArray[i].setGainLinear(1.0f);
            gainArray[i].setGainLinear(lfoVolumeArray[i]);
        }
        else
            gainArray[i].setGainLinear(1.0f);
    }

    
    
    
    for (int i=0; i< maxEnvelopes; i++){
        if (globalToggleArray[i]){
            //m_rate = rateArray[i];
            // This will activate when the toggle changes to reset the envelope
            if (toggleActivated[i]) {
                //resetEnvelope(i);
                //toggleActivated[i] = false;
            }
            
            highPassArray[i].process(context);
            lowPassArray[i].process(context);
            gainArray[i].process(context);
            break;
        }
    }
    
    //old code for processing midi
    //logic now changed to parameter buttons that user can toggle
    
    /*
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        
        
        if (message.isNoteOn())
        {
            juce::Logger::outputDebugString("Received C note");
            //60 correpsonds to C3 note
            if (message.getNoteNumber() == 60){
                //lfoValuePtr = &lfoVolumeArray[0];
            }
            if (message.getNoteNumber() == 62){
                // keyPressed(1, true);

                //lfoValuePtr = &lfoVolumeArray[1];
            }
        }
        else if (message.isNoteOff())
        {
            if (message.getNoteNumber() == 60){
                // keyPressed(0, false);
            }
            if (message.getNoteNumber() == 62){
                // keyPressed(1, false);
            }
            //lfoValuePtr = &lfoValue;
        }
    }
     */

}


/*
void Engine::keyPressed(int index, bool pressed){
    if (volumeToggle1 && pressed){
        lfoValuePtr = &lfoVolumeArray[index];
    }
    else
        lfoValuePtr = &lfoValue;
    
    if (EQToggle1 && pressed)
    {
        lowPass1.setEnabled(pressed);
        highPass1.setEnabled(pressed);
    }
    else {
        lowPass1.setEnabled(false);
        highPass1.setEnabled(false);
    }

}
*/


void Engine::resetEnvelope(int envelopeIndex) {
    envEds[envelopeIndex]->resetEnv();
}


void Engine::reset()
{
}


void Engine::parameterChanged(const String& parameterID, float newValue )
{
    
    for (int i = 0;i<maxEnvelopes; i++){
        
        if (parameterID == Parameters::volumeStringArray[i]) {
            userVolumeArray[i] = 1.0f - newValue;
        }
        if (parameterID == Parameters::rateStringArray[i]) {
            updateRate(newValue, i);
            if(globalToggleArray[i]){
                m_rate = rateArray[i];
            }
        }
        if (parameterID == Parameters::EQMinStringArray[i]) {
            userEQMinArray[i] = newValue;
        }
        if (parameterID == Parameters::EQMaxStringArray[i]) {
            userEQMaxArray[i] = newValue;
        }
        if(parameterID == Parameters::volumeToggleStringArray[i]){
            volumeToggleArray[i] = (bool) newValue;

        }
        if(parameterID == Parameters::EQToggleStringArray[i]){
            EQToggleArray[i] = (bool) newValue;
            lowPassArray[i].setEnabled(EQToggleArray[i]);
            highPassArray[i].setEnabled(EQToggleArray[i]);

        }
        
        if(parameterID == Parameters::globalToggleStringArray[i]){
            globalToggleArray[i] = (bool) newValue;
            
            if (newValue){
                m_rate = rateArray[i];
                updateGlobal(i);
            }
            
            // TODO: Causes envelop reset
            toggleActivated[i] = (newValue == 1.0f);
        }
    }
    
}

void Engine::updateRate(float value, int index){
    if (0 <= value && value < 0.125){
        rateArray[index] = 0.25;
    }
    else if (0.125 <= value && value < 0.375){
        rateArray[index] = 0.5;
    }
    else if (0.375 <= value && value < 0.625){
        rateArray[index] = 1;
    }
    else if (0.625 <= value && value < 0.875){
        rateArray[index] = 2;
    }
    else if (0.875 <= value && value <= 1.0){
        rateArray[index] = 4;
    }
    else
        rateArray[index] = 1;
}

void Engine::updateGlobal(int index){
    
}


void Engine::updateParameters()
{
    //example line below
    //dryWetMix.setDryDecibels(*parameters.getRawParameterValue(Params::ID_DRY_GAIN));
}
    
//when user changes waveform this is called and will update underlying data structure
void Engine::changeListenerCallback(ChangeBroadcaster* source)
{
    int i=0;
    for (auto&& envEd : envEds ){
        if (source == &(*envEd)){
            setMSEGTable(*envEd, msegTableArray[i]);
        }
        i++;
    }
}

void Engine::setMSEGTable(EnvelopeEditor& envEd, AudioSampleBuffer& buffer)
{
    buffer.setSize(1, int(tableSize));
    auto* samples = buffer.getWritePointer (0);

    env.reset(&envEd.envDesc);
    float fy;
    //do we need to do below?
    bool endOfEnvelope = env.getSample(fy);
    samples[0] = fy;
    
    for (int i = 1; !endOfEnvelope && i < tableSize; i++)
    {
        endOfEnvelope = env.getSample(fy);
        auto sample = fy;
        //should be float between 0 and 1
        samples[i] = (float) sample;
    }
}


