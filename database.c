/*
Name: Huzaifa Memon
*/      

#include "database.h"

#define MAX_DNA_LENGTH 5096

//Task 2
/**
 * Function name: *load_database
 * Purpose:
 *   This function reads a database from a CSV file and dynamically allocates memory from it
 *
 * Parameter(s):
 *   *filename - A const char with a pointer that represents the file that needs to be open. 
 *   *record_count - An int with a pointer that represents the number of records.
 *
 * Return Value(s):
 *   A struct pointer to a database or NULL if file can't be open or cannot be allocated. 
 *
 * Side Effect(s):
 *   Changes value of *record_count, and allocates memory.
 */
struct DNARecord *load_database(const char *filename, int *record_count){
    *record_count = 0;

    //open the file
    FILE *fp = fopen(filename, "r");

    //if file fails to open
    if(fp == NULL){
        return NULL;
    }//end of if statement

    //record array 
    char record[MAX_DNA_LENGTH];

    int count = 0;

    //count the number of records
    while(fgets(record, MAX_DNA_LENGTH, fp) != NULL){
        count++;
    }//end of while

    //if no records are found
    if(count == 0){
        fclose(fp);
        *record_count = 0;
        return NULL;
    }//end of if

    //save record count
    *record_count = count;

    //Allocate array of structs
    struct DNARecord *database = malloc(sizeof(struct DNARecord) * count);

    //reopen the file
    fclose(fp);
    fp = fopen(filename, "r");

    //if file can't open
    if(fp == NULL){
        //free all content from database if it fails to open
        free(database);
        return NULL;
    }//end of if statement

    //now using fgets, we will read each line and tokenize
    int i = 0;

    while(fgets(record, MAX_DNA_LENGTH, fp) != NULL){
        //remove the \n if it is last line
        int length = strlen(record);
        if(length > 0 && record[length - 1] == '\n'){
            record[length - 1] = '\0';
        }//if statement

        //parse each line using strtok
        //for ID
        char *token = strtok(record, ",");
        if(token != NULL){
            strncpy(database[i].id, token, sizeof(database[i].id) - 1);
            database[i].id[sizeof(database[i].id) - 1] = '\0';
        }//end of if

        //for classification
        token = strtok(NULL, ",");
        if(token != NULL){
            strncpy(database[i].classification, token, sizeof(database[i].classification) - 1);
            database[i].classification[sizeof(database[i].classification) - 1] = '\0';
        }//end of if

        //for sequence
        token = strtok(NULL, ",");
        if(token != NULL){
            //allocate memory for sequence
            int sequence_length = strlen(token);
            database[i].sequence = malloc(sequence_length + 1);
            if(database[i].sequence != NULL){
                strcpy(database[i].sequence, token);
            }//end of if
        }//end of if

        i++;
    }//end of while loop

    //close the file
    fclose(fp);

    return database;  
}//end of load database function

//Task 3
/**
 * Function name: *find_longest_sequence
 * Purpose:
 *   This function checks a database and determines which sequence has the longest length.
 *
 * Parameter(s):
 *   *database - A struct with a pointer that represents the database.
 *   record_count - An intnthat represents the number of records.
 *   *longest_length - An int with a pointer that represents the longest sequence length
 *
 * Return Value(s):
 *   The index of the ID with the longest sequence length or NULL if the database is empty.
 *
 * Side Effect(s):
 *   Stores the length of a database sequence into *longest_length.
 */
char *find_longest_sequence(struct DNARecord *database, int record_count, int *longest_length){
    //if database is empty
    if(record_count == 0){
        *longest_length = 0;
        return NULL;
    }//end of if statement

    //variable for index of longest length
    int longest_index = 0;
    *longest_length = 0;

    //for loop for looping through every sequence until longest one is found
    for(int i = 0; i < record_count; i++){
        int current_length = strlen(database[i].sequence);

        //update the longest length when a larger one is found
        if (current_length > *longest_length){
            *longest_length = current_length;
            longest_index = i;
        }//end of if statement
    }//end of for loop

    return database[longest_index].id;
}//end of find longest sequence function

//Task 4
/**
 * Function name: count_by_classification
 * Purpose:
 *   This function checks a database and counts how many records match a specific classification.
 *
 * Parameter(s):
 *   *database - A struct with a pointer that represents the database.
 *   record_count - An int that represents the number of records.
 *   *classification - A const char with a pointer that represents a classification.
 *
 * Return Value(s):
 *   The total number of records that match a specific classification.
 *
 * Side Effect(s):
 *   None.
 */
int count_by_classification(struct DNARecord *database, int record_count, const char *classification){
    int total_count = 0;

    //use for loop to loop through the database to find number of classifications
    for(int i = 0; i < record_count; i++){
        //if statement for comparing if 2 strings are equal
        if(strcmp(database[i].classification, classification) == 0){
            total_count++;
        }//end of if
    }//end of for

    return total_count;

}//end of count_by_classification function

//Task 5
/**
 * Function name: calculate_average_sequence_length
 * Purpose:
 *   This function calculates the average length of a sequence in a database.
 *
 * Parameter(s):
 *   *database - A struct with a pointer that represents the database.
 *   record_count - An int that represents the number of records.
 *
 * Return Value(s):
 *   The average length of a sequence, or 0.0 if the database is empty. 
 *
 * Side Effect(s):
 *   None.
 */
double calculate_average_sequence_length(struct DNARecord *database, int record_count){
    //if database is empty
    if(record_count == 0){
        return 0.0;
    }//end of if statement

    double total = 0;

    //for loop for looping through each sequence and adding each one
    for(int i = 0; i < record_count; i++){
        total += strlen(database[i].sequence);
    }//end of for loop

    //average length 
    double average_length = total / record_count;
    return average_length;
}//end of calculate_average_sequence_length function

//Task 6
/**
 * Function name: add_record
 * Purpose:
 *   This function adds a record into a database by expanding an array and allocating memory.
 *
 * Parameter(s):
 *   **db_ptr - A struct with a pointer to a pointer.
 *   *record_count - An int with a pointer that represents the number of records.
 *   *id - A const char with a pointer that represents a record ID
 *   *classification - A const char with a pointer that represents a classification.
 *   *sequence - A const char with a pointer that represents a DNA sequence.
 * Return Value(s):
 *   Returns 1 if successfully added, returns 0 if failed to add.
 * Side Effect(s):
 *   Database is resized, allocates memory, and changes *record_count
 */
int add_record(struct DNARecord **db_ptr, int *record_count, const char *id, const char *classification, const char *sequence){
    //expand array
    struct DNARecord *temp_ptr = realloc(*db_ptr, sizeof(struct DNARecord) * (*record_count + 1));

    //if realloc fails
    if(temp_ptr == NULL){
        return 0;
    }//end of if statement

    //put the pointer into a temporary one
    *db_ptr = temp_ptr;

    //make the record count into i to make it easier to copy.
    int i = *record_count;

    //copy the ID
    strncpy((*db_ptr)[i].id, id, sizeof((*db_ptr)[i].id) - 1);
    (*db_ptr)[i].id[sizeof((*db_ptr)[i].id) - 1] = '\0';

    //copy the classification
    strncpy((*db_ptr)[i].classification, classification, sizeof((*db_ptr)[i].classification) - 1);
    (*db_ptr)[i].classification[sizeof((*db_ptr)[i].classification) - 1] = '\0';

    //allocate memory
    int sequence_length = strlen(sequence);
    (*db_ptr)[i].sequence = malloc(sequence_length + 1);

    //check for malloc failure
    if((*db_ptr)[i].sequence == NULL){
        return 0;
    }//end of if statement

    //copy the sequence string
    strcpy((*db_ptr)[i].sequence, sequence);
    (*record_count)++;

    //if successful
    return 1;
}//end add_record function

//Task 7
/**
 * Function name: delete_record
 * Purpose:
 *   This function deletes the ID of a record from a database by shrinking an array and freeing a sequence, and shifting elements.
 * Parameter(s):
 *   **db_ptr - A struct with a pointer to a pointer.
 *   *record_count - An int with a pointer that represents the number of records.
 *   *id_to_delete - A const char with a pointer that represents the ID to delete
 * Return Value(s):
 *   Returns 1 if successfully removed, returns 0 if failed to remove.
 * Side Effect(s):
 *   Frees sequence, shifts elements, and shrinks array of database.
 */
int delete_record(struct DNARecord **db_ptr, int *record_count, const char *id_to_delete){
    //if database empty
     if(*record_count == 0){
        return 0;
    }//end of if statement

    //-1 if index is not found
    int index_not_found = -1;

    //for loop for looping through database to find the index of the id to delete
    for(int i = 0; i < *record_count; i++){
        if(strcmp((*db_ptr)[i].id, id_to_delete) == 0){
            index_not_found = i;
            break;
        }//end of if 
    }//end of for loop

    //if not found
    if(index_not_found == -1){
        return 0;
    }//end of if statement

    //free the internal sequence of the record being moved
    free((*db_ptr)[index_not_found].sequence);

    //shift all elements 1 down
    for(int j = index_not_found; j < (*record_count) - 1; j++){
        (*db_ptr)[j] = (*db_ptr)[j+1];
    }//end of for

    //decrease count
    (*record_count)--;

    //shrink array
    struct DNARecord *temp_ptr = realloc(*db_ptr, sizeof(struct DNARecord) * (*record_count));
    
    //if realloc succeeded, then update it to the original pointer
    if(temp_ptr != NULL){
        *db_ptr = temp_ptr;
    }//end of if statement

    //return 1 on success
    return 1;

}//end of delete_record

//Task 8
/**
 * Function name: free_database
 * Purpose:
 *   This function frees the database and all memory allocated.
 * Parameter(s):
 *   *database - A struct with a pointer that represents the database.
 *   record_count - An int that represents the number of records.
 * Return Value(s):
 *   None.
 * Side Effect(s):
 *   Frees sequence and database.
 */
void free_database(struct DNARecord *database, int record_count){
    //for loop for looping through database
    for(int i = 0; i < record_count; i++){
        //free each sequence pointer at index i
        free(database[i].sequence);
    }//end of for loop

    //free the database array itself
    free(database);
}//end of for loop

