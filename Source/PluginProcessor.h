/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Data/FilterData.h"

//==============================================================================
/**
*/
class GigimpSynthAudioProcessor  : public foleys::MagicProcessor
{
public:
    //==============================================================================
    GigimpSynthAudioProcessor();
    ~GigimpSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::Synthesiser synth;

    juce::AudioProcessorValueTreeState::ParameterLayout createParams();

    // GUI MAGIC: define that as last member of your AudioProcessor
    foleys::MagicLevelSource* outputMeter = nullptr;
    foleys::MagicPlotSource* oscilloscope = nullptr;
    foleys::MagicPlotSource* analyser = nullptr;


    FilterData filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GigimpSynthAudioProcessor)
};
