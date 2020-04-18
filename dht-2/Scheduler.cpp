//
// Created by Alexander Galilov on 09.04.2020.
// E-Mail: alexander.galilov@gmail.com

#include "Scheduler.h"

Scheduler::Scheduler() {
  memset(_tasks, 0, sizeof(_tasks));
}

void Scheduler::tick() {
  unsigned long t = millis();
  for (uint8_t i = 0; i < MAX_TASK_NUMBER; i++) {
    if (t < _tasks[i]._lastRun) { // overflow correction
      _tasks[i]._lastRun = (0xFFFFFFFFLU - _tasks[i]._lastRun);
    }
    auto nextRunAt = _tasks[i]._lastRun + _tasks[i]._interval;
    if (_tasks[i]._task != nullptr &&
        (t > nextRunAt || _tasks[i]._immediate)) {
      _tasks[i]._immediate = false;
      _tasks[i]._task(_tasks[i]._arg);
      _tasks[i]._lastRun = t;
    }
  }
}

uint8_t Scheduler::addRepeatableTask(TSchedulerTask task, void *arg,
                                     long interval,
                                     bool runImmediate) {
  for (uint8_t i = 0; i < MAX_TASK_NUMBER; i++) {
    if (_tasks[i]._task == nullptr) {
      _tasks[i]._task = task;
      _tasks[i]._arg = arg;
      _tasks[i]._interval = interval;
      _tasks[i]._lastRun = 0;
      _tasks[i]._immediate = runImmediate;
      return i;
    }
  }
  return MAX_TASK_NUMBER;
}

void Scheduler::removeRepeatableTask(uint8_t taskId) {
  _tasks[taskId % MAX_TASK_NUMBER]._task = nullptr;
}
