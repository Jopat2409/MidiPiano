#ifndef _INTERNAL_MIDI_H
#define _INTERNAL_MIDI_H

#include "..\BinaryFile.h"

#include "Track.h"
#include "MidiFormat.h"


#include <iostream>
#include <vector>
#include <string>

#define HEADER_CHECK 0x4d546864
#define TRACK_EVENT_CHECK 0x4d54726b

class Midi : public BinaryFile {
public:
	~Midi();

	// Loading
	static Midi* FromFile(const char* path);

	inline midi_format_t GetTrackFormat() { return m_trackFormat; };

	inline Track* GetTracks() { return m_tracks; };
	inline Track* GetTrack(int track) { return &(m_tracks[track]); };
	inline int GetNumTracks() { return m_numTracks; };

private:
	Midi();

	// Header loading methods
	bool LoadHeader();
	bool LoadFormat();
	bool LoadNumTracks();
	bool LoadTimeDivision();


	bool LoadTrackChunks();
	bool AssertString(const int& stringCode);
	void LoadTrackEvent();

	void LoadMetaEvent(unsigned long dT);

	void LoadTextEvent(int type, unsigned long deltaTime);
	void LoadTimeSignature(unsigned long deltaTime);
	void LoadKeySignature(unsigned long deltaTime);
	void LoadTempoEvent(unsigned long deltaTime);

	void SetDevicePort(unsigned long deltaTime);

	void LoadSysEvent(unsigned long dT);
	void LoadMidiEvent(uint8_t eventType, unsigned long dT);


	// Header Fields
	int32_t m_headerLength{-1};
	midi_format_t m_trackFormat{ MIDI_TRACK_NOT_SET };
	int16_t m_numTracks{ -1 };
	int16_t m_timeDeltaDivision{-1};

	Track* m_tracks{nullptr};
	Track* m_currentTrack{nullptr};

	bool m_trackEndInterrupt{false};

	int currentTime{ 0 };

};

#define MIDI_ASSERT(x, msg) if (!x) Midi::SetLastError(msg);


#endif