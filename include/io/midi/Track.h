#ifndef _INTERNAL_TRACK_H
#define _INTERNAL_TRACK_H

#include <vector>
#include "MidiEvent.h"
#include "..\..\base\Logging.h"


class Track {
public:
	~Track();
	friend class Midi;
	void PushTextEvent(int eventType, unsigned long deltaTime, char* data);
	void PushTimeSignatureEvent(unsigned long deltaTime, int n, int d, int c, int b);
	void PushKeySignatureEvent(unsigned long deltaTime, key_scale_t scale, key_mode_t mode, int num);
	void PushNoteEvent(unsigned long time, int key, int velocity);
	void CloseNoteEvent(unsigned long time, int key, int velocity);
	void PushControllerEvent(unsigned long time, int controller, int value);

	inline std::vector<MidiEvent*>& GetEvents() { return m_events; };
private:
	std::vector<MidiEvent*> m_events;
	std::vector<Note*> m_openNotes;

	char* m_title{ nullptr };
	char* m_copyrightNotice{ nullptr };
	char* m_instrumentName{nullptr};

	int m_sequenceNum{-1};
};


#endif