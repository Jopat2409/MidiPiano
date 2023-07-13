#ifndef _INTERNAL_MIDI_FORMAT_H
#define _INTERNAL_MIDI_FORMAT_H

typedef enum {
	MIDI_TRACK_NOT_SET = -1,
	MIDI_TRACK_SINGLE = 0,
	MIDI_TRACK_SIMOULTANEOUS = 1,
	MIDI_TRACK_SEQUENTIAL = 2
} midi_format_t;

#endif