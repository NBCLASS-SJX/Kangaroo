#include "daemon.h"

#if defined(__GNUC__)

void sig_process(int signo, sighandler_t handler)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(signo, &sa, NULL);
}

void sig_handler(int signo)
{
    if (signo == SIGPIPE) {
        return;
    }
}

void run_daemon()
{
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid > 0) {
        exit(0);
    }

    setsid();
    sig_process(SIGHUP, SIG_IGN);
    sig_process(SIGTERM, SIG_IGN);
    sig_process(SIGPIPE, sig_handler);
}

#endif