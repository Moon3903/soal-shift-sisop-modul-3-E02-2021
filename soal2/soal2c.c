// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 
	// We use two pipes 
	// First pipe to send input string from parent 
	// Second pipe to send concatenated string from child 

	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	// char fixed_str[] = "forgeeks.org"; 
	// char input_str[100]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	p = fork(); 

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 
    if(p == 0){
        dup2(fd1[1], STDOUT_FILENO);

        close(fd1[0]);
        close(fd1[1]);

        char *argv[] = {"ps", "aux", NULL};
        execv("/bin/ps", argv);
    }
    else{
        pid_t child = fork();
        
        if(child < 0){
            fprintf(stderr, "fork 2 Failed" ); 
		    return 1;
        }

        if(child == 0){
            dup2(fd1[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);

            close(fd1[0]);
            close(fd1[1]);

            close(fd2[0]);
            close(fd2[1]);

            char *argv[] = {"sort", "-nrk", "3.3", NULL};
            execv("/usr/bin/sort", argv);
        }
        else {
            dup2(fd2[0], STDIN_FILENO);

            close(fd2[0]);
            close(fd2[1]);

            char *argv[] = {"head", "-5", NULL};
            execv("/usr/bin/head", argv);
        }
    }
}