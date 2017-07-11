#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

// TODO: document this mess

char* points(char letterGrade[])
{
    char *gradePoints[] = {"0.00", "0.67", "1.00", "1.33", "1.67", "2.00", "2.33", "2.67", "3.00", "3.33", "3.67", "4.00"};
    //TODO: move grade arrays out of this and the next function and into grade processing function
    char *grade[] = {"F", "D-", "D", "D+", "C-", "C", "C+", "B-", "B", "B+", "A-", "A"};
    char *retPoints = malloc(sizeof(retPoints));
    for (int i = 0; i < 12; i++){
        if (!strcmp(grade[i],letterGrade)) {
            for (int c = 0; c <= 3; c++) { // TODO: refactor, strcpy?
                retPoints[c] = gradePoints[i][c];
            }
            return retPoints;
            break;
        }
    }
}

bool validGrade(char letterGrade[])
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

double calculateGPA(double creditHours[], double gradePoints[], int numberOfGrades) {
    double totalHours;
    double totalGradePoints;

    for (int i = 0; i < numberOfGrades; i++) {
        totalHours += creditHours[i];
        gradePoints[i] = creditHours[i] * gradePoints[i];
        totalGradePoints += gradePoints[i];
    }

    return totalGradePoints / totalHours;
}

int print()
{
    char classes[10][10]; //TODO: limit class name length on entry to avoid overflow
    double creditHours[10];
    char grades[10][3];
    double gradePoints[10];
    FILE* dataFile = fopen("gpat.data","r");
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
        while (fgets(buffer, sizeof buffer, dataFile) != NULL){
            token = strtok(buffer,"|");
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
                token = strtok(NULL,"|");
                tokenc++;
            }
            linec++;
        }


        fclose(dataFile);

        printf("\nClass / Credits / Grade / Points\n\n");
        for (int i = 0; i < linec; i++) {
            printf("%.40s / %.2f / %.2s / %.2f\n", classes[i], creditHours[i], grades[i], gradePoints[i]);
        }

        double gpa = calculateGPA(creditHours, gradePoints, linec);
        printf("\nCumulative GPA: %.2f\n\n", gpa);
    }
    return EXIT_SUCCESS;
}

int add(char* class, char* credits, char* grade)
{
    // TODO: file error handling, check grade validity before using it as argument for this function
    char* gradePoints;
    FILE* dataFile = fopen("gpat.data", "a");

    fputs(class, dataFile);
    fputs("|", dataFile);
    fputs(credits, dataFile);
    fputs("|", dataFile);
    fputs(grade, dataFile);
    fputs("|", dataFile);
    gradePoints = points(grade);
    if (gradePoints) {
        fputs(gradePoints,dataFile);
        free(gradePoints);
    } else {
        return EXIT_FAILURE;
    }
    fputs("\n", dataFile);

    fclose(dataFile);
    return EXIT_SUCCESS;
}

int delete(char* classToDelete)
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

int clearall()
{
    // TODO: error handleing, add sanity check before calling remove
    remove("gpat.data");
    return EXIT_SUCCESS;
}

int whatif()
{
    printf("external whatif\n");
    return EXIT_SUCCESS;
}

int printHelp()
{
    //TODO: update printhelp function to fit new program functionality
    printf("usage: gpat [actions]\n\n"
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

int main(int argc, char *argv[])
{
    const char* shortopts = "a:c:g:d:prhw";
    const struct option longopts[] = {
        {"add", required_argument, NULL, 'a'},
        {"credits", required_argument, NULL, 'c'},
        {"grade", required_argument, NULL, 'g'},
        {"delete", required_argument, NULL, 'd'},
        {"print", no_argument, NULL, 'p'},
        {"reset", no_argument, NULL, 'r'},
        {"help", no_argument, NULL, 'h'},
        {"whatif", no_argument, NULL, 'w'}
        //{NULL, 0, NULL, 0}
    };

    bool addFuncActive = false;
    bool deleteFuncActive = false;
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
            print();
            break;
        case 'r':
            clearall();
            break;
        case 'h':
            printHelp();
            break;
        case 'w':
            printf("TODO: whatif report\n");
            break;
        }
    }

    if (addFuncActive && deleteFuncActive) {
        printf("You cannot both add and delete at the same time.\n");
        return EXIT_SUCCESS;
    } else if (addFuncActive && !deleteFuncActive) {
        if (class && grade && credits) {
            if (validGrade(grade)) {
                add(class, credits, grade);
                return EXIT_SUCCESS;
            } else {
                printf("That is not a valid grade.\n");
                return EXIT_FAILURE;
            }
        } else {
            printf("Please call the \"-a\" flag only if you call it with the \"-c\" and \"-g\" flags.\n");
            return EXIT_FAILURE;
        }
    } else if (deleteFuncActive && !addFuncActive) {
        delete(class);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
