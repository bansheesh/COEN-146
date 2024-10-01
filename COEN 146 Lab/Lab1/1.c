//Name: Lydia Martin
//Date: Tuesday 5:15
//Title: Lab1 – Write your C program to copy files (binary and text) 
//using functions, compile, debug, run, and test
//Description: This program can copy both binary and text files 
//using a variety of functions
//#include <cstdio>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>  
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 2048 //buffer size

int func_copy(char *src_filename, char *dst_filename){
	FILE *src = fopen(src_filename, "r");	// opens a file for reading
	if (src == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}
	FILE *dst = fopen(dst_filename, "w");	// opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		exit(0);
	}
	// allocate a buffer to store read data, and check if memory is allocated.
    char *buf = malloc((size_t)BUF_SIZE);  
    if (buf == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        close (src);
        close(dst);
        exit(0);
    }
    // reads content of file is loop iterations until end of file
	size_t bytes_read, bytes_written;
    while ((bytes_read = fread(buf, 1, BUF_SIZE, src)) > 0){
        // writes bytes_read to dst_filename
        bytes_written = fwrite(buf, 1, bytes_read, dst);
        // if written != read report error and close program
        if(bytes_written != bytes_read){
            fprintf(stderr, "Error writing to %s\n", dst_filename);
            free(buf);
            close(src);
            close(dst);
            exit(0);
        }
    }
    
    //check for read failure
    if (bytes_read < 0){
        fprintf(stderr, "Read error: %i\n", errno);
    }
	// closes src file pointer, dst file pointer, and frees memory used for buf
    fclose(src);
    fclose(dst);
    free(buf);
	return 0;

}



