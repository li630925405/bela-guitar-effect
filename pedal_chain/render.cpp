#include <Bela.h>
#include <cmath>
#include <libraries/Gui/Gui.h>
#include <libraries/GuiController/GuiController.h>
#include <libraries/Scope/Scope.h>
#include "MonoFilePlayer.h"
#include "Distortion.h"
#include "Reverberation.h"
#include "isr_simple.h"

	
// Name of the sound file (in project folder)
std::string gFilename = "clean_guitar.wav";

// Object that handles playing sound from a buffer
MonoFilePlayer gPlayer;
Distortion distort;
Reverberation reverb;
isr_simple isr;

const int kButtonPin1 = 0;		
const int kButtonPin2 = 1;
bool mode = 0;  // 0 for play .wav, 1 for guitar input
bool breadboard = 1;  // 0 for control from breadboard, 1 for control from GUI

// Last state of the button
int gLastButtonStatus1 = HIGH;
int gLastButtonStatus2 = HIGH;

bool apply_distort = false;
unsigned int apply_reverb = 0;

// Browser-based GUI to adjust parameters
Gui gGui;
GuiController gGuiController;
Scope gScope;

float distort_value = 0;
float reverb_value = 0;

bool setup(BelaContext *context, void *userData)
{
	// Load the audio file
	if(!gPlayer.setup(gFilename)) {
    	rt_printf("Error loading audio file '%s'\n", gFilename.c_str());
    	return false;
	}
    rt_printf("Loaded the audio file '%s' with %d frames (%.1f seconds)\n", 
    			gFilename.c_str(), gPlayer.size(),
    			gPlayer.size() / context->audioSampleRate);

	/* digital buttons */
	pinMode(context, 0, kButtonPin1, INPUT);
	pinMode(context, 0, kButtonPin2, INPUT);
	
	gGui.setup(context->projectName);
	gGuiController.setup(&gGui, "Controls");  
	gGuiController.addSlider("reverb", 0, 0, 2, 1);
	gGuiController.addSlider("reverb amount", 0, 0, 1, 0);
	gGuiController.addSlider("distortion", 0, 0, 1, 1);
	gGuiController.addSlider("distortion amount", 0, 0, 1, 0);
	
	return true;
}

void render(BelaContext *context, void *userData)
{
	for(unsigned int n = 0; n < context->audioFrames; n++) {
		// float in = audioRead(context, n, 0);
		// float right = audioRead(context, n, 1);
		// audioWrite(context, n, 0, left);
		// audioWrite(context, n, 1, left);
		float in = 0;
		
		if (mode == 0) {
			in = gPlayer.process();
		} else {
			in = audioRead(context, n, 0) + audioRead(context, n, 1);
		}
		
		float out = in;
		
		if (breadboard == 0) {
			if ((n % 2) == 0)  {
				distort_value = analogRead(context, n/2, 0);
		    	reverb_value = analogRead(context, n/2, 1);
			}
		} else {
		    apply_reverb = gGuiController.getSliderValue(0); 
		    reverb_value = gGuiController.getSliderValue(1); 
		    apply_distort = gGuiController.getSliderValue(2); 
		    distort_value = gGuiController.getSliderValue(3); 
		}
		
		if (apply_distort) {
			// distort_value = map(distort_value, 0, 1, 0, 1000);
			out = distort.process(out, distort_value); 
		}
		// spring reverb
		if (apply_reverb == 1) {
			out = reverb.process(out, reverb_value);
		}
		// image source reverb 
		else if (apply_reverb == 2) {
			float size = map(reverb_value, 0, 1, 5, 100);
			out = isr.process(out, size);
		}
		
		if (breadboard == 0) {
			/* detect button press */
			int status1 = digitalRead(context, n, kButtonPin1);
			int status2 = digitalRead(context, n, kButtonPin2);
			if (status1 == LOW && gLastButtonStatus1 == HIGH) {
				apply_distort = !apply_distort;
			}
			if (status2 == LOW && gLastButtonStatus2 == HIGH) {
				if (apply_reverb == 1) {
					reverb.reset();
				} else if (apply_reverb == 2) {
					isr.reset();
				} 
				apply_reverb = (apply_reverb + 1) % 3;
			}
			
			gLastButtonStatus1 = status1;
			gLastButtonStatus2 = status2;
		}
		
		for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			audioWrite(context, n, channel, out);
		}
    	
    	// gScope.log(in, out);
    }
}

void cleanup(BelaContext *context, void *userData)
{

}