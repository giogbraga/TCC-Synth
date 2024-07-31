/*
  ==============================================================================

    SynthSound.h
    Created: 26 Oct 2023 11:09:06am
    Author:  gigio

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }

};