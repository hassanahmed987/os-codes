#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    char message[100];
    char yo[] = "hello from daddy";
    char *myfifo = "/tmp/myfifo"; // Changed /temp/ to /tmp/

    // Create the named pipe file
    mkfifo(myfifo, 0666);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) {
        // Parent: Writer
        int fd = open(myfifo, O_WRONLY); 
        write(fd, yo, sizeof(yo));
        close(fd);
    } 
    else {
        // Child: Reader
        int fd = open(myfifo, O_RDONLY); 
        read(fd, message, sizeof(message));
        printf("The message from parent is: %s\n", message);
        close(fd);
        
        // Remove the pipe file from the disk
        unlink(myfifo);
    }

    return 0;
}
