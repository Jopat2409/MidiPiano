#include "..\include\io\midi\MidiEvent.h"

#include <iostream>

MidiEvent::MidiEvent(int type, unsigned long timestamp) {
	m_startTimestamp = timestamp;
	m_eventType = type;
}


TextEvent::TextEvent(int type, unsigned long timestamp, char* data) : MidiEvent(type, timestamp) {
	m_data = new char[strlen(data) + 1];
	sprintf(m_data, data);
}

TextEvent::~TextEvent() {
	delete[] m_data;
}

TimeSignatureEvent::TimeSignatureEvent(unsigned long timestamp, int num, int den, int cc, int bb) : MidiEvent(META_TIME_SIGNATURE, timestamp) {
	m_timeSig.numerator = num;
	m_timeSig.denominator = den;
	m_timeSig.clocksPerMetronome = cc;
	m_timeSig.notesPerClock = bb;
}

KeySignatureEvent::KeySignatureEvent(unsigned long timestamp, key_scale_t scale, key_mode_t mode, int num) : MidiEvent(META_KEY_SIGNATURE, timestamp) {
	m_keySignature.scale = scale;
	m_keySignature.mode = mode;
	m_keySignature.numAccidentals = num;
}

Note::Note(unsigned long timestamp, int key, int velocity) : MidiEvent(MIDI_NOTE_ON, timestamp) {
	m_note = key;
	m_velocity = velocity;
}
void Note::Close(unsigned long timestamp) {
	m_endTimestamp = timestamp;
}
const char* Note::GetNote_char() {
	static const char* notes[] = {"C", "C#", "D", "Eb","E", "F", "F#", "G", "G#", "A", "Bb", "B", "\0"};
	return notes[m_note % 12];
}
int Note::GetNote_octave() {
	return (m_note / 12) - 1;
}

ControllerChange::ControllerChange(unsigned long timestamp, int controller, int value) : MidiEvent(MIDI_CONTROLLER, timestamp), m_controller(controller), m_value(value) {

}