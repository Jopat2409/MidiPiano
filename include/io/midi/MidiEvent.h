#ifndef _INTERNAL_MIDIEVENT_H
#define _INTERNAL_MIDIEVENT_H

#include "TimeSignature.h"

#include <string>

#define META_SEQUENCE_NUM 0x00
#define META_TEXT_EVENT 0x01
#define META_COPYRIGHT_NOTICE 0x02
#define META_SEQUENCE_NAME 0x03
#define META_INSTRUMENT_NAME 0x04
#define META_LYRIC_TEXT 0x05
#define META_MARKER_TEXT 0x06
#define META_CUE_POINT 0x07
#define META_MIDI_PREFIX 0x20
#define META_PREFIX_PORT 0x21
#define META_END_OF_TRACK 0x2F
#define META_TEMPO_SETTING 0x51
#define META_SMPTE_OFFSET 0x54
#define META_TIME_SIGNATURE 0x58
#define META_KEY_SIGNATURE 0x59
#define META_SEQUENCER_EVENT 0x7F



#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90
#define MIDI_KEY_PRESSURE 0xA0
#define MIDI_CONTROLLER 0xB0
#define MIDI_PROGRAM_CHANGE 0xC0
#define MIDI_CHANNEL_PRESSURE 0xD0
#define MIDI_PITCH_BEND 0xE0


#define MIDI_CHANNEL_MASK 0x0F
#define MIDI_STATUS_MASK 0xF0

#define CHANNEL_FROM_MESSAGE(msg) msg & MIDI_CHANNEL_MASK
#define STATUS_FROM_MESSAGE(msg) msg & MIDI_STATUS_MASK


class MidiEvent {
public:
	MidiEvent(int type, unsigned long timestamp);

	inline int GetEventType() { return m_eventType; };
	inline unsigned long GetEventTime() { return m_startTimestamp; };

	friend class Midi;
protected:
	unsigned long m_startTimestamp = 0;
	int m_eventType = -1;
};


class TextEvent : public MidiEvent {
public:
	TextEvent(int type, unsigned long timestamp, char* data);
	~TextEvent();

	inline char* GetTextData() { return m_data; };
private:
	char* m_data;
};

class TimeSignatureEvent : public MidiEvent {
public:
	TimeSignatureEvent(unsigned long timestamp, int n, int d, int cc, int bb);

	inline float GetTimeSignature() { return (float)(m_timeSig.numerator) / m_timeSig.denominator; };
private:
	time_signature_t m_timeSig;
};

typedef enum {
	MODE_UNSET = -1,
	MODE_MINOR = 1,
	MODE_MAJOR = 2
} key_mode_t;

typedef enum {
	SCALE_UNSET = -1,
	SCALE_FLAT = 1,
	SCALE_SHARP = 2
} key_scale_t;

struct key_signature_t {
	key_scale_t scale{ SCALE_UNSET };
	key_mode_t mode{ MODE_UNSET };
	int numAccidentals{ 0 };
};

class KeySignatureEvent : public MidiEvent {
public:
	
	KeySignatureEvent(unsigned long timestamp, key_scale_t scale, key_mode_t mode, int num);
	// To implement std::string ToString()

private:
	key_signature_t m_keySignature;

};

class Note : public MidiEvent {
public:
	friend class Track;
	Note(unsigned long timestamp, int note, int velocity);

	inline unsigned long GetStartTime() { return m_startTimestamp; };
	inline unsigned long GetEndTime() { return m_startTimestamp; };
	inline int GetNote() { return m_note; };
	inline int GetVelocity() { return m_velocity; };

	const char* GetNote_char();
	int GetNote_octave();
private:
	void Close(unsigned long endTime);
	unsigned long m_endTimestamp{0};
	int m_note{ -1 };
	int m_velocity{ -1 };
};

class ControllerChange : public MidiEvent {
public:
	ControllerChange(unsigned long timestamp, int controller, int val);

	inline int GetAffectedController() { return m_controller; };
	inline int GetValue() { return m_value; };
private:
	int m_controller{ -1 };
	int m_value{ -1 };
};

#endif