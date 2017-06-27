#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// TODO: document this mess
char *grade[] = {"F", "D-", "D", "D+", "C-", "C", "C+", "B-", "B", "B+", "A-", "A"};

char* points(char letterGrade[])
{
    char *gradePoints[] = {"0.00", "0.67", "1.00", "1.33", "1.67", "2.00", "2.33", "2.67", "3.00", "3.33", "3.67", "4.00"};
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

int add()
{
    // TODO: file error handling
    char fileInput[10];
    char* gradePoints;
    bool valid;
    FILE* dataFile = fopen("gpat.data", "a");

    printf("Class Name: ");
    scanf("%[^\n]%*c", &fileInput);
    fputs(fileInput, dataFile);
    fputs("|", dataFile);
    printf("Credit Hours: ");
    scanf("%[^\n]%*c", &fileInput);
    fputs(fileInput, dataFile);
    fputs("|", dataFile);
    do {
        printf("Letter Grade: ");
        scanf("%[^\n]%*c", &fileInput);
        valid = validGrade(fileInput);
        if (!valid){
            printf("Invalid grade.\n");
        }
    } while (!valid);
    fputs(fileInput, dataFile);
    fputs("|", dataFile);
    gradePoints = points(fileInput);
    if (gradePoints) {
        fputs(gradePoints,dataFile);
        free(gradePoints);
    }
    fputs("\n", dataFile);

    fclose(dataFile);
    return EXIT_SUCCESS;
}

int del()
{
    // TODO: error checking
    char classToDelete[10];
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

        printf("\nClass to delete: ");
        scanf("%[^\n]%*c", &classToDelete);

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
    // TODO: error handleing
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
    printf("-----------------------------\n commands are case sensitive\n-----------------------------\nprint - print out a list of your classes followed by your cumulative gpa\nadd - add a class\ndelete - delete a class\nclearall - delete the datafile containing your stored classes\nwhatif - a what if report to determine gpa if your were to perform to a certain level\nquit - quit gpat\n\n");
    return EXIT_SUCCESS;
}

int main()
{
    printf("Welcome to gpat! Type \"help\" for a list of valid commands!\n");

    char inputCmd[10];
    char yn;

    while (1) {
        printf("gpat > ");
        scanf("%[^\n]%*c", &inputCmd);

        if (!strcmp(inputCmd,"help")) {
            printHelp();
        } else if (!strcmp(inputCmd,"print")) {
            print();
        } else if (!strcmp(inputCmd,"add")) {
            add();
        } else if (!strcmp(inputCmd,"delete")) {
            del();
        } else if (!strcmp(inputCmd,"clearall")) {
            printf("Are you sure you would like to delete the data file containing your classes and grades? This cannot be undone. (y/n) ");
            scanf("%[^\n]%*c", &inputCmd);
            if (!strcmp(inputCmd,"y")) {
                clearall();
                printf("Data file has successfully been deleted.\n");
            } else {
                printf("Data file has not been deleted.\n");
            }
        } else if (!strcmp(inputCmd,"whatif")) {
            whatif();
        } else if (!strcmp(inputCmd,"quit")) {
            printf("\nThank you for using gpat!\n\n");
            break;
        } else {
            printf("%s is not a valid command. Type \"help\" for a list of valid commands.\n", inputCmd);
        }
    }

    return EXIT_SUCCESS;
}
