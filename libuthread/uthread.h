#ifndef _UTHREAD_H
#define _UTHREAD_H

/*
 * uthread_func_t - Thread function type
 * @arg: Argument to be passed to the thread
 */
typedef void (*uthread_func_t)(void *arg);

/*
 * uthread_start - Start the multithreading library
 * @func: Function of the first thread to start
 * @arg: Argument to be passed to the first thread
 *
 * This function should only be called by the main execution thread of the
 * process. It starts the multithreading scheduling and becomes the "idle"
 * thread. It returns once all the threads have finished running.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */
int uthread_start(uthread_func_t func, void *arg);

/*
 * uthread_create - Create a new thread
 * @func: Function to be executed by the thread
 * @arg: Argument to be passed to the thread
 *
 * This function creates a new thread running the function @func to which
 * argument @arg is passed.
 *
 * Return: 0 in case of success, -1 in case of failure (e.g., memory allocation,
 * context creation).
 */
int uthread_create(uthread_func_t func, void *arg);

/*
 * uthread_yield - Yield execution
 *
 * This function is to be called from the currently active and running thread in
 * order to yield for other threads to execute.
 */
void uthread_yield(void);

/*
 * uthread_exit - Exit from currently running thread
 *
 * This function is to be called from the currently active and running thread in
 * order to finish its execution.
 *
 * This function shall never return.
 */
void uthread_exit(void);

#endif /* _THREAD_H */
