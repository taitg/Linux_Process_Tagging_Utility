/* CPSC 457 Spring 2016
 * Assignment 2
 *
 * Geordie Tait 10013837
 *
 * This program reads and prints the contents of /proc/ptag
 *
 * Based on code from:
 * http://linux.die.net/man/2/stat
 * http://cboard.cprogramming.com/c-programming/132716-reading-proc-file-user-space.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/* main function:
 * The main program function
 */
int main(int argc, char** argv) {
    struct stat sb;
    char c;

    // check if /proc/ptag exists
    if (stat("/proc/ptag", &sb) == 0) {
        // open the file
        FILE *fp = fopen("/proc/ptag", "r");

        // print each character until end of file is reached
        while (c != EOF) {
            c = fgetc(fp);
            printf("%c", c);
        }

        exit(EXIT_SUCCESS);

    // file does not exist, exit with error
    } else {
        fprintf(stderr, "/proc/ptag doesn't exist, perhaps tagstat module is not loaded.\nTry running \"sudo modprobe tagstat\"\n");
	exit(EXIT_FAILURE);
    }
}
