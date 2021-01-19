#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

/* Both disable and enable was implemented from
 https://www.gnu.org/software/libc/manual/html_node/Process-Signal-Mask.html
https://www.gnu.org/software/libc/manual/html_node/Checking-for-Pending-Signals.html*/

// handler for SIGVTALRM, signum++ and signum-- there to avoid warnings
void yield_handler(__attribute__((unused)) int signum) {
        uthread_yield();
}
 
void preempt_disable(void) {
        sigset_t sigDisable;
        sigemptyset(&sigDisable);
        sigaddset(&sigDisable, SIGVTALRM);
        sigprocmask(SIG_BLOCK, &sigDisable, NULL);
}

void preempt_enable(void) {
        sigset_t sigEnable;
        sigemptyset(&sigEnable);
        sigaddset(&sigEnable, SIGVTALRM);
        sigprocmask(SIG_UNBLOCK, &sigEnable, NULL);
}

void preempt_start(void) {
        struct itimerval alarm;
        struct sigaction sa;

        // Convert to microseconds as values should not be decimal numbers
        int timer = 1000000/HZ;

        /* Set up signal handler to call uthread_yield() 
        when alarm sends signal SIGVTALRM*/
        //Influenced from slide 41 of lecture 1 (syscalls)
        sa.sa_handler = &yield_handler;
        sigaction(SIGVTALRM, &sa, NULL);

        // Use the defined hertz to determine period
        // Followed instructions from https://man7.org/linux/man-pages/man2/setitimer.2.html
        alarm.it_interval.tv_sec = 0;
        alarm.it_interval.tv_usec = timer;
        alarm.it_value.tv_sec = 0;
        alarm.it_value.tv_usec = timer;

        setitimer(ITIMER_VIRTUAL, &alarm, NULL);
}

// https://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html
// This explains SA_RESETHAND resets signal handler using sigaction
void preempt_stop(void) {
        struct itimerval alarm;
        struct sigaction sa;

        // Restore sigaction to default
        sa.sa_flags = SA_RESETHAND | SA_NODEFER;
        
        sigaction(SIGVTALRM, &sa, NULL);

        // Restore timer to default
        getitimer(ITIMER_VIRTUAL, &alarm);
        alarm.it_interval.tv_sec = 0;
        alarm.it_interval.tv_usec = 0;
        alarm.it_value.tv_sec = 0;
        alarm.it_value.tv_sec = 0;

        // No error handling as this resets the clock and will return -1
        setitimer(ITIMER_VIRTUAL, &alarm, NULL);
}

