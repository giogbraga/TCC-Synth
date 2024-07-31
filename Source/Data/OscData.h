/*
  ==============================================================================

    OscData.h
    Created: 29 Nov 2023 9:36:06am
    Author:  gigio

  ==============================================================================

*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
    void updateFm(const float freq, const float depth);

private:
    void processFmOsc(juce::dsp::AudioBlock<float>& block);
    void initialiseWave(const int choice);
    float calculateFrequency(const int midiNoteNumber) const;
    void updateCurrentFrequency();

    juce::dsp::Oscillator<float> fmOsc{ [](float x) { return std::sin(x); } };
    float fmMod{ 0.0f };
    float fmDepth{ 0.0f };
    int lastMidiNote{ 0 };
};
