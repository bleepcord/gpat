#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/*
 * Given a properly formatted data file, this function will parse and print
 * the data while also calculating the cumulative gpa and outputing the result.
 * TODO: check input file for formatting errors.
 *
 * params:
 *     dataFile - a pointer to a properly formatted gpat data file.
 * return:
 *     EXIT_SUCCESS or EXIT_FAILURE appropriately.
 */
int
print(FILE* dataFile)
{
	char classes[10][10]; //TODO: limit class name length on entry to avoid overflow
	double creditHours[10];
	char grades[10][3];
	double gradePoints[10];
	int maxLineSize = 100;
	char buffer[maxLineSize];

	// parse data file and organize tokens
	if (!dataFile) {
		perror("You do not have any classes stored yet ");
		return EXIT_FAILURE;
	} else {
		char* token = NULL;
		int tokenc = 0;
		int linec = 0;
		while (fgets(buffer, sizeof buffer, dataFile) != NULL) {
			token = strtok(buffer,"/");
			while (token != NULL) {
				if (tokenc % 4 == 0) {
					strcpy(classes[linec], token);
				} else if(tokenc % 4 == 1){
					creditHours[linec] = strtold(token, NULL);
				} else if(tokenc % 4 == 2){
					strcpy(grades[linec], token);
				} else if(tokenc % 4 == 3){
					gradePoints[linec] = strtold(token, NULL);
				}
				token = strtok(NULL,"/");
				tokenc++;
			}
			linec++;
		}

		printf("\nClass / Credits / Grade / Points\n\n");
		for (int i = 0; i < linec; i++) {
			printf("%.40s / %.2f / %.2s / %.2f\n", classes[i], creditHours[i], grades[i], gradePoints[i]);
		}

		double *gpa = calculateGPA(creditHours, gradePoints, linec);
		printf("\nCumulative GPA: %.2f\n\n", *gpa);
	}
	return EXIT_SUCCESS;
}


/*
 * This function adds a new entry to a gpat data file.
 * This function will properly format the parameters, and output an entry
 * to the target file.
 *
 * params:
 *     class   - pointer to a string denoting the name of the input class.
 *     credits - pointer to a string denoting the number of credits a
 *               class is worth
 *     grade   - pointer to a string denoting the grade received in the class.
 *     dataFile- pointer to the file which the formatted entry will be output.
 * return:
 *     EXIT_SUCCESS or EXIT_FAILURE appropriately
 */
int
add(char* class, char* credits, char* grade, FILE* dataFile)
{
	// TODO: file error handling, check grade validity before using it as argument for this function
	char* gradePoints = NULL;

	fputs(class, dataFile);
	fputs("/", dataFile);
	fputs(credits, dataFile);
	fputs("/", dataFile);
	fputs(grade, dataFile);
	fputs("/", dataFile);
	gradePoints = points(grade);
	if (gradePoints) {
		fputs(gradePoints,dataFile);
		free(gradePoints);
	} else {
		return EXIT_FAILURE;
	}
	fputs("\n", dataFile);

	return EXIT_SUCCESS;
}


/*
 * This function parses gpat.data and deletes entries which loosely match
 * the input string.
 *
 * params:
 *     classToDelete - pointer to string to be matched.
 * return:
 *     EXIT_SUCCESS or EXIT_FAILURE appropriately.
 */
int
delete(char* classToDelete)
{
	// TODO: error checking
	int maxLineSize = 100;
	FILE* tempFile = fopen("temp.data", "a");
	FILE* dataFile = fopen("gpat.data","r");
	char buffer[maxLineSize];
	int deleteCount = 0;

	if (!dataFile) {
		perror("You do not have any classes stored yet ");
		return EXIT_FAILURE;
	} else {
		int fileLineCount = 0;

		while (fgets(buffer, sizeof buffer, dataFile) != NULL){
			printf("%s", buffer);
			fileLineCount++;
		}
		fclose(dataFile);
		dataFile = fopen("gpat.data", "r");

		while (fgets(buffer, sizeof buffer, dataFile) != NULL){
			if (!strstr(buffer,classToDelete)) {
				fputs(buffer,tempFile);
			} else {
				deleteCount++;
			}
		}

		// delete data file if this function results in an empty file, reset file pointers to NULL
		if (deleteCount == fileLineCount) {
			fclose(dataFile);
			fclose(tempFile);
			dataFile = NULL;
			tempFile = NULL;
			remove("gpat.data");
			remove("temp.data");
		}
	}

	printf("Deleted %d classes.\n", deleteCount);

	// close files if not closed earlier
	if (dataFile) {
		fclose(dataFile);
		remove("gpat.data");
	}
	if (tempFile) {
		fclose(tempFile);
		rename("temp.data", "gpat.data");
	}

	return EXIT_SUCCESS;
}


/*
 * This function deletes gpat.data.
 * TODO: error handleing and sanity check.
 */
int
clearall(void)
{
	remove("gpat.data");
	return EXIT_SUCCESS;
}


/*
 * Start or update a whatif report.
 * A whatif report creates a temporary data file which can be modified
 * separately from the main data file. This allows user to determine what their
 * gpa would be if they were to perform at a certain level in current/future
 * classes.
 *
 * params:
 *     class   - pointer to string denoting class to add.
 *     credits - pointer to string denoting number of credits a class is worth.
 *     grade   - pointer to string denoting grade to test for class.
 *     dataFile- pointer to file to update.
 */
int
whatif(char* class, char* credits, char* grade, FILE* dataFile)
{
	FILE* whatifDataFile = NULL;
	const int maxLineSize = 100;
	const int gradeSize = 3;
	const int pointSize = 2;
	char buffer[maxLineSize];

	/* Copy data file to temp data file. */
	if (whatifDataFile = fopen("whatif.data", "a+")) {
		fseek(whatifDataFile, 0, SEEK_END);
		unsigned int len = ftell(whatifDataFile);
		rewind(whatifDataFile);
		if (len == 0) {
			while (fgets(buffer, sizeof buffer, dataFile) != NULL){
				fputs(buffer, whatifDataFile);
			}
		}
		add(class, credits, grade, whatifDataFile);
	}
	else {
		puts("Unable to read datafile.\n");
		return EXIT_FAILURE;
	}
	print(whatifDataFile);
	fclose(whatifDataFile);
	return EXIT_SUCCESS;
}
