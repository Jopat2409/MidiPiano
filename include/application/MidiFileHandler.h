#ifndef _INTERNAL_MIDIFILEHANDLER_H
#define _INTERNAL_MIDIFILEHANDLER_H

#include "..\io\midi\Midi.h"

struct track_handler_t {
	Track* handledTrack{nullptr};
	int currentTempo{ 0 };

	unsigned long currentPtrLeft{ 0 };
	unsigned long currentPtrRight{ 0 };
};

// Handles what notes are at what times
class MidiHandler {
public:
	MidiHandler();
	~MidiHandler();

	int LoadSong(const char* path);
	void UpdateSong(unsigned long deltaTime);

	Note* ToPlay();
	Note* ToRender();
private:

	Midi* m_currentSong{nullptr};
	track_handler_t* m_trackHandlers{nullptr};

	unsigned long m_songTime{ 0 };
	unsigned long m_songPtrLeft{ 0 };
	unsigned long m_songPtrRight{ 0 };
};




#endif