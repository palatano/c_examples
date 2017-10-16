#include <stdio.h>
#include "log_linkedlist.h"
#include "sort.h"

// Scans the timestamp, and calculates the total
// number of seconds.
long total_timestamp_seconds(char* timestamp) {
    int year, month, day, hour, minute, second;

    sscanf(timestamp, "%d-%d-%d %d:%d:%d", &year,
            &month, &day, &hour, &minute, &second);

    long total_seconds = year * 31536000 +
        month * 2629743 +
        day * 86400 +
        hour * 3600 +
        minute * 60 + second;
    return total_seconds;
}

// Merge a sorted linked list inlist into resultlist
// and return a pointer to the head of the combined list.
loglist_t* mergeLists(loglist_t* result_list, loglist_t* inlist) {
    loglist_t* sorted_list = NULL;
    
    // Check if either of the lists are NULL. If so, return the other.
    if (result_list == NULL) {
        return inlist;
    }

    if (inlist == NULL) {
        return result_list;
    }

    // Read in the values from the timestamp to get the total number of seconds.
    long total_seconds_first = total_timestamp_seconds(result_list->line.timestamp);
    long total_seconds_second = total_timestamp_seconds(inlist->line.timestamp);

    // We compare which one has the lower number of seconds,
    // which is earlier.
    if (total_seconds_first <= total_seconds_second) {
        sorted_list = result_list;
        sorted_list->next = mergeLists(result_list->next, inlist);
    } else {
        sorted_list = inlist;
        sorted_list->next = mergeLists(result_list, inlist->next);
    }
    return sorted_list;
}

// Sort the provided linked list inlist
// and return a pointer to the head of the sorted list.
loglist_t* sortList(loglist_t* inlist) {
    // Our merge sort algorithm will use a divide
    // and conquer strategy, where it will divide the 
    // list in half, call itself on the two halfs, and
    // then merge them.
    loglist_t* mid = inlist;
    loglist_t* end = inlist->next;

    // Base Case: Check if the argument inlist is of
    // size one or two.
    if (inlist == NULL || inlist->next == NULL) {
        return inlist;
    }

    // Otherwise, let's divide up the list.
    while (end != NULL) {
        // The end pointer's index should be twice as big
        // as the mid's pointer, to effectively get the middle
        // of the list in mid.
        end = end->next; 
        
        if (end != NULL) {
            end = end->next;
            mid = mid->next;
        }

    }

    // Now, we assign the left and right pointers of the list
    loglist_t* left = inlist; // The size will be from 0 to index at mid.
    loglist_t* right = mid->next; // The size will be from index at mid->next to the end.
    mid->next = NULL; // This sets the end of the left list.

    // Merge the lists.
    left = sortList(left);
    right = sortList(right);
    inlist = mergeLists(left, right);
    return inlist;
}
