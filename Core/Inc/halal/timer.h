#ifndef INC_HALAL_TIMER_H_
#define INC_HALAL_TIMER_H_

class Timer {
public:
	static void start(void);
	static void stop(void);

	static void bpm_up(void);
	static void bpm_down(void);

private:
	static bool _running;
	static const unsigned int BPM_STEP = 1000;
};

#endif /* INC_HALAL_TIMER_H_ */
