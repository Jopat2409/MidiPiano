#include "..\include\io\midi\Midi.h"
#include <iostream>

int main() {
	Midi* test = Midi::FromFile("D:\\test.mid");
	if (test == nullptr) std::cout << Midi::GetLastError();
	Track* track02 = test->GetTrack(1);
	for (MidiEvent* currentEvent : track02->GetEvents()) {
		if (currentEvent->GetEventType() == MIDI_NOTE_ON) {
			Note* cNote = (Note*)currentEvent;
			std::cout << "found note " << cNote->GetNote_char() << " at time " << currentEvent->GetEventTime() << "\n";
		}
			
	}
	delete test;
	return 0;
}