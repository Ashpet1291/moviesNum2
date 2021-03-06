// CS 344 Assignment 2: Files and Directories
// By Ashley Pettibone
// sources and inspirations
// tutorialspoint for random number generator

//*       gcc --std=gnu99 -o movies_by_year mainProg.c
//*		./movies_by_year

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define PREFIX "movies_"
#define ENDING ".csv"

// keeps track of the total amount of movies in the file
int movieCount=0;


/*
* Movie struct to hold movie data
*/
struct movie {
	char *title;
	int year;
	char *languages;
	double ratingValue;
	struct movie *next;
};


/*
* outputs the text to let the user know the options they can choose form
*/
void listChoicesMain() {
	char choiceOne[] = "1.";
	char choiceTwo[] = "2.";
	
	printf("%s %s \n", choiceOne, "Select file to process");
	printf("%s %s \n", choiceTwo, "Exit the program \n");
	printf("%s", "Enter a choice 1 or 2: ");
}


/*
* I used this code from the example we were given in class
* It creates a movie from the data in selected file and then is called by organizeFile to create a linked list of movies form a file
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;
 
 
    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);	   

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
	currMovie->year = atoi(token);	

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The last token is the rating value
    token = strtok_r(NULL, "\n", &saveptr);
	currMovie->ratingValue = atof(token);


    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* I used this code from the example that the instructor provided us
* organizes the movie data in a linked list
*/
struct movie *parseData(char *filePath)
{
	char *fileProcesses = filePath;
    // Open the specified file for reading only
    FILE *movieFile = fopen(fileProcesses, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;
    
    char trash;
    trash = getline(&currLine, &len, movieFile);
    movieCount = 0;

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        movieCount++;
        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
       
    return head;
}


/*
* process the chosen file
* makes a new directory, prints 1 file for each year a movie was released, in the new directory
* 
*/
void processFile(char *filename) {
	
	struct movie *list = parseData(filename);  
	
	int tempMovieSize = 0;	
	int maxStringSize = 50;
	// arrays to copy movie list into
	int listYears[movieCount];
	char titleList[movieCount][maxStringSize];
	tempMovieSize = movieCount;	
	
	int newlistYears[movieCount];	
	
	int p;
	// copy data in arrays to compare
	while(list != NULL) 
	{
		for(p=0; p<movieCount; p++) {
			listYears[p] = list->year;
			newlistYears[p] = list->year;
			strcpy(titleList[p], list->title);
			list = list->next;
		}		
	}	
	
	// variables for loops
	int i;
	int j;
	int k;
	
	// this gets rid of doubles in the year list, so there is only one entry per year,	
	for (i = 0; i<tempMovieSize; i++) {
		for(j=i+1; j<tempMovieSize; j++) {
			if(listYears[i] == listYears[j]) {
				 for(k=j; k < tempMovieSize; k++) {
                    listYears[k] = listYears[k + 1];
                }             
                tempMovieSize--;               
                j--;
			}
		}		
	} 
    
    char directoryName[50];
    char directoryName2[50];
    int randNum;
	
	// get random number for the directory name	
	randNum = generateRandNum();		
	sprintf(directoryName, "pettiboa.movies.%d", randNum);		
	strcpy(directoryName2, directoryName);

	// to check if the directory was correctly made	
	int status;	
	status = mkdir(directoryName, 0750);		
	if(status != 0) {
		printf("there was a problem making directoriies to: %s\n", directoryName);
		return;
	}

	int g;
	int kr;
	
	char line2[] = "/";
	strcat(directoryName2, line2);
		
	char *str = malloc(10);
    char *string = malloc(10);
    char s1[] = ".txt";
    char *newline = malloc(20);
    char fullDirPath[100];
		
	// write year files in created directory	
	for(g=0; g<tempMovieSize; g++)
    {    	    	
		sprintf(str,"%d", listYears[g]);
		strcat(str, s1);

		// gets full directory path to put new files in
		sprintf(fullDirPath, "%s%s", directoryName2, str);
		// create files
		FILE* fptr = fopen(fullDirPath, "w+");
		//    permissions for directory
    	if(chmod(fullDirPath, 0640) == -1) {
    		printf("There was an error changing permissions");
		} 
			int looper = 0;
			int hold = 0;
			while(looper < tempMovieSize) {
				// loop to make year text files
				for(kr = 0; kr<movieCount; kr++) {
  				
  				if(newlistYears[kr] == listYears[hold])
					// prints titles in files // puts titles in fptr file	
					fprintf(fptr, "%s\n", titleList[kr]);
					looper++;		
				}
				hold++;			
			}
			// print error message if can't create a new file
			if(fptr == NULL) { 
				printf("Error creating file\n"); 
			exit(-1); 
			} 
	fclose(fptr);
		 	
 }
	printf("Created directory with name %s", directoryName);
}


/*
* Finds the largest file and automatically processes it
*/
void findLargestFile() {
	
	// open current working directory
	DIR* currDir = opendir(".");
    struct dirent *aDir;
    off_t st_size;
    struct stat dirStat;
    char entryName[256];
    int size =0;
    
    // while reading the current directory
    while((aDir = readdir(currDir)) != NULL){
    
    char *point = strstr(aDir->d_name, ENDING);
	
	// check if the file starts with "movies_" and also contains ".csv
    if((strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) && (point != NULL)) {

        // If so, get directory name for the current entry
        stat(aDir->d_name, &dirStat);  
          
        if(dirStat.st_size >= size)  {     	
         	size = dirStat.st_size;
        	strcpy(entryName, aDir->d_name);
        }
    }
  	}
 //  }
	// Close the directory
	closedir(currDir);
	printf("Now processing the chosen file named %s \n", entryName);
	processFile(entryName);

}


/*
* finds the smallest file in the directory and automatically processes it
*/
void findSmallestFile() {
	
	// open current working directory
	DIR* currDir = opendir(".");
    struct dirent *aDir;
    off_t st_size;
    struct stat dirStat;
    char entryName[256];
    int size=800000;
	  
    // while reading the current directory
    while((aDir = readdir(currDir)) != NULL){

	char *point1 = strstr(aDir->d_name, ENDING);
	 
    if((strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) && (point1 != NULL)) {
    	
        stat(aDir->d_name, &dirStat);  
          
        if(dirStat.st_size < size)  {     	
         	size = dirStat.st_size;
        	strcpy(entryName, aDir->d_name);
        }
  	}
    }
	// Close the directory
	closedir(currDir);
//	printf("The largest file/directory starting with the prefix \"%s\" in the current directory is %s\n", PREFIX, entryName);
	printf("Now processing the chosen file named for %s\n", entryName);
	// process the biggest file
	processFile(entryName);
}

/*
* finds a file given by the user, and then processes it
* returns and error if the file is not in the current directory
*/
void findFile() {
	
	FILE *userFile;
	char userChoice[70];
	
	
	// tell user to input name of a file
	printf("Enter the complete file name: ");
	
	scanf("%s", userChoice);
	
	// checks if the file exist
    if(userFile = fopen(userChoice, "r")) {
    	fclose(userFile);
   		
    	if (parseData(userChoice) != 0) 
      	printf("Now processing the chosen file named %s \n", userChoice);
      	// process the file given by the user
      	processFile(userChoice);
      
   	} else {
    	printf("%s was not found. Try again \n\n", userChoice);
    	
    // couldn't figure out how to call this function in here, after the error, so just listed all of the second menu options		
	int userChoice;
	
	// prints the choices for the second menu option
	printf("Which file you want to process? \n");
	printf("Enter 1 to pick the largest file \n");
	printf("Enter 2 to pick the smallest file \n");
	printf("Enter 3 to specify the name of a file \n\n");
	printf("Enter a choice from 1 to 3: ");
		
	// get number that user entered
	// check if input is an integer, and is one of the options given, otherwise gives error message
	if(scanf("%d", &userChoice) == 1) {
		if(userChoice > 3) {
        	printf("That is not an integer between 1-3, please Enter a choice from 1 to 3: \n");
        	
        	printf("Which file you want to process? \n");
			printf("Enter 1 to pick the largest file \n");
			printf("Enter 2 to pick the smallest file \n");
			printf("Enter 3 to specify the name of a file \n\n");
			printf("Enter a choice from 1 to 3: ");
    	    	}
    	} else {
        	printf("You did not enter an integer, please Enter a choice from 1 to 3: \n");
        	
			// list choices again
        	printf("Which file you want to process? \n");
			printf("Enter 1 to pick the largest file \n");
			printf("Enter 2 to pick the smallest file \n");
			printf("Enter 3 to specify the name of a file \n\n");
			printf("Enter a choice from 1 to 3: ");
    }
    
    // if user picks option 1, finds the largest file and processes it
    if(userChoice == 1) {
    	findLargestFile();    	
    	printf("\n");
	}
	// if user chooses option 2, gets the smallest file and processes it
	else if(userChoice == 2) {
		findSmallestFile();
		printf("\n");
	}
	// gets a file given by the user then process it
	else if(userChoice == 3) {
		findFile();
		printf("\n");
	}
	else {
		exit(0);
	}   	
    }
}

/*
* Generates random number for directory
*/
int generateRandNum(){
	
	time_t t;
	srand((unsigned)time(&t));
	
	int randomNum = rand() % 99999;
	
	return randomNum;
}


/*
* list second menu options, then decides what to do based on user input
* if user enters wrong choice, will give an error and ask them to input a choice again
*/
void listChoicesTwo() {
	
	int userChoice;
	
	// prints the choices for the second menu option
	printf("Which file you want to process? \n");
	printf("Enter 1 to pick the largest file \n");
	printf("Enter 2 to pick the smallest file \n");
	printf("Enter 3 to specify the name of a file \n\n");
	printf("Enter a choice from 1 to 3: ");
		
	// get number that user entered
	// check if input is an integer, and is one of the options given, otherwise gives error message
	if(scanf("%d", &userChoice) == 1) {
		if(userChoice > 3) {
        	printf("That is not an integer between 1-3, please Enter a choice from 1 to 3: \n");
        	
        	printf("Which file you want to process? \n");
			printf("Enter 1 to pick the largest file \n");
			printf("Enter 2 to pick the smallest file \n");
			printf("Enter 3 to specify the name of a file \n\n");
			printf("Enter a choice from 1 to 3: ");
    	    	}
    	} else {
        	printf("You did not enter an integer, please Enter a choice from 1 to 3: \n");
        	
			// list choices again
        	printf("Which file you want to process? \n");
			printf("Enter 1 to pick the largest file \n");
			printf("Enter 2 to pick the smallest file \n");
			printf("Enter 3 to specify the name of a file \n\n");
			printf("Enter a choice from 1 to 3: ");
    }
    
    // if user picks option 1, finds the largest file and processes it
    if(userChoice == 1) {
    	findLargestFile();    	
    	printf("\n");
	}
	// if user chooses option 2, gets the smallest file and processes it
	else if(userChoice == 2) {
		findSmallestFile();
		printf("\n");
	}
	// gets a file given by the user then process it
	else if(userChoice == 3) {
		findFile();
		printf("\n");
	}
	else {
		exit(0);
	}
}

/*
* To list the first 2 choices
*  users have the option to selct a file to process, which leads them to another menu, or they can exit
*/
void optionActionsMain() {
	int userChoice;
	while(userChoice != 2) 
	{
		listChoicesMain();
		// get number that user entered
		// check if input is an integer, and is one of the options given, otherwise gives error message
		if(scanf("%d", &userChoice) == 1) {
			if(userChoice > 2) {
        	printf("That is not an integer 1 or 2, please Enter a choice 1 or 2 \n");
        	listChoicesMain();
        	}
    	}		
		else {
        	printf("You did not enter an integer, please Enter a choice from 1 to 4: ");
        	// list choices again
        	listChoicesMain();
    	}
    
    // if user picks option 1
    if(userChoice == 1) {
    	printf("\n");
    	listChoicesTwo();
    	printf("\n");
    
	}
	else if(userChoice == 2) {
		exit(0);
		printf("\n");
	}
	}
}



/*
* Where it all starts!
* starts by listing main options
* from there, depending on what user enters, then an action will be made
*/
int main(void)
{
	// list the main options for the user to choose from	
	optionActionsMain();
    
    printf("\n");
	
    return EXIT_SUCCESS;
}
