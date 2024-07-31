/*
  ==============================================================================

    OscData.cpp
    Created: 29 Nov 2023 9:36:06am
    Author:  gigio

  ==============================================================================

*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    prepare(spec);
    fmOsc.prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    initialiseWave(choice);
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    float frequency = calculateFrequency(midiNoteNumber);
    setFrequency(frequency);
    lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    processFmOsc(block);
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::processFmOsc(juce::dsp::AudioBlock<float>& block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }
}

void OscData::updateFm(const float freq, const float depth)
{
    fmOsc.setFrequency(freq);
    fmDepth = depth;
    updateCurrentFrequency();
}

void OscData::initialiseWave(const int choice)
{
    switch (choice)
    {
    case 0:
        // Sine
        initialise([](float x) { return std::sin(x); });
        break;

    case 1:
        // Saw wave
        initialise([](float x) { return x / juce::MathConstants<float>::pi; });
        break;

    case 2:
        // Square wave
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;

    default:
        jassertfalse;   // You're not supposed to be here!
        break;
    }
}

float OscData::calculateFrequency(const int midiNoteNumber) const
{
    return juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod;
}

void OscData::updateCurrentFrequency()
{
    auto currentFreq = calculateFrequency(lastMidiNote);
    setFrequency(currentFreq >= 0 ? currentFreq : -currentFreq);
}
