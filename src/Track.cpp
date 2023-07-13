#include "..\include\io\midi\Track.h"

void Track::PushTextEvent(int eventType, unsigned long deltaTime, char* data) {
	m_events.push_back(new TextEvent(eventType, deltaTime, data));
}

void Track::PushTimeSignatureEvent(unsigned long deltaTime, int n, int d, int c, int b) {
	m_events.push_back(new TimeSignatureEvent(deltaTime, n, d, c, b));
}

void Track::PushKeySignatureEvent(unsigned long deltaTime,key_scale_t scale, key_mode_t mode, int num) {
	m_events.push_back(new KeySignatureEvent(deltaTime, scale, mode, num));
}
void Track::PushControllerEvent(unsigned long time, int controller, int value) {
	m_events.push_back(new ControllerChange(time, controller, value));
}

void Track::PushNoteEvent(unsigned long time, int key, int velocity) {
	if (velocity == 0)
		return CloseNoteEvent(time, key, velocity);
	Note* note = new Note(time, key, velocity);
	m_openNotes.push_back(note);
	m_events.push_back(note);
	std::cout << "Pushed note: " << note->GetNote_char() << note->GetNote_octave() << "\n";
}
void Track::CloseNoteEvent(unsigned long time, int key, int velocity) {
	Note* toClose = nullptr;
	for (Note* note : m_openNotes) {
		if (note->GetNote() == key) {
			toClose = note;
			break;
		}
	}
	if (toClose == nullptr) {
		WARN("Closing note that does not exist");
	}
	else {
		toClose->Close(time);
		remove(m_openNotes.begin(), m_openNotes.end(), toClose);
	}
}

Track::~Track() {
	delete[] m_copyrightNotice;
	delete[] m_title;
	delete[] m_instrumentName;

	for (MidiEvent* e : m_events)
		delete e;
}