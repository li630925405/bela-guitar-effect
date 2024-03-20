
#pragma once

#include <vector>

class Distortion {
public:
	Distortion() {}													// Default constructor
	Distortion(float parameter); 						
	
	void setup(float parameter); 		
	
	void setFrequency(float f);	// Set the oscillator frequency
	float getFrequency();		// Get the oscillator frequency
	
	float process(float in, float value);				// Get the next sample and update the phase
	
	~Distortion() {}				// Destructor

private:
	std::vector<float> table_;	// Buffer holding the wavetable

	float inverseSampleRate_;	// 1 divided by the audio sample rate	
	float frequency_;			// Frequency of the oscillator
	float readPointer_;			// Location of the read pointer (phase of oscillator)
	bool useInterpolation_;		// Whether to use linear interpolation
};