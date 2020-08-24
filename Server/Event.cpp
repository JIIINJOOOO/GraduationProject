#include "Event.h"
#include "main.h"
using namespace std;

mutex timerLock;
priority_queue<Event> timerQueue;
extern HANDLE g_iocp;

void AddTimer(int oid, int event_type, high_resolution_clock::time_point duration, int target) {
	Event ev{ oid, event_type, duration, target };
	timerLock.lock();
	timerQueue.push(ev);
	timerLock.unlock();
}

void TimerThread() {
	while (true) {
		this_thread::sleep_for(1ms);
		while (true) {
			timerLock.lock();
			if (timerQueue.empty()) {
				timerLock.unlock();
				break;
			}
			if (timerQueue.top().wakeupTime > high_resolution_clock::now()) {
				timerLock.unlock();
				break;
			}
			Event ev = timerQueue.top();
			timerQueue.pop();
			timerLock.unlock();
			EXOVER *exover = new EXOVER;
			exover->op = EV_BOSS;
			PostQueuedCompletionStatus(g_iocp, 1, ev.objID, &exover->over);
		}
	}
}