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

#define d_midi
std::string midiInName = "APC40 mkII 20:0";
std::string midiOutName = "";
std::vector<std::string> lisp_files = {
	"midi-itm.lisp",
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
void initialize(int argc, char **argv)
{
	cl_boot(argc, argv);
	atexit(cl_shutdown);

	// Run Lisp code
	for (unsigned int i=0; i!=lisp_files.size(); ++i)
		lisp("(load \"" + lisp_files[i] + "\")");

	// Make C++ functions available to Lisp
	DEFUN("send_midi", send_midi, 3);
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
	lisp(lisp_call);
}

bool done;
static void finish(int ignore) { done = true; }
RtMidiOut *midiout;
int main(int argc, char* argv[])
{
	// Bootstrap Lisp
	initialize(argc, argv);

	// Initialize RtMidi
	RtMidiIn *midiin = new RtMidiIn();
	midiout = new RtMidiOut();
	std::vector<unsigned char> message;
	int nBytes, i;
	int midi_data[3];

	for (unsigned int i=0; i!=midiin->getPortCount(); ++i)
		if (midiin->getPortName(i) == midiInName)
			midiin->openPort(i);
	for (unsigned int i=0; i!=midiout->getPortCount(); ++i)
		if (midiout->getPortName(i) == midiOutName)
			midiout->openPort(i);

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

		sleep(0.1);
	}

	delete midiin;
	delete midiout;
	return EXIT_SUCCESS;
}
#ifdef d_midi
int i;
#endif
int channel, note, value;
std::vector<unsigned char> _message(3);
void send_midi(cl_object _channel, cl_object _note, cl_object _value)
{
	_message[0] = fix(_channel);
	_message[1] = fix(_note);
	_message[2] = fix(_value);
	midiout->sendMessage( &_message );
#ifdef d_midi
			std::cout << "OUT: ";
			for (i=0; i!=2; ++i)
				std::cout << (int)_message[i] << ", ";
			std::cout << (int)_message[i] << std::endl;
#endif
}
