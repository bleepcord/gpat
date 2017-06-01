#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// TODO: document this mess

char *grade[] = {"F", "D-", "D", "D+", "C-", "C", "C+", "B-", "B", "B+", "A-", "A"};

float points(char letterGrade[]) {
    float gradePoints[] = {0.00, 0.67, 1.00, 1.33, 1.67, 2.00, 2.33, 2.67, 3.00, 3.33, 3.67, 4.00};
    for (int i = 0; i < 12; i++){
        if (!strcmp(grade[i],letterGrade)) {
            //printf("%f", gradePoints[i]);
            return gradePoints[i];
            break;
        }
    }
}

bool validGrade(char letterGrade[]) {
  for (int i = 0; i < 12; i++) {
    if (!strcmp(grade[i],letterGrade)) {
      return true;
      break;
    }
  }
  return false;
}

int print() {
  FILE *dataFile = fopen("gpat.data","r");
  char buffer[20];
  if (!dataFile) {
    perror("You do not have any classes stored yet ");
    return EXIT_FAILURE;
  } else {
    while (fgets(buffer, sizeof buffer, dataFile) != NULL){
      printf("%s", buffer);
    }
    fclose(dataFile);
  }
  return 0;
}

int add() {
  // TODO: file error handling
  char fileInput[10];
  float* gradePoints = malloc(sizeof(gradePoints));
  bool valid;
  FILE *dataFile = fopen("gpat.data", "a");

  printf("Class Name: ");
  scanf("%[^\n]%*c", &fileInput);
  fputs(fileInput, dataFile);
  fputs("    ", dataFile);
  printf("Credit Hours: ");
  scanf("%[^\n]%*c", &fileInput);
  fputs(fileInput, dataFile);
  fputs("    ", dataFile);
  do {
    printf("Letter Grade: ");
    scanf("%[^\n]%*c", &fileInput);
    valid = validGrade(fileInput);
    if (!valid){
      printf("Invalid grade.\n");
    }
  } while (!valid);
  fputs(fileInput, dataFile);
  fputs("    ", dataFile);
  *gradePoints = points(fileInput);
  int retErr = fwrite(gradePoints, sizeof(gradePoints) , 1, dataFile);
  fputs("\n", dataFile);

  fclose(dataFile);
  free(gradePoints);
  return 0;
}

int del() {
  printf("external delete\n");
  return 0;
}

int clearall() {
  // TODO: error handleing
  remove("gpat.data");
  return 0;
}

int whatif() {
  printf("external whatif\n");
  return 0;
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
      printf("-----------------------------\n commands are case sensitive\n-----------------------------\nprint - print out a list of your classes followed by your cumulative gpa\nadd - add a class\ndelete - delete a class\nclearall - delete the datafile containing your stored classes\nwhatif - a what if report to determine gpa if your were to perform to a certain level\nquit - quit gpat\n\n");
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
}
