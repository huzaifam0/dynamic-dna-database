/*
 * Filename:  main.c
 *
 * This file is used as the main tester file for CIS*1300 Assignment 5.
 * It is provided for you to test your functions. 
 * The tests provided are obviously not comprehensive. Feel free to add additional tests.
 * PS: Do NOT submit this file.
 */

#include "a5.h"

int main(void) {
    int record_count = 0;
    struct DNARecord *database = NULL;
    const char *db_filename = "dna_db.csv";
    
    printf("Loading DNA database from '%s'...\n", db_filename);
    

    // Task 2 Test. Load the database
    database = load_database(db_filename, &record_count);

    if (database == NULL || record_count == 0) {
        printf("Failed to load database or database is empty.\n");
        return 1;
    }

    printf("Successfully loaded %d records.\n\n", record_count);
    printf("--- Running Database Analytics ---\n");


    // Task 3 Test. Find Longest Sequence
    int longest_length = 0;
    char *longest_id = find_longest_sequence(database, record_count, &longest_length);
    if (longest_id != NULL) {
        printf("Record with longest sequence: %s, Longest length: %d\n", longest_id, longest_length);
    }


    // Task 4 Test 1: Count by Classification
    const char *classification_to_find = "Mammal";
    int mammal_count = count_by_classification(database, record_count, classification_to_find);
    printf("Count for classification '%s': %d\n", classification_to_find, mammal_count);

    // Task 4 Test 2: Count by Classification
    classification_to_find = "Reptile";
    int reptile_count = count_by_classification(database, record_count, classification_to_find);
    printf("Count for classification '%s': %d\n", classification_to_find, reptile_count);


    // Task 5 Test: Average Sequence Length
    double avg_len = calculate_average_sequence_length(database, record_count);
    printf("Average sequence length: %.2f\n", avg_len);


    printf("\n--- Running Database Modifications ---\n");


    // Task 6 Test: Add a new record
    const char *new_id = "ID999";
    const char *new_class = "Avian";
    const char *new_seq = "TAGATAGATAGATAG";
    printf("Attempting to add new record: %s... (Current count: %d)\n", new_id, record_count);
    
    int add_success = add_record(&database, &record_count, new_id, new_class, new_seq);
    
    if (add_success) {
        printf("Successfully added record. New count: %d\n", record_count);
        // Optional: Check if the last record is indeed the one we added
        if (strcmp(database[record_count - 1].id, new_id) == 0) {
            printf("Verified: Last record ID is %s.\n", database[record_count - 1].id);
        }
    } else {
        printf("Failed to add record.\n");
    }


    // Task 7 Test 1: Delete an existing record
    const char *id_to_delete = "ID002";
    printf("\nAttempting to delete record: %s... (Current count: %d)\n", id_to_delete, record_count);
    
    int delete_success = delete_record(&database, &record_count, id_to_delete);
    
    if (delete_success) {
        printf("Successfully deleted record. New count: %d\n", record_count);
    } else {
        printf("Failed to delete record (or record not found).\n");
    }

    // Task 7 Test 2: Attempt to delete a non-existent record
    id_to_delete = "ID-XYZ";
    printf("\nAttempting to delete non-existent record: %s... (Current count: %d)\n", id_to_delete, record_count);
    
    delete_success = delete_record(&database, &record_count, id_to_delete);
    
    if (!delete_success) {
        printf("Correctly failed to find and delete record %s. Count remains: %d\n", id_to_delete, record_count);
    } else {
        printf("Error: Function claimed to delete a non-existent record.\n");
    }


    printf("\n--- Memory Cleanup ---\n");


    // Task 8 Test. Free All Allocated Memory 
    // This function must free all individual sequences AND the main array.

    printf("Calling free_database to clean up %d records...\n", record_count);
    
    free_database(database, record_count);
    
    
    printf("Cleanup complete.\n");

    return 0;
}