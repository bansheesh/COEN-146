//Name: Lydia Martin
//Date: Tuesday 5:15
//Title: Lab1 File: 2.c – Write your C program to copy files (binary and text) 
//using system calls, compile, debug, run, and test
//Description: This program can copy both binary and text files 
//using a variety of system calls
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 2048 //buffer size

int syscall_copy(char *src_filename, char *dst_filename){
    int src_fd = open(src_filename, O_RDONLY);	// opens a file for reading
	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		close(src_fd);
		exit(0);
	}
	int dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);	
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		close(dst_fd);
		exit(0);
	}


	// read/ write loop
	// allocate a buffer to store read data, and check if memory is allocated.
    char *buf = malloc((size_t)BUF_SIZE);   
	if (buf == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        close (src_fd);
        close(dst_fd);
        exit(0);
    }
    // reads up to BUF_SIZE bytes from src_filename
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read (src_fd, buf, BUF_SIZE))>0){
        // writes bytes_read to dst_filename
        bytes_written = write(dst_fd, buf, (size_t)bytes_read);
        //if written != read report error and close program
        if(bytes_written != bytes_read){
            fprintf(stderr, "Error writing to %s\n", dst_filename);
            free(buf);
            close(src_fd);
            close(dst_fd);
            exit(0);
        }
    }
   
    //check for read failure
    if (bytes_read < 0){
        fprintf(stderr, "Read error: %i\n", errno);
    }
	// closes src_fd file descriptor, dst_fd file descriptor and frees memory used for buf
    close(src_fd);
    close(dst_fd);
    free(buf);

	return 0;
}

