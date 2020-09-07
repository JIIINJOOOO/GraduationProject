#include "Event.h"
#include "main.h"
#include "CPlayer.h"
using namespace std;

mutex timerLock;
priority_queue<Event> timerQueue;
extern HANDLE g_iocp;
extern map<int, CPlayer*> g_player;

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
			switch (ev.eventID) {
			case EV_BOSS: {
				EXOVER *exover = new EXOVER;
				exover->op = EV_BOSS;
				PostQueuedCompletionStatus(g_iocp, 1, ev.objID, &exover->over);
			}break;
			case EV_MONSTER: {
				EXOVER *exover = new EXOVER;
				exover->op = EV_MONSTER;
				PostQueuedCompletionStatus(g_iocp, 1, ev.objID, &exover->over);
			}break;
			case EV_BERSERK:
				g_player[ev.objID]->BerserkOff();
				break;
			case EV_ATK_OFF:
				g_player[ev.objID]->isAttack = false;
				g_player[ev.objID]->lastAtkTime = high_resolution_clock::now();
				break;
			case EV_GUARD_OFF:
				g_player[ev.objID]->isGuard = false;
				break;
			case EV_EVADE_OFF:
				g_player[ev.objID]->isEvade = false;
				break;
			case EV_FIREBALL: {
				EXOVER *exover = new EXOVER;
				exover->op = EV_FIREBALL;
				PostQueuedCompletionStatus(g_iocp, 1, ev.objID, &exover->over);
				// g_player[ev.objID]->fireball.Update();
			}break;
			case EV_ASS_OFF:
				g_player[ev.objID]->isAssasinate = false;
				break;
			}
		}
	}
}