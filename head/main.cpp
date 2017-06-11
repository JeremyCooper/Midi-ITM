#include <iostream>
//#include <stdio.h>
//#include <unistd.h>
#include <vector>
#include <thread>
#include <ecl/ecl.h>
#include "RtMidi.h"
#include "cinder/app/App.h"
#include "cinder/app/AppBase.h"
#include "cinder/app/RendererGl.h"
bool done; //this belongs in singleton class?
#include "midi-itmConfig.h"
#include "graphics/Graphics.h"
//#define d_midi
using namespace ci;
using namespace ci::app;

/*
 * These belong in a config.h
 */
const std::string midiInName = "APC40 mkII";
const std::string feedbackName = "APC40 mkII";
const std::string lispDirectory = "/Users/jeremycooper/Projects/Lisp+C/midi-itm/lisp/";
const std::vector<std::string> lisp_files = {
        "midi-itm.lsp",
        "output-bindings.lsp",
        "colors.lsp",
        "declarations.lsp"
};

void midi_loop();
void initialize_lisp(int argc, char **argv);

class MidiItmApp : public App {
private:
    std::vector<gl::TextureRef> pTextures;
    Graphics *graphics = new Graphics();
public:
    void setup() {
        fprintf(stdout, "Current Task: %s\n", Task);

        graphics->setup();

        //std::thread midi_thread(&midi_loop);
        //midi_thread.detach();
    }
    void resize() {}

    void update() {
        graphics->update();
    }
    void draw() {
        gl::clear();
        //any UI here...
        graphics->draw();
        graphics->publish();
    }
    void keyDown(KeyEvent event) {
        delete graphics;
        quit();
    }
};
CINDER_APP(MidiItmApp, RendererGl(RendererGl::Options().msaa(16)), [&](App::Settings *settings) {
    settings->setWindowSize(800, 600);
    settings->setFrameRate(60.0f);
})

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
void initialize_lisp(int argc, char **argv)
{
	cl_boot(argc, argv);
	atexit(cl_shutdown);

	// Run Lisp code
	for (unsigned int i=0; i!=lisp_files.size(); ++i)
		lisp("(load \"" + lispDirectory + lisp_files[i] + "\")");

	// Make C++ functions available to Lisp
	DEFUN("send_midi", send_midi, 3);
	DEFUN("send_feedback", send_feedback, 3);
	//DEFUN("communicate", GFX_Api::communicate, 4);
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

static void finish(int) { done = true; }
RtMidiOut *midiout;
RtMidiOut *feedback;
//TODO: End midi_thread gracefully
void midi_loop() {
    int argc = 0;
    char* s = NULL;
    char**argv = &s;
    initialize_lisp(argc, argv);

    RtMidiIn *midiin = new RtMidiIn();
    midiout = new RtMidiOut();
    feedback = new RtMidiOut();
    std::vector<unsigned char> message;
    unsigned int i;
    unsigned long nBytes;
    int midi_data[3];

    for (i=0; i!=midiin->getPortCount(); ++i)
    {
#ifdef d_midi
        std::cout << "In port: " << midiin->getPortName(i) << std::endl;
#endif
        if (midiin->getPortName(i) == midiInName)
            midiin->openPort(i);
        //RtMidiIn::RtMidiCallback callback_fn;
        //midiin->setCallback(callback_fn);
    }
    midiout->openVirtualPort("midi-itm");
    for (i=0; i!=feedback->getPortCount(); ++i)
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    sleep(1);
    delete midiin;
    delete midiout;
    delete feedback;
}
#ifdef d_midi
int i;
#endif
std::vector<unsigned char> midiout_message(3);
void send_midi(cl_object _channel, cl_object _note, cl_object _value)
{
	midiout_message[0] = (unsigned char)fix(_channel);
	midiout_message[1] = (unsigned char)fix(_note);
	midiout_message[2] = (unsigned char)fix(_value);
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
std::vector<unsigned char> feedback_message(3);
void send_feedback(cl_object _channel, cl_object _note, cl_object _value)
{
	feedback_message[0] = (unsigned char)fix(_channel);
	feedback_message[1] = (unsigned char)fix(_note);
	feedback_message[2] = (unsigned char)fix(_value);
	feedback->sendMessage( &feedback_message );
#ifdef d_midi
			std::cout << "FEEDBACK: ";
			for (fb_i=0; fb_i!=2; ++fb_i)
				std::cout << (int)feedback_message[fb_i] << ", ";
			std::cout << (int)feedback_message[fb_i] << std::endl;
#endif
}
