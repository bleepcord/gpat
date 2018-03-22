#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <assert.h>

#include "data.h"
#include "gpacalculations.h"

/*
 * gpat
 * simple gpa tracking util
 * TODO: lots of valgrind errors to fix
 */

int
printHelp(void)
{
	printf("\nusage: gpat [actions]\n\n"
	    "--add (-a) add a class to the data file. \"-a class_name\" or \"-a \'class name\'\".\n"
	    "           This flag must be used with the \'-c\' and the \'-g\' flag.\n"
	    "--credits (-c) the amount of credits the class being added is worth.\n"
	    "--grade (-g) the letter grade received in the class.\n"
	    "--delete (-d) delete a class from the data file. \"-d class_name\" or \"-d \'class name\'\".\n"
	    "              This flag will delete all classes from the data file that contain the string passed to the flag.\n"
	    "--help (-h) Print this menu.\n"
	    "--print (-p) Print a list of classes in the data file as well as the cumulative GPA.\n"
	    "--reset (-r) Delete the data file. Only do this if you want to remove all of the classes from the data file.\n"
	    "--whatif (-w) TODO: UNIMPLEMENTED. Calculate cumulative GPA including future classes to see what the GPA would be if the student performed to a certain level.\n"
	    );
	return EXIT_SUCCESS;
}

/* TODO: refactor */
int
main(int argc, char *argv[])
{
	const char* shortopts = "a:c:g:d:prhw::";
	const struct option longopts[] = {
		{"add", required_argument, NULL, 'a'},
		{"credits", required_argument, NULL, 'c'},
		{"grade", required_argument, NULL, 'g'},
		{"delete", required_argument, NULL, 'd'},
		{"print", no_argument, NULL, 'p'},
		{"reset", no_argument, NULL, 'r'},
		{"help", no_argument, NULL, 'h'},
		{"whatif", optional_argument, NULL, 'w'}
		//{NULL, 0, NULL, 0}
	};

	int retCode;
	FILE* dataFile = fopen("gpat.data", "a+");
	bool addFuncActive = false;
	bool deleteFuncActive = false;
	bool whatifFuncActive = false;
	char* class = NULL;
	char* credits = NULL;
	char* grade = NULL;
	int argument = NULL;

	while ((argument = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		switch(argument) {
		case 'a':
			addFuncActive = true;
			class = optarg;
			break;
		case 'c':
			credits = optarg;
			break;
		case 'g':
			grade = optarg;
			break;
		case 'd':
			deleteFuncActive = true;
			class = optarg;
			break;
		case 'p':
			retCode = print(dataFile);
			break;
		case 'r':
			clearall();
			break;
		case 'h':
			printHelp();
			break;
		case 'w':
			whatifFuncActive = true;
			class = optarg;
			break;
		default:
			printHelp();
		}
	    }

	/*
	 * TODO: Everything below here is spaghetti. Clean this up.
	 */
	if (addFuncActive && deleteFuncActive) {
		printf("You cannot both add and delete at the same time.\n");
		retCode = EXIT_FAILURE;
	}
	else if (addFuncActive && !deleteFuncActive) {
		if (class && grade && credits) {
			if (validGrade(grade)) {
				add(class, credits, grade, dataFile);
				retCode = EXIT_SUCCESS;
			}
			else {
				printf("That is not a valid grade.\n");
				retCode = EXIT_FAILURE;
			}
		}
		else {
			printf("Please call the \"-a\" flag only if you call it with the \"-c\" and \"-g\" flags.\n");
			retCode = EXIT_FAILURE;
		}
	}
	else if (deleteFuncActive && !addFuncActive) {
		delete(class);
		retCode = EXIT_SUCCESS;
	}
	else if (whatifFuncActive) {
		if (class && grade && credits) {
			if (validGrade(grade)) {
				whatif(class, credits, grade, dataFile);
			}
		}
		else {
			retCode = EXIT_SUCCESS;
		}
	}

	fclose(dataFile);
	return retCode;
}
