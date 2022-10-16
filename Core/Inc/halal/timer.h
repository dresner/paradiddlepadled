#ifndef INC_HALAL_TIMER_H_
#define INC_HALAL_TIMER_H_

class Timer {
public:
	static void start(void);
	static void stop(void);

	static void bpm_up(unsigned int how_much);
	static void bpm_down(unsigned int how_much);

private:
	static bool _running;
	static const unsigned int BPM_STEP = 271186;
};

#endif /* INC_HALAL_TIMER_H_ */
