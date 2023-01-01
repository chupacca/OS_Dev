#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//////////////////////////////////////////////////////
//GLOBAL VARIABLE/////////////////////////////////////
//////////////////////////////////////////////////////
int SIZE = 256;

/////////////////////////////////////////////////////
//PROTOTYPES/////////////////////////////////////////
/////////////////////////////////////////////////////

/**
    Prints the command as a string.
    Parameters:
     theCmds: the array of strings that contain the commands.
     count: the int denoting how many elements are in theCmds.
*/
void printCmd(char** theCmds, int count);

/**
    Prints a line of dashes.
*/
void printLine();

/**
    Gets the cmd from the user to be executed.
    Parameters:
     theCmds: the array of strings that contain the commands.
     whichCmd: the int denoting how many elements are in theCmds.
    Return:
     an integer that returns the count.
*/
int getCmd(char*** theCmds, int whichCmd);

/**
    Executes the commands given by the users.
    Parameters:
     theCmds: the array of strings that contain the commands.
     whichCmd: the int denoting how many elements are in theCmds.
*/
void executeCmd(char** theCmds, int whichCmd);

/**
    Free a double char array.
    Parameters:
     theCmds: the array of strings that contain the commands.
     count: the int denoting how many elements are in theCmds.
*/
void freeDArray(char** theCmds, int count);

/**
    Prints the 80 dashes.
    Parameters:
     theCmds: the array of strings that contain the commands.
     count: the int denoting how many elements are in theCmds.
*/
void printDashes(char** theCmds, int count);

/////////////////////////////////////////////////////
//Methods////////////////////////////////////////////
/////////////////////////////////////////////////////

void printCmd(char **theCmds, int count)
{
    int i;
    for(i = 0; i < count - 2; i++)
    {
        printf("%s", theCmds[i]);
        if(i < count - 3)
        {
            printf(" ");
        }
    }
}

/////////////////////////////////////////////////////////////////

void printLine()
{
	printf("--------------------------------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////

int getCmd(char*** theCmds, int whichCmd)
{
    int count;
    int end;
    char temp;

    (*theCmds)[0] = (char*) calloc(SIZE, sizeof(char));
    printf("mash-%d>", whichCmd);
    scanf("%s", (*theCmds)[0]);

    count = 1;
    end = scanf ("%c", &temp);

    while(temp != '\n' && end == 1)
    {    
        
        *theCmds = (char**) realloc(*theCmds, (++count) * sizeof(char*));
        
        (*theCmds)[count - 1] = (char*) calloc(SIZE, sizeof(char));
        scanf("%s", (*theCmds)[count - 1]);
        end = scanf ("%c", &temp);
    }

    *theCmds = realloc(*theCmds, (++count) * sizeof(char*));
    (*theCmds)[count - 1] = calloc(SIZE, sizeof(char));
    *theCmds = realloc(*theCmds, (++count) * sizeof(char*));

    //the file
    (*theCmds)[count - 1] = "\0";

    return count;
}

/////////////////////////////////////////////////////////////////

void executeCmd(char** theCmds, int whichCmd)
{
    int cmdError;

    if(cmdError = execvp(*theCmds, theCmds))
    {
        printf("[SHELL %d] STATUS CODE == %d\n", whichCmd, cmdError);
    }
}

/////////////////////////////////////////////////////////////////

void freeDArray(char** theCmds, int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        free(theCmds[i]);
    }
    free(theCmds);
}

void printDashes(char** theCmds, int count)
{
    int dashes = 62, i;
    
    for(i = 0; i < count - 2; i++)
    {
        dashes -= strlen(theCmds[i]);
    }

    for(i = 0; i < dashes; i++)
    {
        printf("-");
    }
    printf("\n");
}

///////////////////////////////////////////////////////////
/////////////////////MAIN//////////////////////////////////
///////////////////////////////////////////////////////////

int main()
{
    int status = 1, mainLoopN = 3;

    pid_t pidf;
    int length1;
    char** theScript1 = (char**) malloc(sizeof(char*)); 
    length1 = getCmd(&theScript1, 1);
    //printf("%d\n", length1);
    //printf("%s\n", theScript1[length1 - 1]);

    pid_t pids;
    int length2;
    char** theScript2 = (char**) malloc(sizeof(char*));
    length2 = getCmd(&theScript2, 2);

    pid_t pidt;
    int length3;
    char** theScript3 = (char**) malloc(sizeof(char*));
    length3 = getCmd(&theScript3, 3);

    char file[250] = "file";
    int* fp = &file;

    printf("file>");
    scanf("%s", file);

    theScript1[length1 - 2] = fp;
    theScript2[length2 - 2] = fp;
    theScript3[length3 - 2] = fp;

    strncpy(theScript2[length2 - 2], theScript1[length1 - 2], strlen(theScript1[length1 - 2]));
    theScript2[length2 - 1] = NULL;
    strncpy(theScript3[length3 - 2], theScript1[length1 - 2], strlen(theScript1[length1 - 2]));
    theScript3[length3 - 1] = NULL;

    pidf = fork();

    int t;
    time_t start, end;

    if(pidf == 0)
    {
        printf("-----LAUNCH CMD 1:"); //18
        printCmd(theScript1, length1);
        printDashes(theScript1, length1);
            
        start = clock();

        executeCmd(theScript1, 1);
        
        end = clock();
        t = (end - start);

        printf("Result took: %dms\n", t);
        printLine();

        abort();
    }
    else
    {
        pids = fork();
        if(pids == 0)
        {
            printf("-----LAUNCH CMD 2:");
            printCmd(theScript2, length2);
            printDashes(theScript2, length2);
            
            start = clock();

            executeCmd(theScript2, 2);
            
            end = clock();
            t = (end - start);

            printf("Result took: %dms\n", t);
            printLine();

            abort();
        }
        else
        {
            pidt = fork();
            if(pidt == 0) 
            {
                printf("-----LAUNCH CMD 3:");
                printCmd(theScript3, length3);
                printDashes(theScript3, length3);    
                
                start = clock();

                executeCmd(theScript3, 3);
                
                end = clock();
                t = (end - start);

                printf("Result took: %dms\n", t);
                printLine();

                abort();
            }
        }

        while (mainLoopN > 0)
        {
            wait(&status);
            mainLoopN--;  
        }

        printLine();
        printf("Done waiting on children: %d %d %d\n", pidf, pids, pidt);
        
        freeDArray(theScript1, length1 - 2);
        freeDArray(theScript2, length2 - 2);
        freeDArray(theScript3, length3 - 2);
    }
    
    return 0;
}
