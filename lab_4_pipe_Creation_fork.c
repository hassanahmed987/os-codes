#include <stdio.h>
#include <unistd.h>   // For pipe(), fork(), and close()
#include <sys/types.h> // For pid_t
#include <sys/wait.h>  // For wait()

int main() {
    pid_t pid;
    int fd[2];
    char msg[] = "abbu speaking";
    char buffer[100];

    // Create the pipe before forking
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) { 
        // Parent Process: Writes to the pipe
        close(fd[0]); // Close unused read end
        
        write(fd[1], msg, sizeof(msg));
        close(fd[1]); // Finished writing
        
        wait(NULL);   // Wait for child to finish printing
    } 
    else { 
        // Child Process: Reads from the pipe
        close(fd[1]); // Close unused write end
        
        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        
        close(fd[0]); // Finished reading
    }

    return 0;
}
