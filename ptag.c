/* CPSC 457 Spring 2016
 * Assignment 2
 *
 * Geordie Tait 10013837
 *
 * This is a user-level utility for tagging processes with strings
 * based on pid. It is designed to work with a new system call
 * labeled ptag (#337).
 */

#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

/* exitWithUsage method:
 * Prints a usage statement to stderr and exits with failure status
 */
void exitWithUsage() {
    fprintf(stderr, "Usage: ./ptag [pid] [-a tag] [-r tag]\n");
    exit(EXIT_FAILURE);
}

/* main function:
 * The main program function
 */
int main(int argc, char *argv[])
{
    int opt;
    char *atag, *rtag;
    int afnd = 0;
    int rfnd = 0;
    int success = 0;

    // check number of arguments
    if ((argc != 4) && (argc != 6))
        exitWithUsage();

    // parse arguments
    while ((opt = getopt(argc, argv, "a:r:")) != -1) {
        switch (opt) {
            case 'a':
                afnd = 1;
                atag = optarg;
                break;
            case 'r':
                rfnd = 1;
                rtag = optarg;
                break;
            default:
                exitWithUsage();
        }
    }

    // check parsed arguments
    if (optind >= argc)
        exitWithUsage();

    // attempt to add a tag, checking for errors
    if (afnd == 1) {
        int astatus = syscall(337, atoi(argv[optind]), atag, 0);
	if (astatus == 0) {
		printf("  ptag: Tag \"%s\" attached to PID %s.\n", atag, argv[optind]);
		success = 1;
	} else if (astatus == 1) {
		fprintf(stderr, "  ptag: %s is not a valid PID.\n", argv[optind]);
	} else if (astatus == 2) {
		fprintf(stderr, "  ptag: Tag \"%s\" already attached to PID %s.\n", atag, argv[optind]);
	} else {
		fprintf(stderr, "  ptag: An error occurred.\n");
	}
    }

    // attempt to remove a tag, checking for errors
    if (rfnd == 1) {
        int rstatus = syscall(337, atoi(argv[optind]), rtag, 1);
	if (rstatus == 0) {
		printf("  ptag: Tag \"%s\" removed from PID %s.\n", rtag, argv[optind]);
		success = 1;
	} else if (rstatus == 1) {
		fprintf(stderr, "  ptag: %s is not a valid PID.\n", argv[optind]);
	} else if (rstatus == 3) {
		fprintf(stderr, "  ptag: Tag \"%s\" is not attached to PID %s and could not be removed.\n", rtag, argv[optind]);
	} else {
		fprintf(stderr, "  ptag: An error occurred.\n");
	}
    }

    // exit with the appropriate success/failure status
    if (success == 1)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}
