/*
*Jose Rodriguez
*BSTSORT ASSIGNMENT 1
*This program asks you to sort the words in an input file and rint *the sorted words to an output file (standard output)
*I affirm that I wrote this program myself without any help from
*any other people or sources from the internet.
*You must use getopt() to parse the command line arguments to *determine the cases.
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


// Create child nodes for struct
typedef struct TreeNode
{
    int count;
    char *string;
    struct TreeNode *leftChild;
    struct TreeNode *rightChild;
}TreeNode;


void Add(char *key, struct TreeNode **node, bool caseSensitiveFlag);
void nodeLocation(struct TreeNode *, FILE *);
void ReadLine(struct TreeNode **root, bool caseSensitiveFlag);
void removeTree(struct TreeNode** root);
int CompareString(const char string1[], const char string2[], bool caseSensitiveFlag);
void searchSpaces(char* input);
bool searchNullInput(const char* input);


int main(int argc, char **argv)
{
    extern char *optarg;       
    extern int optind;
    extern int errno;          
    FILE *inputFile        = stdin; 
    FILE *outputFile       = stdout; 
    int input              = 0;
    int outputFlag         = 0; 
    int inputFlag          = 0;
    bool caseSensitiveFlag = false; 
    char inputFileName[128]; 
    char outputFileName[128];
    char bufferReader[400];   
    TreeNode *root = NULL; // root node holder

    static char usage[] = "Usage: %s bstsort [-c] [-o output_file_name] [input_file_name]\n";

    
    while ((input = getopt(argc,argv, "co:")) != -1)
    {
        
        switch (input)
        {
            //optional c and o inputs
            case 'c':
                caseSensitiveFlag = true; 
                break;
            case 'o':
                strcpy(outputFileName, optarg); 
                outputFlag = 1;
                break;
            case '?':
                
                fprintf(stderr,usage,argv[0]);
                exit(0);
        }
    }

    
    if (optind != argc)
    {
        //take file name if needed
        strcpy(inputFileName, argv[optind]);
        inputFlag = 1; 
    }

    if(outputFlag && inputFlag)
    {
        
        if (inputFileName != NULL)
        {
            errno = 0; 

            inputFile = fopen(inputFileName, "r"); 

            if(!inputFile) 
            {
                fprintf(stderr, "Failed to open requested file: %s\n", strerror(errno));
                exit(errno); 
            }

            // Read file
            while(fgets(bufferReader, 400, inputFile) != NULL)
            {
                searchSpaces(bufferReader); 

                if (!searchNullInput(bufferReader)) 
                {
                    bufferReader[strcspn(bufferReader, "\n")] = '\0'; 
                    Add(bufferReader,  &root, caseSensitiveFlag); 
                }
            }
            fclose(inputFile);
        }

        
        errno = 0;

        FILE *outputFile = fopen(outputFileName, "w"); 

        if(!outputFile) 
        {
            fprintf(stderr, "File writing failed: %s\n", strerror(errno));
            exit(errno); 
        }

        nodeLocation(root, outputFile); 

        fclose(outputFile); 
    }
    else if (outputFlag == 1) 
    {
        ReadLine(&root, caseSensitiveFlag); 

        errno = 0;

        FILE *outputFile = fopen(outputFileName, "w"); 

        if(!outputFile) 
        {
            fprintf(stderr, "Failed to open requested file: %s\n", strerror(errno));
            exit(errno); 
        }

        nodeLocation(root, outputFile); 

        fclose(outputFile); 
    }
    else if (inputFlag == 1) 
    {
        
        if (inputFileName != NULL)
        {
            errno = 0;

            inputFile = fopen(inputFileName, "r"); 

            if(!inputFile) 
            {
                fprintf(stderr, "Failed to open requested file: %s\n", strerror(errno));
                exit(errno); 
            }

            
            while(fgets(bufferReader, 400, inputFile) != NULL)
            {
                bufferReader[strcspn(bufferReader, "\n")] = '\0'; 
                Add(bufferReader,  &root, caseSensitiveFlag); 
            }
            fclose(inputFile); 
        }

        printf("\n"); 

        nodeLocation(root, outputFile); 
    }
    else 
    {
        ReadLine(&root, caseSensitiveFlag); 
        printf("\n"); 
        nodeLocation(root, outputFile); 
    }

    removeTree(&root); 

    return 0;
}

// Compare string inputs
int CompareString(const char *string1, const char *string2, bool caseSensitiveFlag)
{
    int length1 = strlen(string1) + 1; 
    int length2 = strlen(string2) + 1;
    int i;

    char string1Copy[length1]; 
	char string2Copy[length2];

	strcpy(string1Copy,string1); 
	strcpy(string2Copy,string2); 

    
    if (!caseSensitiveFlag)
    {
        for(i = 0; i < length1; i++)
        {
            string1Copy[i] = tolower(string1[i]); 
        }

        for(i = 0; i < length2; i++)
        {
            string2Copy[i] = tolower(string2[i]); 
        }
    }

    i = 0; 

    while(string1Copy[i] != '\0' && (string1Copy[i] == string2Copy[i]))
    {
        i++; 
    }

    if (string1Copy[i] == '\0') 
    {
	    return 0;
    }
    else if (string1Copy[i] < string2Copy[i]) 
    {        
	return -1;
    }
    else
    { 
        return 1;
    }
}


void nodeLocation(struct TreeNode *root, FILE *stream)
{
    if (root != NULL) 
    {
        nodeLocation(root->leftChild, stream); 

        int i;

        for (i = 0; i < root->count; i++) 
        {
            fprintf(stream, "%s\n", root->string); 
        }

        nodeLocation(root->rightChild, stream); 
    }
}


void ReadLine(struct TreeNode **root, bool isCaseSensitive)
{
  char input[129]; 

  printf("\nPlease enter a few words or a sentence: ");

  
  while ((fgets(input, sizeof(input), stdin) != NULL) && (input[0] != '\n'))
  {
      searchSpaces(input); 

      if (!searchNullInput(input)) 
      {
        input[strcspn(input, "\n")] = '\0'; 
        Add(input,  root, isCaseSensitive); 
        printf("\nPlease add another few words or a sentence or Hit enter to sort input: "); 
      }
      else
      {
        printf("\nCannot process input. Please try again or Hit enter to exit: "); 
      }
  }
}


void removeTree(struct TreeNode **root)
{
    if(*root != NULL) 
    {
        removeTree(&(*root)->leftChild);
        removeTree(&(*root)->rightChild);
        free((*root)->string);
        free((*root)); 
    }

}

// Seprarate node added if needed
void Add(char *key, struct TreeNode **node, bool isCaseSensitive)
{
    if(*node == NULL) 
    {
        *node = malloc(sizeof(struct TreeNode)); 
        (*node)->string = malloc(sizeof(char) * strlen(key) + 1); 
        memcpy((*node)->string, key, strlen(key) + 1); 
        (*node)->count = 1; 
        (*node)->leftChild = NULL; 
        (*node)->rightChild = NULL;
    }
    else
    {
        int result;

        result = CompareString(key, (*node)->string, isCaseSensitive);

        if(result < 0)
	{
            Add(key, &(*node)->leftChild, isCaseSensitive);
	}
        else if (result > 0) 
	{
            Add(key, &(*node)->rightChild, isCaseSensitive);
	}
        else
	{
            (*node)->count += 1; 
	}
    }
}


void searchSpaces(char *input)
{
    int i;
    int startingPoint = 0;
    int endingPoint = strlen(input) - 1;

    while(isspace((unsigned char)input[startingPoint])) 
        startingPoint++; 

    
    while((endingPoint >= startingPoint) && isspace((unsigned char) input[endingPoint]))
        endingPoint--;

    for(i = startingPoint; i <= endingPoint; i++)
        input[i - startingPoint] = input[i]; 

    input[i - startingPoint] = '\0';
}


bool searchNullInput(const char *input)
{
    while(*input != '\0') 
    {
        if(!isspace((unsigned char) *input)) 
            return false; 

        input++; 
    }
    return true;
}
