/*
  ==============================================================================

    FilterData.cpp
    Created: 19 May 2024 11:22:07am
    Author:  gigio

  ==============================================================================

*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    resetFilter();
    setupProcessSpec(sampleRate, samplesPerBlock, numChannels);
    isPrepared = true;
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> block{ buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float>{ block });
}

void FilterData::updateParameters(const float modulator, const int filterType, const float frequency, const float resonance)
{
    setFilterType(filterType);
    setFilterCutoffFrequency(modulator, frequency);
    filter.setResonance(resonance);
}

void FilterData::resetFilter()
{
    filter.reset();
}

void FilterData::setupProcessSpec(double sampleRate, int samplesPerBlock, int numChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    filter.prepare(spec);
}

void FilterData::setFilterType(int filterType)
{
    switch (filterType)
    {
    case 0:
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;

    case 1:
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;

    case 2:
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    }
}

void FilterData::setFilterCutoffFrequency(float modulator, float frequency)
{
    float modulatedFreq = frequency * modulator;
    modulatedFreq = std::fmax(std::fmin(modulatedFreq, 20000.0f), 20.0f);
    filter.setCutoffFrequency(modulatedFreq);
}
