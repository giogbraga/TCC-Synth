/*
  ==============================================================================

    SynthVoice.h
    Created: 26 Oct 2023 11:08:21am
    Author:  gigio

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/AdsrData.h"
#include "Data/FilterData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void updateFilter(const int filterType, const float frequency, const float resonance);

    OscData& getOscillator() { return osc; }
    AdsrData& getAdsr() { return adsr; }
    AdsrData& getFilterAdsr() { return filterAdsr; }
    FilterData& getFilter() { return filter; }

private:
    void setNoteFrequency(int midiNoteNumber);
    void startADSR();
    void stopADSR();
    bool shouldClearCurrentNoteTailOff(bool allowTailOff);
    void prepareProcessSpec(double sampleRate, int samplesPerBlock, int outputChannels);
    void initializeComponents(double sampleRate, int samplesPerBlock, int outputChannels);
    void prepareSynthBuffer(juce::AudioBuffer<float>& outputBuffer, int numSamples);
    void processAudioBlock(int numSamples);
    void mixOutputBuffer(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples);

    juce::AudioBuffer<float> synthBuffer;

    OscData osc;
    AdsrData adsr;
    AdsrData filterAdsr;
    FilterData filter;
    juce::dsp::Gain<float> gain;

    bool isPrepared{ false };
};
