//
// Created by Alexander Galilov on 09.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

#define MAX_TASK_NUMBER 8

typedef void(*TSchedulerTask)(void *arg);

class Scheduler {
private:
    struct TaskDescriptor {
        unsigned long _interval;
        unsigned long _lastRun;
        bool _immediate;
        TSchedulerTask _task;
        void *_arg;
    };
public:
    Scheduler();

    void tick();

    /**
     * Add repeatable task to scheduler
     * @param task a pointer to void task(void *arg) function
     * @param arg a function argument
     * @param interval milliseconds between calls
     * @param runImmediate true = perform first call ASAP, false = perform first call after 'interval' milliseconds
     * @return an added task id or MAX_TASK_NUMBER when no free space for task descriptor
     */
    uint8_t addRepeatableTask(TSchedulerTask task, void *arg, long interval, bool runImmediate);

    void removeRepeatableTask(uint8_t taskId);

private:
    TaskDescriptor _tasks[MAX_TASK_NUMBER];
};


#endif //SCHEDULER_H
