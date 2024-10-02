/*
* Name: Lydia Martin
* Date: Tuesday 5:15
* Title: Lab2 – Write your C program to copy multiple files (say 10 files) by creating threads, 
* 		 each of which will be responsible of copying one file. 
* Description: This program copies multiple filse by creating threads to copy each file.
*/

// COEN 146L : Lab 2 - template to use for creating multiple thread to make file transfers (step 1)

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()

#define BUF_SIZE 2048 //buffer size

// data structure to hold copy
struct copy_struct {
	int thread_id; 
	char* src_filename;
	char* dst_filename; 
};

// copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	
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
        fclose (src);
        fclose(dst);
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
            fclose(src);
            fclose(dst);
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
	return 0;
}

// thread function to copy one file
void* copy_thread(void* arg) {
	struct copy_struct params = *(struct copy_struct*)arg;  // cast/dereference void* to copy_struct
	printf("thread[%i] - copying %s to %s\n", params.thread_id, params.src_filename, params.dst_filename);
	//call file copy function
	func_copy(src_filename, dst_filename);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	 // check correct usage of arguments in command line
	if (argc < 3 || (argc -1) % 2 != 0) {  
		fprintf(stderr, "usage: %s <n connections> ........\n", argv[0]);
		exit(1);
	}
	char* src_filename;
	char* dst_filename;
	char* src_filenames[argc / 2]; // array of source files
	char* dst_filenames[argc / 2]; // array of desintation files
	int num_threads; // number of threads to create
	
	pthread_t threads[num_threads]; //initialize threads
	struct copy_struct thread_params[num_threads]; // structure for each thread
	int i;
	for (i = 0; i < num_threads; i++) {
		// initialize thread parameters
		thread_params[i].thread_id = i;
		thread_params[i].src_filename = argc[2 + i];
		thread_params[i].dst_filename = argc[2 + num_threads + i];

		// create each copy thread
		//pthread_create(pt)
		// use pthread_create(.....);
	}

	// wait for all threads to finish
	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_exit(NULL);
}