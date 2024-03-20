#pragma once
#include <queue> 
#include <vector>

class isr_simple {
public:
	isr_simple(float size = 10, std::vector<float> mic_pos = {1, 1}, std::vector<float> speaker_pos = {3, 4});
	~isr_simple() {}				// Destructor
	float process(float in, float size);
	void reset();

private:
	float size = 0;
	std::vector<float> mic_pos = {0, 0};
	std::vector<float> speaker_pos = {0, 0};
	std::vector<float> output_buffer;
	unsigned int gWritePointer = 0;
	unsigned int gReadPointer = 0;
};
