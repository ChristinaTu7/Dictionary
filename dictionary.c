#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

table_t* create_table() {
    table_t *table = malloc(sizeof(table_t));    

    if (table == NULL) {
        return NULL;
    }

    table->length = INITIAL_HASH_TABLE_SIZE;
    table->array = malloc(INITIAL_HASH_TABLE_SIZE * sizeof(list_node_t*));
    
    if (table->array == NULL) {
        free (table);
        return NULL;
    }

    for (int i = 0; i < table->length; i++) {
        table->array[i] = NULL;
    }
    
    return table;
}

dictionary_t* create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));

    if (dict == NULL) {
        return NULL;
    }

    dict->table = create_table();

    if (dict->table == NULL) {
        free (dict);
        return NULL;
    }

    dict->size = 0;
    return dict;
}
    

int hash_code(const char* word) {                 
    unsigned int hash = 0;

    while (*word) {
        hash = hash * 31 + (*word);
        word++;
        }

        return hash;
    }

float load_factor (const dictionary_t* dict) {
    return (float)dict->size / dict->table->length;
}


table_t* resize_table(table_t* orig) {
    size_t new_length = orig->length * 2;                               //resizes new table twice the size
    table_t* new_table = create_table(new_length);

    if (new_table == NULL) {
        return NULL;
    }

    new_table->array = NULL;
    dictionary_t* new_dict = create_dictionary();

    if (new_dict == NULL) {
        free(new_table);
        return NULL;
    }

    new_dict->table = new_table;

    for (unsigned int i = 0; i < orig->length; i++) {                   //iterates through old table and insert words to new resized table
        list_node_t* ptr = orig->array[i];
        while (ptr != NULL) {
            dict_insert(new_dict, ptr->word);
            ptr = ptr->next;
            }
        }

        free(orig);
        return new_table;
    }


int dict_insert(dictionary_t *dict, const char *word) {
    if (dict == NULL || word == NULL) {
        return -1;
    }

    int hashCode = hash_code(word);                                     //gets the hashcode of the word
    int index = hashCode % dict->table->length;                         //find the index where the word is located
    list_node_t *head = dict->table->array[index];                      //create a new list node

    for (list_node_t* ptr = head; ptr!= NULL; ptr = ptr->next) {        //checks if word exists in the dict already
        if (strcmp(ptr->word, word) == 0) {
            printf ("%s already exists", word);
            return -1;
        }
    }

    list_node_t* newNode = (list_node_t*) malloc(sizeof(list_node_t));

    if (newNode == NULL) {
        return -1;
    }

    strcpy (newNode->word, word);
    newNode->next = head;
    dict->table->array[index] = newNode;
    dict->size++;
    return 0;
    }

   
int dict_find(const dictionary_t *dict, const char *query) {
    if (dict == NULL || dict->table == NULL || dict->table->array == NULL) {
        return 0;
    }

    int hashCode = hash_code(query);                                   //calculate hashcode for query
    int index = hashCode % dict->table->length;                        //locate the index within the hash table
    list_node_t *currentNode = dict->table->array[index];              //goes through the linked list at the index

    while (currentNode != NULL) {
        if (strcmp (currentNode->word, query) == 0) {                  //compare current node with query
            return 1;                                                  //found word, return 1
        }
        currentNode = currentNode->next;                               //moves on to the next node
    }

    return 0;                                                          //not found, return 0
}


void dict_print(const dictionary_t *dict) {         
    if (dict == NULL) {
        return;
    }

    for ( int i = 0; i < dict->table->length; i++) {                   //iterate through table
        list_node_t *ptr = dict->table->array[i];                      //get index 
        while (ptr != NULL) {                               
             printf("%s\n",ptr->word);                                 //print out the word
             ptr = ptr->next;                                          //go to next node
        }
    }
}


void dict_free(dictionary_t *dict) {   
    if (dict == NULL) {
        return;
    }               

    for (int i = 0; i < dict->table->length; i++) {                    //iterates through the table of arrays and frees each node
        list_node_t *ptr = dict->table->array[i];
        while (ptr != NULL) {
            list_node_t *next = ptr->next;
            free (ptr);
            ptr = next;
        }
    }

    free (dict->table->array);
    free (dict->table);
    free (dict);
}


dictionary_t *read_dict_from_text_file(const char *file_name) {
    char word[MAX_WORD_LEN];
    dictionary_t *dict_2 = create_dictionary();

    if (dict_2 == NULL) {
        return NULL;
    }

    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        dict_free(dict_2);
        return NULL;
    }

    while (fscanf(file, "%s", word) != EOF) {                          //reads file line by line, if word exist, close file and free new dict
        if (dict_insert(dict_2, word) == -1) {
            fclose(file);
            dict_free(dict_2);
            return NULL;
        }
    }

    fclose (file);
    return (dict_2);
}


int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        return -1;
    }

    for (int i = 0; i < dict->table->length; i++) {                   //increment through dictionary
        list_node_t *ptr = dict->table->array[i];           
        while (ptr != NULL) {                               
            fprintf(file,"%s\n", ptr->word);                          //writes words from dictionary to file
            ptr = ptr->next;                                
        }
    }

    fclose (file);
    return 0;
}
