## Real Time Linux programming

In computer science, real-time computing (RTC), or reactive computing, is the study of hardware and software systems that are subject to a "real-time constraint", for example operational deadlines from event to system response. Real-time programs must guarantee response within strict time constraints, often referred to as "deadlines".

Real-time responses are often understood to be in the order of milliseconds, and sometimes microseconds. Conversely, a system without real-time facilities, cannot guarantee a response within any timeframe (regardless of actual or expected response times).

Real-time software may use one or more of the following: synchronous programming languages, real-time operating systems, and real-time networks, each of which provide essential frameworks on which to build a real-time software application.
A real-time system may be one where its application can be considered (within context) to be mission critical. The anti-lock brakes on a car are a simple example of a real-time computing system – the real-time constraint in this system is the time in which the brakes must be released to prevent the wheel from locking. 
Real-time computations can be said to have failed if they are not completed before their deadline, where their deadline is relative to an event. 
A real-time deadline must be met, regardless of system load.

## Unix signals

Signals are a limited form of inter-process communication used in Unix, Unix-like, and other POSIX-compliant operating systems. A signal is an asynchronous notification sent to a process or to a specific thread within the same process in order to notify it of an event that occurred. Signals have been around since the 1970s Bell Labs Unix and have been more recently specified in the POSIX standard.
When a signal is sent, the operating system interrupts the target process's normal flow of execution to deliver the signal. Execution can be interrupted during any non-atomic instruction. If the process has previously registered a signal handler, that routine is executed. Otherwise, the default signal handler is executed.
Embedded programs may find signals useful for interprocess communications, as the computational and memory footprint for signals is small.

### Sending signals
The kill(2) system call will send a specified signal to a specified process, if permissions allow. Similarly, the kill(1) command allows a user to send signals to processes. The raise(3) library function sends the specified signal to the current process.
Exceptions such as division by zero or a segmentation violation will generate signals (here, SIGFPE and SIGSEGV respectively, which both by default cause a core dump and a program exit).
The kernel can generate signals to notify processes of events. For example, SIGPIPE will be generated when a process writes to a pipe which has been closed by the reader; by default, this causes the process to terminate, which is convenient when constructing shell pipelines.
Typing certain key combinations at the controlling terminal of a running process causes the system to send it certain signals:
Ctrl-C (in older Unixes, DEL) sends an INT signal (SIGINT); by default, this causes the process to terminate.
Ctrl-Z sends a TSTP signal (SIGTSTP); by default, this causes the process to suspend execution.
Ctrl-\ sends a QUIT signal (SIGQUIT); by default, this causes the process to terminate and dump core.
Ctrl-T (not supported on all UNIXes) sends an INFO signal (SIGINFO); by default, and if supported by the command, this causes the operating system to show information about the running command.
These default key combinations with modern operating systems can be changed with the stty command

### Handling signals
Signal handlers can be installed with the signal() system call. If a signal handler is not installed for a particular signal, the default handler is used. Otherwise the signal is intercepted and the signal handler is invoked. The process can also specify two default behaviors, without creating a handler: ignore the signal (SIGIGN) and use the default signal handler (SIGDFL). There are two signals which cannot be intercepted and handled: SIGKILL and SIGSTOP

### Semaphore variables
In computer science, particularly in operating systems, a semaphore is a variable or abstract data type that is used for controlling access, by multiple processes, to a common resource in a parallel programming or a multi user environment.

A useful way to think of a semaphore is as a record of how many units of a particular resource are available, coupled with operations to safely (i.e., without race conditions) adjust that record as units are required or become free, and, if necessary, wait until a unit of the resource becomes available. 

Semaphores are a useful tool in the prevention of race conditions; however, their use is by no means a guarantee that a program is free from these problems. Semaphores which allow an arbitrary resource count are called counting semaphores, while semaphores which are restricted to the values 0 and 1 (or locked/unlocked, unavailable/available) are called binary semaphores.


<em>Source: Wikipedia</em>
