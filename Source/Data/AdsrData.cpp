/*
  ==============================================================================

    AdsrData.cpp
    Created: 28 Nov 2023 9:45:11am
    Author:  gigio

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrData.h"

void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    setADSRParams(attack, decay, sustain, release);
    applyADSRParams();
}

void AdsrData::setADSRParams(const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
}

void AdsrData::applyADSRParams()
{
    setParameters(adsrParams);
}
