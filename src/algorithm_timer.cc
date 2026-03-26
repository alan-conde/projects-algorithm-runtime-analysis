#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // fork, pipe, dup2, close, execvp
#include <sys/wait.h>   // waitpid
#include <time.h>       // clock, CLOCKS_PER_SECOND

#include "algorithms.h"

// -------------------------------------
// Generate a random integer array
// -------------------------------------

int* generate_random_array(int n, int max_val) {
    // Allocate memory for array
    int* arr = (int*) malloc(n*sizeof(int));
    if(!arr) {
        perror("Malloc failed.\n");
        exit(1);
    }

    // Fill in array based on max_val
    for(int i = 0; i < n; ++i) {
        arr[i] = rand() % (max_val + 1);
    }
    return arr;
}

// -------------------------------------
// Write runtime for size n to script
// -------------------------------------

void write_to_script(int* fd, char* buf, size_t buf_size, int n, const char* algo, double* times, int runs) {
    // Write n and algorithm
    int num_bytes = snprintf(buf, buf_size, "%d,%s", n, algo);
    
    // Append each time value
    for(int i = 0; i < runs; ++i) {
        num_bytes += snprintf(buf+num_bytes, buf_size-num_bytes, ",%f", times[i]);
    }

    // Write new line
    num_bytes += snprintf(buf+num_bytes, buf_size-num_bytes, "\n");

    if(write(fd[1], buf, num_bytes) == -1) {
        perror("Write failed.\n");
        exit(1);
    }
}

// Seed random numbers
void seed_random() {
    srand(time(NULL));
}

int main(int argc, char* argv[]) {
    seed_random();
    int opt;
	int n = 100;
    int m = 100;
	int v = 100;

    const char* algos[] = {(char*)"bubbleSort", (char*)"selectionSort", (char*)"insertionSort"};
    int num_algos = sizeof(algos) / sizeof(algos[0]);

    // Set values from input
	while((opt = getopt(argc, argv, "n:m:v:")) != -1) {
		switch(opt) {
			case 'n':
				n = atoi(optarg);
				break;
            case 'm':
                m = atoi(optarg);
                break;
			case 'v':
				v = atoi(optarg);
				break;
		}
	}

    // Create unidirecitonal pipe
    int fd[2];
    if(pipe(fd) == -1) {
        perror("Pipe failed.\n");
        exit(1);
    }

    // Fork process
    pid_t pid = fork();
    if(pid < 0) {
        perror("Fork failed.\n");
        exit(1);
    }
    
    if(pid == 0) {
        // ===== CHILD PROCESS =====
        
        // Connect read end of pipe to STDIN
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);

        // Execute python script using execvp()
        char* args[] = {(char*)"python3", (char*)"scripts/runtime_plotter.py", nullptr};
        execvp(args[0], args);

        // Return if execution faild
        perror("Exec failed.\n");
        exit(1);
    }
    else {
        // ===== PARENT PROCESS =====

        close(fd[0]); // Parent writes only
        char buf[64 + 32*m];

        /* Write header to CSV output */
        int num_bytes = snprintf(buf, sizeof(buf), "input_size,algorithm");
        for(int i = 0; i < m; ++i) {
            num_bytes += snprintf(buf+num_bytes, sizeof(buf)-num_bytes, ",t%d", i);
        }
        num_bytes += snprintf(buf+num_bytes, sizeof(buf)-num_bytes, "\n");

        if(write(fd[1], buf, num_bytes) == -1) {
            perror("Write failed.\n");
            exit(1);
        }
        
        for(int i = 1; i <= n; ++i) {
            //! removed edge case
            // // Edge case (n = 0)
            // if(i == 0) {
            //     write_to_script(fd, buf, sizeof(buf), i, "bubbleSort", times, m);
            //     free(times);
            //     continue;
            // }
            
            // Iterate through each algorithm
            for(int a = 0; a < num_algos; ++a) {
                // Initialize empty set of time values
                double* times = (double*)calloc(m, sizeof(double));
                if(!times) {
                    perror("Calloc failed.\n");
                    exit(1);
                }

                // Run algorithm at each input_size m times
                for(int j = 0; j < m; ++j) {
                    int* arr = generate_random_array(i, v);

                    /* Time algorithm */
                    clock_t start = clock();
                    switch(a) {
                        case 0:
                            Algorithms::bubbleSort(arr, i);
                            break;
                        case 1:
                            Algorithms::selectionSort(arr, i);
                            break;
                        case 2:
                            Algorithms::insertionSort(arr, i);
                            break;
                    }
                    clock_t end = clock();

                    times[j] = (double) (end - start)/CLOCKS_PER_SEC;
                    free(arr);
                }

                // Send information to python script
                write_to_script(fd, buf, sizeof(buf), i, algos[a], times, m);
                free(times);
            }
        }

        // Close write end of pipe (send EOF signal)
        close(fd[1]);
        waitpid(pid, nullptr, 0);
    }
}