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
#include <libraries/AudioFile/AudioFile.h>
#include <cmath>
#include "Distortion.h"

// Constructor taking the path of a file to load

// Return the next sample of the loaded audio file
float Distortion::process(float in, float value)
{
	float distort_value = map(value, 0, 1, 0, 1000);
	float out = ((in > 0) - (in < 0)) * (1 - exp(-1 * fabs(in * distort_value))) / 100;
    out = (1 - value) * in + value * out;
    return out;
}
	