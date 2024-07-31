/*
  ==============================================================================

    SynthVoice.cpp
    Created: 26 Oct 2023 11:08:21am
    Author:  gigio

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    setNoteFrequency(midiNoteNumber);
    startADSR();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    stopADSR();

    if (shouldClearCurrentNoteTailOff(allowTailOff))
        clearCurrentNote();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    // Implement controller functionality if needed
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    // Implement pitch wheel functionality if needed
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    prepareProcessSpec(sampleRate, samplesPerBlock, outputChannels);
    initializeComponents(sampleRate, samplesPerBlock, outputChannels);
    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
        return;

    prepareSynthBuffer(outputBuffer, numSamples);
    processAudioBlock(numSamples);
    mixOutputBuffer(outputBuffer, startSample, numSamples);

    if (!adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::updateFilter(const int filterType, const float frequency, const float resonance)
{
    auto modulator = filterAdsr.getNextSample();
    filter.updateParameters(modulator, filterType, frequency, resonance);
}

void SynthVoice::setNoteFrequency(int midiNoteNumber)
{
    osc.setWaveFrequency(midiNoteNumber);
}

void SynthVoice::startADSR()
{
    adsr.noteOn();
    filterAdsr.noteOn();
}

void SynthVoice::stopADSR()
{
    adsr.noteOff();
    filterAdsr.noteOff();
}

bool SynthVoice::shouldClearCurrentNoteTailOff(bool allowTailOff)
{
    return !allowTailOff || !adsr.isActive();
}

void SynthVoice::prepareProcessSpec(double sampleRate, int samplesPerBlock, int outputChannels)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc.prepareToPlay(spec);
    gain.prepare(spec);
    gain.setGainLinear(0.3f);
}

void SynthVoice::initializeComponents(double sampleRate, int samplesPerBlock, int outputChannels)
{
    filterAdsr.setSampleRate(sampleRate);
    filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
    adsr.setSampleRate(sampleRate);
}

void SynthVoice::prepareSynthBuffer(juce::AudioBuffer<float>& outputBuffer, int numSamples)
{
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
}

void SynthVoice::processAudioBlock(int numSamples)
{
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    osc.getNextAudioBlock(audioBlock);
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    filter.process(synthBuffer);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    filterAdsr.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);
}

void SynthVoice::mixOutputBuffer(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
    }
}
