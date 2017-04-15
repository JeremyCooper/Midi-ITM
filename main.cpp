//#define d_midi
/*
 "Routes midi input to Lisp, reveals 
  function for midi output to Lisp"
*/
#include <iostream>
#include <unistd.h>
//#include <cstdlib>
#include <signal.h>
#include <vector>
#include <ecl/ecl.h>
#include "RtMidi.h"

//#define d_midi

std::string midiInName = "APC40 mkII 20:0";
std::string midiOutName = "";
std::string feedbackName = "APC40 mkII 20:0";
std::vector<std::string> lisp_files = {
	"midi-itm.lsp",
	"output-bindings.lsp",
	"colors.lsp",
	"declarations.lsp"
};

// A macro to create a DEFUN abstraction in C++
#define DEFUN(name, fun, args) \
	cl_def_c_function(c_string_to_object(name), \
	(cl_objectfn_fixed)fun, \
	args)
// Define a function to run arbitrary Lisp expresssions
cl_object lisp(const std::string& call) {
	return cl_safe_eval(c_string_to_object(call.c_str()), Cnil, Cnil);
}

void send_midi(cl_object, cl_object, cl_object);
void send_feedback(cl_object, cl_object, cl_object);
void initialize(int argc, char **argv)
{
	cl_boot(argc, argv);
	atexit(cl_shutdown);

	// Run Lisp code
//	for (unsigned int i=0; i!=lisp_files.size(); ++i)
//		lisp("(load \"" + lisp_files[i] + "\")");
	lisp("(load \"midi-itm.lsp\")");
	lisp("(load \"output-bindings.lsp\")");
	lisp("(load \"declarations.lsp\")");

	// Make C++ functions available to Lisp
	DEFUN("send_midi", send_midi, 3);
	DEFUN("send_feedback", send_feedback, 3);
}
int z;
std::string lisp_call;
void to_lisp(int midi_data[3])
{
	lisp_call = "(route ";// += channel += " " += note += " " += value += ")";
	for (z=0; z!=3; ++z)
	{
		lisp_call += std::to_string(midi_data[z]) + " ";
	}
	lisp_call += ")";
#ifdef d_midi
	//std::cout << lisp_call << std::endl;
#endif
	lisp(lisp_call);
}

bool done;
static void finish(int ignore) { done = true; }
RtMidiOut *midiout;
RtMidiOut *feedback;
int main(int argc, char* argv[])
{
	// Bootstrap Lisp
	initialize(argc, argv);

	// Initialize RtMidi
	RtMidiIn *midiin = new RtMidiIn();
	midiout = new RtMidiOut();
	feedback = new RtMidiOut();
	std::vector<unsigned char> message;
	int nBytes, i;
	int midi_data[3];

	for (unsigned int i=0; i!=midiin->getPortCount(); ++i)
	{
#ifdef d_midi
		std::cout << "In port: " << midiin->getPortName(i) << std::endl;
#endif
		if (midiin->getPortName(i) == midiInName)
			midiin->openPort(i);
	}
/*	for (unsigned int i=0; i!=midiout->getPortCount(); ++i)
	{
#ifdef d_midi
		std::cout << "Out port: " << midiout->getPortName(i) << std::endl;
#endif
		if (midiout->getPortName(i) == midiOutName)
			midiout->openPort(i);
	}*/
	midiout->openVirtualPort("midi-itm");
	for (unsigned int i=0; i!=feedback->getPortCount(); ++i)
		if (feedback->getPortName(i) == feedbackName)
			feedback->openPort(i);

	// Send sysex message
	std::vector<unsigned char> sysex = {
		0xF0, 0x47, 0x00, 0x29, 0x60, 0x00, 0x04, 0x41/*mode byte*/, 0x08, 0x04, 0x01, 0xF7
	};
	feedback->sendMessage( &sysex );

	done = false;
	(void) signal(SIGINT, finish);

	while (!done) {
		midiin->getMessage( &message );
		nBytes = message.size();
		for (i=0; i!=nBytes; ++i)
			if (i < 4)
				midi_data[i] = (int)message[i];
		if (nBytes > 0)
		{
#ifdef d_midi
			std::cout << "IN: ";
			for (i=0; i!=2; ++i)
				std::cout << midi_data[i] << ", ";
			std::cout << midi_data[i] << std::endl;
#endif
			to_lisp(midi_data);
		}

		sleep(0.3);
	}

	delete midiin;
	delete midiout;
	return EXIT_SUCCESS;
}
#ifdef d_midi
int i;
#endif
int channel, note, value;
std::vector<unsigned char> midiout_message(3);
void send_midi(cl_object _channel, cl_object _note, cl_object _value)
{
	midiout_message[0] = fix(_channel);
	midiout_message[1] = fix(_note);
	midiout_message[2] = fix(_value);
	midiout->sendMessage( &midiout_message );
#ifdef d_midi
			std::cout << "OUT: ";
			for (i=0; i!=2; ++i)
				std::cout << (int)midiout_message[i] << ", ";
			std::cout << (int)midiout_message[i] << std::endl;
#endif
}
#ifdef d_midi
int fb_i;
#endif
int fb_channel, fb_note, fb_value;
std::vector<unsigned char> feedback_message(3);
void send_feedback(cl_object _channel, cl_object _note, cl_object _value)
{
	feedback_message[0] = fix(_channel);
	feedback_message[1] = fix(_note);
	feedback_message[2] = fix(_value);
	feedback->sendMessage( &feedback_message );
#ifdef d_midi
			std::cout << "FEEDBACK: ";
			for (fb_i=0; fb_i!=2; ++fb_i)
				std::cout << (int)feedback_message[fb_i] << ", ";
			std::cout << (int)feedback_message[fb_i] << std::endl;
#endif
}
