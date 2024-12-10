#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define MAX_CMD_LEN 128

// A helper function to spell check a specific file
// 'file_name': Name of the file to spell check
// 'dict': A dictionary containing correct words

int spell_check_file(const char *file_name, const dictionary_t *dict) {     
    FILE *file = fopen(file_name, "r");
    
    if (file == NULL) {
        return -1;
    }

    if (dict == NULL) {
        printf ("Dictionary is null\n");
        return 0;
    }

    char line[1024];
    int words_found = 1;

    while (fgets (line, sizeof(line), file)) {                              //reads the file strings line by line
        char *word = strtok (line, " \n");                                  //breaks down string into smaller parts
        while (word) {
            if (!dict_find(dict, word)) {                                   //if not found in dict, print with [X]
                printf("%s[X] ", word);    
                words_found = 0;            
            } else {
                printf("%s ", word);                                        //if found, print out word
            }
            word = strtok(NULL, " \n");                                     //creates new line when null
        }
        printf("\n");
    }

    fclose(file);
    return words_found;
}
    

int main(int argc, char **argv) {
    dictionary_t *dict = create_dictionary();
    char cmd[MAX_CMD_LEN] = {0};

    if (argc > 1) {                                                         //checks if there's at least one command line argument
        dictionary_t* newDict = read_dict_from_text_file(argv[1]);          //first command argument is the dictionary

        if (newDict != NULL) {                                              //frees old dict and assign old dict to new dict 
            dict_free(dict);
            dict = newDict;
            printf("Dictionary successfully read from text file\n");
        } else {
            printf("Failed to read dictionary from text file\n");           //frees the dict and returns -1 to exit out of program
            dict_free(dict);
            return -1;
        }

        if (argv[2] != NULL){                                               //checks if there's a second argument
    
            if (spell_check_file(argv[2], dict) == -1) {                    //if the second argument failed, free the dict and exit out of program
                printf ("Spell check failed \n");
            }

            dict_free (dict);
            return -1;
        }
    }

    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");

    while (1) {
        printf("spell_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

        else if (strcmp("add",cmd) == 0) {
            scanf("%s", cmd); 
            dict_insert(dict, cmd);
        } 

        else if (strcmp("lookup", cmd) == 0) {
            char word[MAX_CMD_LEN] = {0};
            scanf("%s", word);
            if (dict_find (dict, word) == 1) {
                printf("'%s' present in dictionary\n", word);
            } else {
                printf("'%s' not found\n", word);
            }
        }

        else if (strcmp("print", cmd) == 0 ) {
            dict_print(dict);
        } 

        else if (strcmp("load", cmd) == 0) {
            scanf("%s", cmd);
            dictionary_t* newDict = read_dict_from_text_file(cmd);

            if (newDict != NULL) {
                printf ("Dictionary successfully read from text file\n");
                dict_free(dict);
                dict = newDict;
            } else {
                dict_free (newDict);
                printf ("Failed to read dictionary from text file\n");
            }
        }

        else if (strcmp("save", cmd) == 0) {
            scanf("%s", cmd);
            if (write_dict_to_text_file(dict, cmd) == 0){
                printf ("Dictionary successfully written to text file \n");
            } else {
                printf ("Dictionary failed written to text file \n");
            }
        }

        else if (strcmp("check", cmd) == 0) {
            if (scanf("%s", cmd)){}
            if (spell_check_file(cmd, dict) == -1) {
                printf ("Spell check failed \n");
            }
        }

        else {
            printf("Unknown command %s\n", cmd);
        }
    }

    dict_free(dict);
    return 0;
}

