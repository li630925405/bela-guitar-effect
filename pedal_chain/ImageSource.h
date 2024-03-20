#pragma once
#include <queue> 
#include <vector>
#include "Tree.h"

class ImageSource {
public:
	ImageSource(int order = 0, float size = 3, std::vector<float> mic_pos = {1, 1}, std::vector<float> speaker_pos = {2, 2});
	void reverb();
	~ImageSource() {}				// Destructor
	float process(float in, int order);
	void convolve(float in, std::vector<float> IR);

private:
	int order = 0;
	float size = 0;
	std::vector<float> mic_pos = {0, 0};
	std::vector<float> speaker_pos = {0, 0};
	std::queue<Tree> sources;
	std::vector<std::vector<float>> dist;  // {distance, depth}
	std::vector<float> IR;
	std::vector<float> output_buffer;
	unsigned int gWritePointer = 0;
	unsigned int gReadPointer = 0;
};
