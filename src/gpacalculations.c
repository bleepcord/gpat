#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "gpacalculations.h"

/*
 * This function calculates the number of points user earns for receiving a
 * specific grade in a class.
 *
 * params:
 *     letterGrade - string denoting letter grade received in class
 * return:
 *     retPoints - pointer to string denoting grade points earned in class.
 */
char *
points(char letterGrade[])
{
	char *gradePoints[] = {"0.00", "0.67", "1.00", "1.33", "1.67", "2.00",
			       "2.33", "2.67", "3.00", "3.33", "3.67", "4.00"};
	/*
	 * TODO: move grade arrays out of this and the next function and into grade
	 * processing function
	 */
	char *grade[] = {"F", "D-", "D", "D+", "C-", "C",
			 "C+", "B-", "B", "B+", "A-", "A"};
	char *retPoints = malloc(sizeof(retPoints));
	for (int i = 0; i < 12; i++){
		if (!strcmp(grade[i],letterGrade)) {
			strcpy(retPoints, gradePoints[i]);
			return retPoints;
		}
	}
}


/*
 * This function tests to see if input letter grade is actually a valid grade.
 *
 * params:
 *     letterGrade - string denoting letter grade received in class
 * return:
 *     boolean type - true if grade is valid, false if grade is invalid
 */
bool
validGrade(char letterGrade[])
{
	char *grade[] = {"F", "D-", "D", "D+", "C-", "C", "C+", "B-", "B", "B+", "A-", "A"};
	for (int i = 0; i < 12; i++) {
		if (!strcmp(grade[i],letterGrade)) {
			return true;
			break;
		}
	}
	return false;
}


/*
 * This function calculates a cumulative gpa.
 * Given arrays of credit hours for a class, points earned for a class, and the
 * number of total grades(classes), this function will calculate a gpa score.
 *
 * params:
 *     creditHours[] - array storing credit hours that each class is worth.
 *     gradePoints[] - array storing points earned in each class.
 *     numberOfGrades- int denoting number of grades in calculation.
 * return:
 *     double type - cumulative gpa.
 */
double *
calculateGPA(double creditHours[], double gradePoints[], int numberOfGrades)
{
	double totalHours;
	double totalGradePoints;
	double *gpa = malloc(sizeof(gpa));

	for (int i = 0; i < numberOfGrades; i++) {
		totalHours += creditHours[i];
		gradePoints[i] = creditHours[i] * gradePoints[i];
		totalGradePoints += gradePoints[i];
	}

	*gpa = totalGradePoints / totalHours;
	return gpa;
}
