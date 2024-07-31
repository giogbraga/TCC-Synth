/*
  ==============================================================================

    FilterData.h
    Created: 19 May 2024 11:22:07am
    Author:  gigio

  ==============================================================================

*/

#pragma once
#include <JuceHeader.h>

class FilterData
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const float modulator, const int filterType, const float frequency, const float resonance);
    void reset();

private:
    void resetFilter();
    void setupProcessSpec(double sampleRate, int samplesPerBlock, int numChannels);
    void setFilterType(int filterType);
    void setFilterCutoffFrequency(float modulator, float frequency);

    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared{ false };
};
