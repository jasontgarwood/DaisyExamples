#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

//init hardware
DaisyPod hw;
static Switch button1;
bool button1On = false;
static GPIO LED1;


//declare effects you'd like to use
Flanger effect1;

//declare effect variables you'd like to change
float effect1Freq;


////////// Sets audio input to output and applies effects when button is active) //////////
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();
	// Analog to digital conversion to read the potentiomenters on a scale of 0-1
	effect1Freq = hw.knob1.Process(); 

	for (size_t i = 0; i < size; i++)
	{
		float output;
		//adjust volume by multiplying with a gain factor
		const float gain = 15.0f;
		output = in[0][i]*gain;

		out[0][i] = effect1.Process(output);
	}

}



////////// Main Function that initializes Daisy and effects //////////
int main(void)
{
	//hardware Initialization 
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAdc();
	hw.StartAudio(AudioCallback);
	LED1.Init(D21, GPIO::Mode::OUTPUT);
    button1.Init(D26, 1000);

	//initialize effects and set parameters
	effect1.Init(hw.AudioSampleRate());
	effect1.SetLfoFreq(0.33f);
	effect1.SetLfoDepth(0.5f);
	effect1.SetFeedback(0.83f);

	while(1) {
		button1.Debounce();
    	button1On = button1.Pressed();
		LED1.Write(button1On);
	}
}
