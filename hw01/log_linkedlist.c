#include "log_linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insert_line(loglist_t** list_ref, logline_t* parsed_line) {
    // Let's check if the head is NULL. If so,
    // use the pointer to assign the head.
    //
    // As shown on the linkedlist example, you need to pass the 
    // pointer the linked list node, so that you can modify
    // the head and have the assignment persist past the local function.
    if ((*list_ref) == NULL) {
        (*list_ref) = malloc(sizeof(loglist_t));
        (*list_ref)->line = *parsed_line;
        (*list_ref)->next = NULL;
        // You need to free this pointer, since the pointer is
        // allocated memory and is now assigned to a loglist_t* node.
        free(parsed_line);
        return;
    }

    // Otherwise, let's find the end of the list and 
    // insert the line there.
    loglist_t* curr = (*list_ref);
    while (curr->next != NULL) {
        curr = curr->next;
    }

    // Link the new node to the end of the list.
    curr->next = malloc(sizeof(loglist_t));
    curr->next->line = *parsed_line;
    curr->next->next = NULL;
    // Free this pointer to prevent memory leaks.
    free(parsed_line);
}

logline_t* parseLine(char* line) {
    char* delim = ",";
    char* tok_1 = strtok(line, delim); // LEVEL
    char* tok_2 = strtok(NULL, delim); // TIMESTAMP
    char* tok_3 = strtok(NULL, delim); // MESSAGE
    char* tok_4 = strtok(NULL, delim); // ADDITIONAL
    
    // To check if the line was parsed correctly, 
    // tok_1, tok_2, tok_3 must not be NULL.
    if (tok_1 == NULL || tok_2 == NULL ||
            tok_3 == NULL) {
        fprintf(stderr, "The line must have at least two commas.\n");
        return NULL;
    }

    // If there is an additional token, that means that there are 
    // more than three commas. I'm assuming this is an error, based
    // on your implementation and the ambiguous constraint on the HW.
    if (tok_4 != NULL) {
        fprintf(stderr,
                "The line is not of the format: <level>, <timestamp>, <message>.\n");
        return NULL;
    }

    // No need to use memset. Calloc will allocate and initialize the 
    // struct and the members! Read more on calloc on linux.die.net.
    logline_t* parsed_line = calloc(1, sizeof(logline_t));

	// Set all buffers in new_line to 0s, this helps prevent
	// null termination error.
    strncpy(parsed_line->level, tok_1, sizeof(parsed_line->level));
    strncpy(parsed_line->timestamp, tok_2, sizeof(parsed_line->timestamp));
    strncpy(parsed_line->message, tok_3, sizeof(parsed_line->message));

    return parsed_line;
}

void deleteList(loglist_t* l) {
    loglist_t* curr = l;
    loglist_t* temp;
    while (curr != NULL) {
        // Get another temporary pointer, to allow the
        // node to be freed without destroying the order
        // of the linkedlist.
        temp = curr;
        curr = curr->next;
        free(temp);
    }
}

void printLines(loglist_t* l) { 
    loglist_t* curr = l;
    while (curr != NULL) {
        // Print out the contents of the logline_t stored
        // in the current loglist_t pointer.
        logline_t line = curr->line;
        printf("%s,%s,%s", line.level, line.timestamp, line.message);
        curr = curr->next;
    }
}

