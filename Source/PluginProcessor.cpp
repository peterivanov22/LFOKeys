/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WubDaddyAudioProcessor::WubDaddyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
, apvts(*this, nullptr, "APVTS", Parameters::createParameterLayout())
, envEd(), envEd2()
, engine(apvts, *this, envEds)
//interesting note: I can pass this class (*this) as AudioProcessor to engine, but things break if I pass it as a WubDaddyAudioProcessor to engine. 
//(my  guess is we can't pass it as two different objects in initializer, since its being passed as audioProcessor to apvts (*this)). 
//But we are passing it as a WubDaddyAudioProcessor to the Plugin Editor, in the editor constructor.

//question:  can we initialize the unique_ptr vector envEds in the intializer? Or in the header file? It seems problematic so I'm essentially initializing it in the engine file
{
    //createEditorLayout();
    midiHandler.startListening();
}

WubDaddyAudioProcessor::~WubDaddyAudioProcessor()
{
    midiHandler.stopListening();
}

//==============================================================================
const juce::String WubDaddyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WubDaddyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
#return false;
   #endif
}

bool WubDaddyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WubDaddyAudioProcessor::isMidiEffect() const
{
   // if JucePlugin_IsMidiEffect
    return true;
    // return false;
}

double WubDaddyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WubDaddyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WubDaddyAudioProcessor::getCurrentProgram()
{
    return 0;
}

//TODO: I guess this is for setting the preset? Not sure
void WubDaddyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WubDaddyAudioProcessor::getProgramName (int index)
{
    return {};
}

void WubDaddyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WubDaddyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    auto totalNumInputChannels  = getTotalNumInputChannels();
    
    engine.prepare({ sampleRate, (uint32) samplesPerBlock, (uint32) totalNumInputChannels });
    
    //Reports latency to the DAW
    setLatencySamples(0);

}

void WubDaddyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WubDaddyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WubDaddyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //does something with floats to reduce CPU time, IDK
    ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    playHead = this->getPlayHead();
    
    if (playHead != nullptr) {
        hostInfo = *(playHead->getPosition());
        engine.processTempoSync(hostInfo);
    }
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

        
    //dsp library called below
    auto block = juce::dsp::AudioBlock<float> (buffer);
    auto context = juce::dsp::ProcessContextReplacing<float> (block);
    //engine.process(context);
    engine.process(context, midiMessages);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
     */
}

//==============================================================================
bool WubDaddyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WubDaddyAudioProcessor::createEditor()
{
    return new WubDaddyAudioProcessorEditor (*this);
}

juce::ValueTree createSegmentValueTree(MultiSegmentEnvelopeGenerator::SegmentDescriptor & segment){
   
    ValueTree tree ("Segment");

    tree.setProperty ("initialValue", segment.initialValue, nullptr);
    tree.setProperty ("finalValue", segment.finalValue, nullptr);
    tree.setProperty ("curvature", segment.curvature, nullptr);
    tree.setProperty ("lengthSamples", segment.lengthSamples, nullptr);

    return tree;
}

juce::ValueTree createEnvelopeValueTree(EnvelopeEditor & envelope, int i){
    
    ValueTree tree ("Envelope" + String(i));
    
    for (int j =0; j< envelope.envDesc.size(); j++){
        tree.appendChild(createSegmentValueTree(envelope.envDesc[j]), nullptr);
    }
    return tree;
}




//==============================================================================
void WubDaddyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    


    
    for (int i=0; i<envEds.size();i++){
        if (apvts.state.getChildWithName("Envelope" + String(i)).isValid())
            apvts.state.removeChild(apvts.state.getChildWithName("Envelope" + String(i)), nullptr);

        
        apvts.state.appendChild(createEnvelopeValueTree(*envEds[i], i), nullptr);
    }
    
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
    
    

    
    
}

void WubDaddyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr){
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
        
        for (int i =0; i< envEds.size(); i++){
            juce::XmlElement* envelopeXml (xmlState->getChildByName ("Envelope" + String(i)));
            if (envelopeXml != nullptr)
            {
                envEds[i]->envDesc.clear();

                for (int j = 0; j < envelopeXml->getNumChildElements(); j++){
                    juce::XmlElement* segmentXML (envelopeXml->getChildElement(j));
                    envEds[i]->envDesc.push_back
                    ({(float) segmentXML->getDoubleAttribute("initialValue"),(float)  segmentXML->getDoubleAttribute("finalValue"),(float) segmentXML->getDoubleAttribute("curvature"),segmentXML->getIntAttribute("lengthSamples") }
                     );

                }
                
                


            }
        }


        
    }

    
    
    
    //envEds[0]->envDesc.clear();
    //envEds[0]->envDesc.push_back({ 1.0f, 1.0f, 0.0f,    EnvelopeEditor::MSEG_DEFAULT_PIXELS_WIDTH });

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WubDaddyAudioProcessor();
}
