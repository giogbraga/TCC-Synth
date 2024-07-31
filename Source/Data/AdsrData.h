/*
  ==============================================================================

    AdsrData.h
    Created: 28 Nov 2023 9:45:11am
    Author:  gigio

  ==============================================================================

*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrData : public juce::ADSR
{
public:
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    void setADSRParams(const float attack, const float decay, const float sustain, const float release);
    void applyADSRParams();

    juce::ADSR::Parameters adsrParams;
};
