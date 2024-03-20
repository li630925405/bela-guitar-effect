
#pragma once

#include <vector>

class Reverberation {
public:
	Reverberation();												// Default constructor
	
	void setFrequency(float f);	// Set the oscillator frequency
	float getFrequency();		// Get the oscillator frequency
	
	float process(float in, float value);				// Get the next sample and update the phase
	
	~Reverberation() {}				// Destructor
	void reset();

private:
	std::vector<float> gDelayBuffer;	// Buffer holding the Spring 1
	std::vector<float> gDelayBuffer2;	// Buffer holding the Spring 2
	unsigned int gWritePointer = 0;
	unsigned int gReadPointer = 0;
	unsigned int gWritePointer2 = 0;
	unsigned int gReadPointer2 = 0;
};