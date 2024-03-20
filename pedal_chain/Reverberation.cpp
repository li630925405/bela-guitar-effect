/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

http://bela.io

C++ Real-Time Audio Programming with Bela - Lecture 8: Filters
*/
#include <Bela.h>
#include <cmath>
#include <libraries/AudioFile/AudioFile.h>
#include "Reverberation.h"


Reverberation::Reverberation() {
	gDelayBuffer.resize(0.5 * 44100);
	gDelayBuffer2.resize(0.5 * 44100);
}

void Reverberation::reset() {
	std::fill(gDelayBuffer.begin(), gDelayBuffer.end(), 0);
	std::fill(gDelayBuffer2.begin(), gDelayBuffer2.end(), 0);
	gWritePointer = 0;
	gReadPointer = 0;
	gWritePointer2 = 0;
	gReadPointer2 = 0;
}

// Return the next sample of the loaded audio file
float Reverberation::process(float in, float value)
{
	int sample = map(value, 0, 1, 1, 0.5 * 44100);
	// int sample = 1;
	float attenuate = 0.5;
	/* read from of first spring */
	gReadPointer = (gWritePointer - sample + gDelayBuffer.size()) % gDelayBuffer.size();
	float out = gDelayBuffer[gReadPointer] * attenuate;
	/* write to the second spring */
	gWritePointer2 = gWritePointer;
	gDelayBuffer2[gWritePointer2] = out;
	/* read from the second spring */
	gReadPointer2 = (gWritePointer2 - sample + gDelayBuffer2.size()) % gDelayBuffer2.size();
	float in2 = gDelayBuffer2[gReadPointer2] * attenuate;
	/* write to the first spring */
	gDelayBuffer[gWritePointer] = in + in2;
	gWritePointer++;
	if (gWritePointer >= gDelayBuffer.size()) {
		gWritePointer = 0;
	}
	return out * 0.5 + in;
}
	