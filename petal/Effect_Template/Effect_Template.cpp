#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;

//declare effects you'd like to use
Flanger effect1;

//declare effect variables you'd like to change
float effect1Freq;


////////// Sets audio input to output and applies effects when button is active) //////////
void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	hw.ProcessAllControls();
	for (size_t i = 0; i < size; i++)
	{
		float output;
		//adjust volume by multiplying with a gain factor
		const float gain = 15.0f;
		output = in[0][i]*gain;

		out[0][i] = effect1.Process(output);
	}
}

////////// Processes Analog to Digital converter so the DS can read the potentiometers //////////

////////// Main Function that initializes Daisy and effects //////////
int main(void)
{
	//Daisy Initialization 
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAdc();
	hw.StartAudio(AudioCallback);

	//initialize effects and set parameters
	effect1.Init(hw.AudioSampleRate());
	effect1.SetLfoFreq(0.33f);
	effect1.SetLfoDepth(0.5f);
	effect1.SetFeedback(0.83f);

	while(1) {}
}
