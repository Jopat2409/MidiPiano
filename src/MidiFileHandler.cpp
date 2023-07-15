#include "..\include\application\MidiFileHandler.h"

MidiHandler::MidiHandler() {

}

MidiHandler::~MidiHandler() {

}

int MidiHandler::LoadSong(const char* path) {
	m_currentSong = Midi::FromFile(path);
	if (Midi::GetLastError())
		return -1;
	// Sort out track handlers, parse 0 delta time events that aren't notes
	m_trackHandlers = new track_handler_t[m_currentSong->GetNumTracks()];

	for (int i = 0; i < m_currentSong->GetNumTracks(); i++) {
		track_handler_t cTrackHandler = m_trackHandlers[i];
		std::vector<MidiEvent*> cTrackEvents = cTrackHandler.handledTrack->GetEvents();
		for (int x = 0; x < cTrackEvents.size(); x++) {
			// Handle 0 delta time events that are not notes
			switch (cTrackEvents[x]->GetEventType()) {
			case META_TEMPO_SETTING:
				break;
			}
		}
	}

}