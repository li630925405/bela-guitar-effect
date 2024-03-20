#include <Bela.h>
#include <cmath>
#include <libraries/AudioFile/AudioFile.h>
#include "Tree.h"
#include "ImageSource.h"


ImageSource::ImageSource(int order, float size, std::vector<float> mic_pos, std::vector<float> speaker_pos) {
	this->order = order;
	this->size = size;
	this->mic_pos = mic_pos;
	this->speaker_pos = speaker_pos;
	this->sources.push(Tree(speaker_pos));
	IR.resize(20);
	output_buffer.resize(20);
}

bool equal(std::vector<float> d1, std::vector<float> d2) {
	return ((d1[0] == d2[0]) & (d1[1] == d2[1]));
}

void ImageSource::reverb() {
	int i = 0;
	while (i < pow(4, order)) {
	    Tree source = sources.front();
	    sources.pop();
	    float d = sqrt(pow((source.data[0] - mic_pos[0]), 2) + pow((source.data[1] - mic_pos[1]), 2));
	    dist.push_back({d, source.depth});
	    std::vector<float> s1 = {-source.data[0], source.data[1]};
	    std::vector<float> s2 = {size - source.data[0] + size, source.data[1]};
	    std::vector<float> s3 = {source.data[0], -source.data[1]};
	    std::vector<float> s4 = {source.data[0], size - source.data[1] + size};
	    if (source.parent) {
	    	if (!equal(source.parent->data, s1)) {
          source.children.push_back(Tree(s1, &source, source.depth+1));
        }
        if (!equal(source.parent->data, s2)) {
          source.children.push_back(Tree(s2, &source, source.depth+1));
        }
        if (!equal(source.parent->data, s3)) {
          source.children.push_back(Tree(s3, &source, source.depth+1));
        }
        if (!equal(source.parent->data, s4)) {
          source.children.push_back(Tree(s4, &source, source.depth+1));  
        }
      }
      else {
        source.children.push_back(Tree(s1, &source, source.depth+1));
        source.children.push_back(Tree(s2, &source, source.depth+1));
        source.children.push_back(Tree(s3, &source, source.depth+1));
        source.children.push_back(Tree(s4, &source, source.depth+1));
      }
		for (auto node : source.children) {
	        sources.push(node);
		}
	    i += 4;    
	}

	int c = 340;
	int sr = 22050;
	
	for (std::vector<float> d_set : dist) {
		float d = d_set[0];
		int index = int(d * sr / c);
		if (index < IR.size()) {
			IR[index] += 1 / d;
		}
	}
}

void ImageSource::convolve(float in, std::vector<float> IR) {
	std::vector<float> buffer;
	buffer.resize(output_buffer.size());
	for (int n = 0; n < IR.size(); n++) {
		for (int i = 0; i < n; i++) {
			buffer[n] +=  in * IR[n - i];
		}
	}
	for (int i = 0; i < buffer.size(); i++) {
		int index = (gWritePointer + i) % output_buffer.size();
		output_buffer[index] += buffer[i];
	}
	gWritePointer = (gWritePointer + 1) % output_buffer.size();
}

float ImageSource::process(float in, int order) {
	this->order = order;
	reverb();
	convolve(in, IR);
	gReadPointer = (gReadPointer + 1) % output_buffer.size();
	float out = output_buffer[gReadPointer];
	return out;
}