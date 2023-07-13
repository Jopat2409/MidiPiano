#ifndef _INTERNAL_TIME_SIG_H
#define _INTERNAL_TIME_SIG_H

struct time_signature_t {
	int numerator{ 0 };
	int denominator{ 0 };
	int clocksPerMetronome{ 0 };
	int notesPerClock{ 0 };
};


#endif