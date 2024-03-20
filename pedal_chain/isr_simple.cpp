#include <Bela.h>
#include <cmath>
#include <libraries/AudioFile/AudioFile.h>
#include "isr_simple.h"

isr_simple::isr_simple(float size, std::vector<float> mic_pos, std::vector<float> speaker_pos) {
	this->size = size;
	float step = size / 5;
	this->mic_pos[0] = step;
	this->mic_pos[1] = step;
	this->speaker_pos[0] = step * 3;
	this->speaker_pos[1] = step * 5;
	output_buffer.resize(44100);
}

void isr_simple::reset() {
	std::fill(output_buffer.begin(), output_buffer.end(), 0);
	gWritePointer = 0;
	gReadPointer = 0;
}

float isr_simple::process(float in, float size) {
	float out = 0;
	float x = speaker_pos[0];
	float y = speaker_pos[1];
	float d = sqrt(pow((x - mic_pos[0]), 2) + pow((y - mic_pos[1]), 2));
	int sample = d * 44100 / 340;
	gReadPointer = (gWritePointer - sample + output_buffer.size()) % output_buffer.size();
	out += (output_buffer[gReadPointer] / d);
	float d1 = sqrt(pow((-x - mic_pos[0]), 2) + pow((y - mic_pos[1]), 2));
	float d2 = sqrt(pow((size - x + size - mic_pos[0]), 2) + pow((y - mic_pos[1]), 2));
	float d3 = sqrt(pow((x - mic_pos[0]), 2) + pow((-y - mic_pos[1]), 2));
	float d4 = sqrt(pow((x - mic_pos[0]), 2) + pow((size - y + size - mic_pos[1]), 2));
	std::vector<float> distance = {d1, d2, d3, d4};
	for (int i = 0; i < 4; i++) {
		int s = distance[i] * 44100 / 340;
		int p = (gWritePointer - s + output_buffer.size()) % output_buffer.size();
		// out += (output_buffer[p] / distance[i]);
		if (d > 10) {
			d = distance[i] / 10;
		}
		out += output_buffer[p] / d;
	}

	output_buffer[gWritePointer] = in;
	gWritePointer++;
	if (gWritePointer >= output_buffer.size()) {
	    gWritePointer = 0;
	}
	
	return out * 0.5 + in;
}