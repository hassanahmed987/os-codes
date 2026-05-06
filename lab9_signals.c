#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

// Global variable to track Ctrl+C presses
int interrupt_count = 0;

// --- Signal Handler Function ---
void signal_handler(int sig, siginfo_t *info, void *context) {
    if (sig == SIGUSR1) {
        printf("[STATUS] Machine %d: Operating normally.\n", info->si_pid);
    } 
    else if (sig == SIGUSR2) {
        printf("[ALERT] Machine %d: HIGH LOAD DETECTED!\n", info->si_pid);
    } 
    else if (sig == SIGALRM) {
        printf("\n>> SYSTEM CHECK: All sensors active and reporting.\n");
        alarm(3); // Schedule the next check in 3 seconds
    } 
    else if (sig == SIGINT) {
        interrupt_count++;
        if (interrupt_count == 1) {
            printf("\n[WARNING] Shutdown requested. Press Ctrl+C again to confirm.\n");
        } else {
            printf("\n[EXIT] Graceful shutdown. Cleaning up processes...\n");
            exit(0); 
        }
    }
}

int main() {
    srand(time(NULL)); // Seed random number generator

    struct sigaction sa;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO; // Required to get the child's PID via 'info->si_pid'
    sigemptyset(&sa.sa_mask);

    // Register handlers for all required signals
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    printf("Factory Monitor Started (PID: %d)\n", getpid());
    alarm(3); // Start the 3-second diagnostic timer

    // Create 3 "Machine" child processes
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {
            // --- Child Process Logic ---
            while (1) {
                sleep(rand() % 5 + 2); // Wait 2 to 6 seconds
                
                // Simple signal selection syntax
                int status_signal = (rand() % 2 == 0) ? SIGUSR1 : SIGUSR2;
                kill(getppid(), status_signal);
            }
        }
    }

    // --- Parent Loop ---
    while (1) {
        pause(); // Wait for signals to arrive
    }

    return 0;
}
