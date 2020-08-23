#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>



#define PAGESIZE 4096
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int not_shared_var = 10;


int main(int argc, char** argv) {

    pid_t child_pid;

    uint8_t *shared_var = mmap(NULL, PAGESIZE,
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANON,  //MAP_ANONYMOUS
                                -1, 0);

    if (shared_var == MAP_FAILED) {
        // printf("\nMAP_FAILED\n");
        handle_error("mmap");
    }

    *shared_var = 100;

    if ((child_pid = fork()) == 0) {
        // child process
        not_shared_var++;
        (*shared_var)++;
    } else {
        // parent process wait for child
        wait(NULL);
    }

    if (child_pid == 0) {
        printf("\n This is child process");
    } else {
        printf("\n This is parent process");
    }
    
    printf("\n not_shared_var = %d", not_shared_var);
    printf("\n shared_var = %d \n", *shared_var);
    printf("\n");

}