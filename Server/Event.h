#pragma once
#include <chrono>
#include <queue>

using namespace std::chrono;

struct Event {
	int objID;
	int eventID;
	high_resolution_clock::time_point wakeupTime;
	int targetID;
	
	constexpr bool operator<(const Event& e) const {
		return wakeupTime > e.wakeupTime;
	}
};

void AddTimer(int oid, int event_type, high_resolution_clock::time_point duration, int target);
void TimerThread();