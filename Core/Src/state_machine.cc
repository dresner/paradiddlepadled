#include "state_machine.h"

State_Machine::State_Action State_Machine::state_machine[4] = {
	//   State,          LED 1,         LED 2,          Action 1,           Action 2,               Transition 1,      Transition 2
		{BPM_GUIDED,     {LED::Up{}},   {LED::Left{}},  event_bpm_up,       event_bpm_down,         &state_machine[2], &state_machine[1]},
		{BPM_LOOSE,      {LED::Up{}},   {LED::Right{}}, event_bpm_up,       event_bpm_down,         &state_machine[3], &state_machine[0]},
		{PATTERN_GUIDED, {LED::Down{}}, {LED::Left{}},  event_pattern_next, event_pattern_previous, &state_machine[0], &state_machine[3]},
		{PATTERN_LOOSE,  {LED::Down{}}, {LED::Right{}}, event_pattern_next, event_pattern_previous, &state_machine[1], &state_machine[2]},
};

State_Machine::State_Action * volatile State_Machine::current_state = &state_machine[0];
