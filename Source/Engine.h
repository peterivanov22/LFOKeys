/*
  ==============================================================================

    Saturator.h
    Created: 19 Jul 2024 12:35:52pm
    Author:  Peter Ivanov

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "EnvelopeEditor.h"
#include "WavetableOscillator.h"





class Engine : public dsp::ProcessorBase, public AudioProcessorValueTreeState::Listener, public ChangeListener
{
public:
    Engine(AudioProcessorValueTreeState & params, AudioProcessor& p, std::vector<std::unique_ptr<EnvelopeEditor>>& envEds);
   
    void prepare(const dsp::ProcessSpec& spec) override;
    
    void process(const dsp::ProcessContextReplacing<float>& context) override;
    
    void process(const dsp::ProcessContextReplacing<float>& context, juce::MidiBuffer& midiMessages);
    
    void reset() override;
    
    void parameterChanged(const juce::String& parameterID, float newValue ) override;
    
    void changeListenerCallback(ChangeBroadcaster* source) override;

    
    
    void processTempoSync(AudioPlayHead::PositionInfo& hostInfo)
    {
        //TODO: deal with other time signatures. need to get num, denom from hostInfo
        //m_rate = 1;
        //should we  check if BPM is ever null below? probably..
        if (m_bpm!=hostInfo.getBpm()){
            m_bpm = *(hostInfo.getBpm());
            m_inc = m_bpm / (60*sampleRate);
        }
        m_ppq = *(hostInfo.getPpqPosition()) * m_rate;
        m_inc = m_bpm/(60*sampleRate);
        
        // if playHeadActive AND is playing ..
        m_ppq = m_inc*m_rate + m_ppq;
        //m_lfo is value between 0 and 1 representing where we should be on the LFO wavetable (0 is start, 1 is end)
        m_lfo = m_ppq - int(m_ppq);

        
    }
    
    void keyPressed(int index, bool pressed);
    void updateRate(float value, int index);
    void updateGlobal(int index);

    // TODO: Causes envelope reset on toggle switch
    void resetEnvelope(int envelopeIndex);
    
private:
    
    //we probably dont want to pass the entire Envelop Editor
    //but i guess we need to to attach listener when user changes the envelop (mouse click)
    //TODO: better way to do this?
    MultiSegmentEnvelopeGenerator env;

    std::vector<std::unique_ptr<EnvelopeEditor>>& envEds;

    static const int maxEnvelopes = 5;
    
    // TODO: causes envelope reset on toggle switch
    bool toggleActivated[maxEnvelopes] = { false };
    

    AudioProcessorValueTreeState& apvts;
    AudioProcessor& processor;


    
    const unsigned int tableSize = EnvelopeEditor::MSEG_DEFAULT_PIXELS_WIDTH;      // [2]
    float level = 0.0f;
 
    
    WavetableOscillator * waveOscArray[maxEnvelopes];

    dsp::Gain<float> gainArray [maxEnvelopes];
    dsp::LadderFilter<float> lowPassArray [maxEnvelopes];
    dsp::LadderFilter<float> highPassArray [maxEnvelopes];


    
    //bool notePressed = false;

    void updateParameters();
    
    AudioSampleBuffer msegTable;
    std::vector<AudioSampleBuffer> msegTableVector;
    AudioSampleBuffer msegTableArray[maxEnvelopes];
    
    float lfoVolumeArray [maxEnvelopes];
    float lfoEQArray [maxEnvelopes];

    float userVolumeArray [maxEnvelopes];
    float userEQMaxArray [maxEnvelopes];
    float userEQMinArray [maxEnvelopes];
    //volume and eq toggles
    std::atomic<bool> volumeToggleArray [maxEnvelopes];
    std::atomic<bool> EQToggleArray [maxEnvelopes];
    std::atomic<bool> globalToggleArray [maxEnvelopes];
    //do we need the following? current just need it for tempoSymc
    float sampleRate, defaultVolumeValue=1;
    float * lfoValuePtrArray [maxEnvelopes];
    
    //tempoSync variables after this
    //should m_rate be atomic? its updated by user, used during audio streaming
    std::atomic<double> m_bpm, m_inc, m_rate, m_ppq, m_lfo;
    
    
    double rateArray[maxEnvelopes];
    
    void setMSEGTable(EnvelopeEditor& envEd, AudioSampleBuffer& buffer);

};
 
