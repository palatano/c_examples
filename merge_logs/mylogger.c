#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "log_linkedlist.h"
#include "sort.h"

// Checks if the file has the file_extension ".log".
int isValidLogFile(char* file_name) { 
    char delim = '.';
    char* ext = strrchr(file_name, delim);

    // If this does not have the extension, .log, skip it.
    if (ext == NULL || strncmp(ext, ".log", 4) != 0) {
        return 0;
    }

    // We need to skip the combinedlogs.log file, if it exists.
    char* combined_filename = "combinedlogs.log";
    if (strncmp(file_name, combined_filename,
                strlen(combined_filename)) == 0) {
        return 0;
    }

    return 1;
}

void clean_up(DIR* dir, FILE* file, loglist_t* list) {
    // This simply checks if each argument is not null,
    // and if so, free the memory associated with it.
    if (dir) {
        closedir(dir);
    }
    if (file) {
        fclose(file);
    }
    if (list) {
        deleteList(list);
    }   
}

int hasValidHeader(char* file_name, char* header) {

    // Remove the extension from the file name, and
    // then capitalize that result.
    char* name_without_ext = strtok(file_name, ".");
    if (name_without_ext == NULL) {
        return 0;
    }

    while (*name_without_ext) {
        *name_without_ext =
            toupper((unsigned char) *name_without_ext);
        name_without_ext++;
    }

    // The header must start with #, if not, error.
    if (header[0] != '#') {
        return 0;
    }
    // To get the rest of the header line, simply
    // increment the char pointer.
    header++;

    // Compare the names to see if they're equal,
    // by using the length of the name w/o the extension
    // as the limit of bytes read.
    if (strncmp(name_without_ext,
                header, strlen(name_without_ext)) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {

    // First, we need to check if we are given
    // the sufficient amount of arguments.
    char* directory_path = NULL; // No limit on length of argument, so use pointer.
    if (argc == 1) {
        directory_path = ".";
    } else if (argc == 2) {
        directory_path = argv[1];   
    } else {
        fprintf(stderr,
                "ERROR: Too many arguments, please provide one or zero arguments.\n");
        return 1;
    }

    // We want to remove the trailing "/" in
    // our directory name, if it exists.
    if (strrchr(directory_path, '/') != NULL) {
        sprintf(directory_path, "%s/", directory_path);
    }

    // Now, we need to check if we can access and read
    // the directory.
    DIR* dir = opendir(directory_path);

    // If it exists...
    if (errno == ENOENT || errno == ENOTDIR) {
        fprintf(stderr,
                "ERROR: Directory does not exist. Please enter a valid directory name.\n");
        return 1;
    }

    // If we can read from it...
    if (errno == EACCES) {
        fprintf(stderr,
                "ERROR: Cannot read from directory. Permission denied.\n");
        return 1;
    }

    // I will define an error flag here, to help
    // with cleaning up in case an error is reached.
    int err_flag = 0;

    // Let's begin reading from the directory.
    struct dirent* entry = NULL;
    char file_path[256];
    loglist_t* list = NULL;
    FILE* log_file = NULL;

    while ( (entry = readdir(dir)) != NULL) {

        snprintf(file_path, 256, "%s/%s",
                directory_path, entry->d_name); 

        // Let's check if it is a valid log file.
        if (!isValidLogFile(entry->d_name)) {
            continue;
        } 

        // If so, let's check if we can open and read it.
        log_file = fopen(file_path, "r");
        if (errno == EACCES) {
            fprintf(stderr, "ERROR: Cannot open file. Permission denied\n");
            err_flag = 1;
            break;
        }

        if (log_file == NULL) {
            fprintf(stderr, "ERROR: File does not exist.\n");
            err_flag = 1;
            break;
        }

        // Check if the file has the correct header, as #<file_name>.
        char line[512];
        if (fgets(line, 512, log_file) == NULL) {
            fprintf(stderr, "ERROR: Cannot read from file.\n");
            err_flag = 1;
            break;
        }

        char* file_name = entry->d_name;
        if (!hasValidHeader(file_name, line)) {
            fprintf(stderr, "ERROR: File header does not match file name.\n");    
            err_flag = 1;
            break;
        }

        // If the file can be read and has the valid header, let's begin adding this to
        // our linked list. Check with fgets instead of feof, so that you can avoid
        // an additional newline.
        while ( (fgets(line, 512, log_file)) != NULL) {
            logline_t* log_line = parseLine(line);
            if (log_line == NULL) {
                fprintf(stderr,
                        "ERROR: Current log line, file: %s, is not formatted correctly.\n",
                        file_name);
                err_flag = 1;
                break;
            }

            insert_line(&list, log_line);
        }
        
        // In case, we got an error from fgets, check err_flag.
        if (err_flag) {
            break;
        }
        
        // Don't forget to close the file!
        fclose(log_file);
        log_file = NULL;
    }

    // If we ever broke out of the loop, and err_flag
    // is set, let's clean up.
    if (err_flag) {
        clean_up(dir, log_file, list);
        return 1;
    }

    // Now, let's sort the list.
    loglist_t* sorted_list = sortList(list);
    printLines(sorted_list);

    // Open the file to write to.
    char combined_logfile_path[250];
    char* combined_filename = "combinedlogs.log";

    sprintf(combined_logfile_path, "%s/%s",
            directory_path, combined_filename);

    FILE* combined_logfile = fopen(combined_logfile_path, "w"); // Overwrites any existing file.
    if (combined_logfile == NULL) {
        fprintf(stderr, "ERROR: Can't create and open combinedlogs.log.\n");
        clean_up(dir, log_file, list);
        return 1;
    }

    // Now, we just need to iterate over the list, and write the 
    // logline_t data back to the data.
    loglist_t* curr = sorted_list;
    while (curr != NULL) {
        logline_t line = curr->line;
        fprintf(combined_logfile, "%s,%s,%s", line.level,
               line.timestamp, line.message);
        curr = curr->next; 
    }

    // Don't forget to close the files and delete
    // the list!
    clean_up(dir, log_file, sorted_list);
    fclose(combined_logfile);
    return 0;
}
