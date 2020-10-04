#ifndef OPENGL_TIMER_H
#define OPENGL_TIMER_H

#include <chrono>
#include <iostream>

class Timer {
	using clock = std::chrono::high_resolution_clock;
	std::chrono::time_point<clock> start;
public:
	Timer() : start(clock::now()) {}
	double getElapsed() const {
		return std::chrono::duration_cast<std::chrono::duration<double,std::ratio<1>>>(clock::now() - start).count();
	}
	~Timer() {
		std::cout<<"Time: "<<getElapsed()<<"s";
	}
};
class FPS {
	double lastTime;
	int nbFrames;
	Timer t;
	double printAt;
public:
	FPS() {
		lastTime = 0;
		nbFrames = 0;
		t = Timer();
		printAt = t.getElapsed();
	}
	void tick() {
		double currentTime = t.getElapsed();
		nbFrames++;
		if (printAt < currentTime) {
			// printf and reset timer
			printf("%fFPS\n", double(nbFrames)/currentTime);
			printAt = currentTime+1;
		}
	}
};
#endif //OPENGL_TIMER_H
