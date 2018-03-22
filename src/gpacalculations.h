/*
 * gpacalculations.h defines`functions that handle calculations involving gpa
 * and grades.
 */

#ifndef GPACALCULATIONS_HEADER
#define GPACALCULATIONS_HEADER

char * points(char *);
bool   validGrade(char *);
double * calculateGPA(double *, double *, int);

#endif
