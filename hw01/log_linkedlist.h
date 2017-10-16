#ifndef LOG_LINKEDLIST_H
#define LOG_LINKEDLIST_H

typedef struct logline {
    char level[20];
    char timestamp[20];
    char message[100];
} logline_t;

typedef struct loglist {
    logline_t line;
    struct loglist *next;
} loglist_t;

void insert_line(loglist_t** list_ref, logline_t* parsed_line);
logline_t* parseLine(char* line); // THIS SHOULD BE CAMELCASE!
void deleteList(loglist_t* l);
void printLines(loglist_t* l);

#endif
