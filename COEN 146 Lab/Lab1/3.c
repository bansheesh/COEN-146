//Name: Lydia Martin
//Date: Tuesday 5:15
//Title: Lab1 File: 3.c – Calculate the time taken to copy files for both 1.c and 2.c 
//Description: This program calculates the time taken to copy files for both 2.c and 
//2.c using the clock function at the beginning and end of the code to measure the 
//time, subtract the values, and then divide by CLOCKS_PER_SEC to get processor time. 
#include <time.h> //clock ()
#include "1.c"
#include "2.c"

void check_copy_times(char *src_filename, char *dst_filename){
    clock_t start, end, syscall_start, syscall_end;
    //capture runtime of func_copy() using start clock, call the copy, end the clock
    start = clock();
    func_copy(src_filename, dst_filename);
    end = clock();
    double func_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //capture runtime of syscall_copy() using start clock, call the copy, end the clock
    syscall_start = clock();
    syscall_copy(src_filename, dst_filename);
    syscall_end = clock();
    double syscall_time_used = ((double)(syscall_end - syscall_start)) / CLOCKS_PER_SEC;

    //print run times
    printf("time to copy using functions: %.7f\n", func_time_used);
	printf("time to copy using syscalls: %.7f\n", syscall_time_used);
}

int main(int argc, char * argv[]){
    if (argc != 3) {  // check correct usage
	    fprintf(stderr, "usage: %s <src_filename> <dst_filename>\n", argv[0]);
		exit(1);
	}
    //get the source and destination files from the command line arguments
	const char* src_filename = argv[1];
    const char* dst_filename = argv[2];
    //call the check copy times function
	check_copy_times(src_filename, dst_filename);
    return 0;
}
