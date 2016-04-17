#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "MidiIn.h"
#include "MidiConstants.h"
#include "MidiMessage.h"
#include "cinder/Font.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define TAU M_PI*2

class VDApp : public App {

public:

	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void cleanup() override;
	void midiListener(midi::Message msg);

	midi::Input mInput;
	vector <int> notes;
	vector <int> cc;

	Font mFont;
	std::string status;
};

void VDApp::midiListener(midi::Message msg){
	switch (msg.status)
	{
	case MIDI_NOTE_ON:
		notes[msg.pitch] = msg.velocity;
		status = "Pitch: " + toString(msg.pitch) + "\n" + 
			"Velocity: " + toString(msg.velocity);
		break;
	case MIDI_NOTE_OFF:
		break;
	case MIDI_CONTROL_CHANGE:
		cc[msg.control] = msg.value;
		status = "Control: " + toString(msg.control) + "\n" + 
			"Value: " + toString(msg.value);
		break;
	default:
		break;
	}

}
void VDApp::setup()
{
	mInput.listPorts();
	console() << "NUMBER OF PORTS: " << mInput.getNumPorts() << endl;
	for (int i = 0; i < mInput.getNumPorts(); i++)
	{
		console() << mInput.getPortName(i) << endl;
	}
	mInput.openPort(0);

	mInput.midiSignal.connect(boost::bind(&VDApp::midiListener, this, boost::arg<1>::arg()));


	for (int i = 0; i < 127; i++)
	{
		notes.push_back(0);
		cc.push_back(0);
	}
	mFont = Font("Arial", 25);
}

void VDApp::update()
{

}
void VDApp::cleanup()
{
	quit();
}
void VDApp::mouseDown(MouseEvent event)
{
    
}

void VDApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::pushMatrices();
	gl::translate(getWindowCenter());
	for (int i = 0; i < notes.size(); i++)
	{
		float x = 200*sin((i*2.83) * M_PI / 180);
		float y = 200*cos((i*2.83) * M_PI / 180);
		float lx = (200 - cc[i])*sin((i*2.83) * M_PI / 180);
		float ly = (200 - cc[i])*cos((i*2.83) * M_PI / 180);

		gl::color(Color(1,1,1));
		gl::drawStrokedCircle(vec2(x, y), 5+(notes[i]/4));
		gl::drawLine(vec2(x, y), vec2(lx, ly));
		gl::color(Color(notes[i], notes[i], notes[i]));
		gl::drawSolidCircle(vec2(x, y), 5 + (notes[i] / 4));
		
	}
	
	
	gl::popMatrices();
	gl::drawStringCentered(status, getWindowCenter(), Color(1,1,1), mFont);
}


CINDER_APP(VDApp, RendererGl)
