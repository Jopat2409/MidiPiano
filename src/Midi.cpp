#include "..\include\io\midi\Midi.h"
#include "..\include\base\Logging.h"


Midi::Midi() : BinaryFile(BIG_ENDIAN) {

}

Midi::~Midi() {

	// Ensure file is closed
	Close();

	delete[] m_tracks;
}

Midi* Midi::FromFile(const char* path) {

	Midi* returnMidi = new Midi();
	INFO("Begining to read file");

	if (!returnMidi->Open(path)) {
		delete returnMidi;
		WARN("Midi file failed to open!")
		return nullptr;
	}

	// Load stuff here
	MIDI_ASSERT(returnMidi->LoadHeader(), "Failed to load header!");
	MIDI_ASSERT(returnMidi->LoadTrackChunks(), "Failed to load track!");

	returnMidi->Close();

	return Midi::GetLastError() == nullptr ? nullptr : returnMidi;
}

bool Midi::LoadHeader() {
	//CheckString
	if (!AssertString(HEADER_CHECK)) {
		WARN("Header failed header check")
		return false;
	}
	
	LoadType<int32_t>(m_headerLength);

	return LoadFormat() && LoadNumTracks() && LoadTimeDivision();
}

bool Midi::LoadFormat() {
	int16_t loadBuffer = LoadType<int16_t>();
	if (!(0 <= loadBuffer && loadBuffer <= 2)) {
		Midi::SetLastError("Invalid track format");
		return false;
	}
	m_trackFormat = static_cast<midi_format_t>(loadBuffer);
	return true;
}
bool Midi::LoadNumTracks() {
	int16_t loadBuffer = LoadType<int16_t>();
	if (m_trackFormat == MIDI_TRACK_SINGLE && loadBuffer != 1) {
		Midi::SetLastError("Invalid number of tracks for single track format");
		return false;
	}
	m_numTracks = loadBuffer;
	m_tracks = new Track[m_numTracks]{};
	return true;
}
bool Midi::LoadTimeDivision() {
	int16_t loadBuffer = LoadType<int16_t>();
	if (loadBuffer & (1 << 15)) {
		WARN("Loading wrong division")
	}
	else {
		// Delta time is only the first 15 bits
		m_timeDeltaDivision = loadBuffer & (~(1 << 15));
		// Calculate BPM 
	}
	return true;
}

bool Midi::LoadTrackChunks() {
	int count = 0;
	while (AssertString(TRACK_EVENT_CHECK) && count < m_numTracks) {
		INFO("Loading new track!")
		m_currentTrack = &m_tracks[count];
		int bytesLeft = LoadType<int32_t>();
		while (bytesLeft >= 0 && !m_trackEndInterrupt) {
			long before = GetReadBytes();
			LoadTrackEvent();
			bytesLeft -= (GetReadBytes() - before);
		}
		count++;
		m_trackEndInterrupt = false;
	}
	return true;
}

bool Midi::AssertString(const int& stringCode) {
	int32_t test;
	LoadType<int32_t>(test);
	return test == stringCode;
}

void Midi::LoadTrackEvent() {
	unsigned long deltaTime = LoadLEB128();
	uint8_t eventType = LoadType<uint8_t>();
	switch (eventType) {
	case 0xff:
		LoadMetaEvent(deltaTime);
		break;
	case 0xF0:
		LoadSysEvent(deltaTime);
		break;
	default:
		LoadMidiEvent(eventType, deltaTime);
		break;
	}
}

/* ---------- META EVENT HANDLING ---------------*/

void Midi::LoadMetaEvent(unsigned long dT) {
	switch (LoadType<char>()) {
	case META_SEQUENCE_NUM:
		break;
	case META_TEXT_EVENT:
		LoadTextEvent(META_TEXT_EVENT, dT);
		break;
	case META_COPYRIGHT_NOTICE:
		LoadTextEvent(META_COPYRIGHT_NOTICE, dT);
		break;
	case META_SEQUENCE_NAME:
		LoadTextEvent(META_SEQUENCE_NAME, dT);
		break;
	case META_INSTRUMENT_NAME:
		LoadTextEvent(META_INSTRUMENT_NAME, dT);
		break;
	case META_LYRIC_TEXT:
		LoadTextEvent(META_LYRIC_TEXT, dT);
		break;
	case META_MARKER_TEXT:
		LoadTextEvent(META_MARKER_TEXT, dT);
		break;
	case META_CUE_POINT:
		LoadTextEvent(META_CUE_POINT, dT);
		break;
	case META_MIDI_PREFIX:
		break;
	case META_PREFIX_PORT:
		SetDevicePort(dT);
		break;
	case META_END_OF_TRACK:
		LoadType<int8_t>();
		m_trackEndInterrupt = true;
		break;
	case META_TEMPO_SETTING:
		LoadTempoEvent(dT);
		break;
	case META_TIME_SIGNATURE:
		LoadTimeSignature(dT);
		break;
	case META_KEY_SIGNATURE:
		LoadKeySignature(dT);
		break;
	}
}

void Midi::LoadTempoEvent(unsigned long deltaTime) {
	int32_t bufferBytes = LoadType<int32_t>();
}

void Midi::SetDevicePort(unsigned long deltaTime) {
	if (LoadType<char>() != 0x01) {
		Midi::SetLastError("Invalid device port");
		return;
	}
	int8_t port = LoadType<int8_t>();
}

void Midi::LoadTextEvent(int type, unsigned long deltaTime) {
	char* bufferData;
	LoadString(bufferData);
	m_currentTrack->PushTextEvent(type, deltaTime, bufferData);
	delete[] bufferData;
}

void Midi::LoadTimeSignature(unsigned long deltaTime) {
	INFO("Loading time signature")
	if (LoadType<char>() != 0x04) {
		Midi::SetLastError("Faulty time sig");
		return;
	}
	uint8_t numerator = LoadType<uint8_t>();
	float denominator = (1 << LoadType<uint8_t>());
	m_currentTrack->PushTimeSignatureEvent(deltaTime, numerator, denominator, LoadType<uint8_t>(), LoadType<uint8_t>());
}

void Midi::LoadKeySignature(unsigned long deltaTime) {
	INFO("Loading key signature")
	if (LoadType<char>() != 0x02) {
		Midi::SetLastError("Faulty key sig");
		return;
	}
	int8_t sf = LoadType<int8_t>();
	bool isMinor = LoadType<int8_t>();
	m_currentTrack->PushKeySignatureEvent(deltaTime, sf < 0 ? SCALE_FLAT : SCALE_SHARP , isMinor ? MODE_MINOR : MODE_MAJOR, abs(sf));
}

/* ---------- MIDI EVENT HANDLING ------------ */
void Midi::LoadMidiEvent(uint8_t eventType, unsigned long dT) {
	uint8_t note, vel;
	switch (STATUS_FROM_MESSAGE(eventType)) {
	case MIDI_NOTE_OFF:
		LoadType<uint8_t>(note);
		LoadType<uint8_t>(vel);
		m_currentTrack->CloseNoteEvent(dT, note, vel);
		break;
	case MIDI_NOTE_ON:
		LoadType<uint8_t>(note);
		LoadType<uint8_t>(vel);
		m_currentTrack->PushNoteEvent(dT, note, vel);
		break;
	case MIDI_KEY_PRESSURE:
		//TODO replace
		LoadType<int16_t>();
		break;
	case MIDI_CONTROLLER:
		m_currentTrack->PushControllerEvent(dT, LoadType<int8_t>(), LoadType<int8_t>());
		break;
	case MIDI_PROGRAM_CHANGE:
		//TODO replace
		LoadType<int8_t>();
		break;
	case MIDI_CHANNEL_PRESSURE:
		//TODO replace
		LoadType<int8_t>();
		break;
	case MIDI_PITCH_BEND:
		//TODO replace
		LoadType<int16_t>();
		break;
	default:
		std::cout << "No midi event: " << eventType << "\n";
		break;
	}
}

void Midi::LoadSysEvent(unsigned long dT) {

}