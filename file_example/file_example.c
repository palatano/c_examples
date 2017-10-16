#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int is_file(char* name) {
    if ((strncmp(name, ".", 1) == 0) || 
        (strncmp(name, "..", 2) == 0)) {
        return 0;
    }
    return 1;
}

void remove_newline(char* str) {
    char *pos;
    if ((pos=strchr(str, '\n')) != NULL) {
        *pos = '\0';
    }
}

void flush_stdin() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void print_file_using_entry(char* dir_name, struct dirent* entry) {
    char file_path[100] = "./";
    
    // To get the file in the right directory, we need to 
    // concatenate a string to get the correct path.
    strcat(file_path, dir_name);
    strcat(file_path, "/");
    strcat(file_path, entry->d_name);
  
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    char single_line[250];

    fprintf(stdout, "\nThe contents for file, %s, are:\n", entry->d_name); 
    while (!feof(file)) {
        if (fgets(single_line, 250, file) == NULL) {
            break;
        }
        fprintf(stdout, "%s", single_line);
    }
    fclose(file);
}

void print_all_files(char* dir_name) {
    DIR* directory = opendir(dir_name);

    // Once we get the directory, we want to get the files in the
    // directory.
    struct dirent *entry;
    while ( (entry = readdir(directory)) != NULL) {
        if (is_file(entry->d_name)) {
            fprintf(stdout, "The filename is: %s\n", entry->d_name);
        }
    }
    closedir(directory);
}

/*
void append_to_file(FILE* out_file, char* dir_name, struct dirent* entry) {
    char file_path[100] = "./";

    strcat(file_path, dir_name);
    strcat(file_path, "/");
    strcat(file_path, entry->d_name);

    char single_line[250];
    FILE* file = fopen(file_path, "r");
    while (!feof(file)) {
        if (fgets(single_line, 250, file) == NULL) {
            break;
        }
        fputs(single_line, out_file);
    }
    fclose(file);
}
*/


void read_file(char* dir_name) {
    char input[250];

    // We need to create the path, so that 
    // the file is written to the directory.
    char file_path[100] = "./";

    strcat(file_path, dir_name);
    strcat(file_path, "/");

    // Ask the user for the file name to print out.
    printf("Enter the filename to print out: ");
    fgets(input, 250, stdin);
    remove_newline(input);
    strcat(file_path, input);
    
    // Open the file and print out the contents.
    FILE* file = fopen(file_path, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    while (!feof(file)) {
        if ((fgets(input, 250, file)) == NULL) {
            break;
        }
        fputs(input, stdout);
    }

    // Don't forget to close the file.
    fclose(file);
}

void write_to_file(char* dir_name) {
    char input[250];

    // We need to create the path, so that 
    // the file is written to the directory.
    char file_path[100] = "./";

    strcat(file_path, dir_name);
    strcat(file_path, "/");

    // First, we need to get the filename.
    printf("Enter the filename as a .txt file: ");
    fgets(input, 250, stdin);
    remove_newline(input);
    strcat(file_path, input);

    // Now, ask for a sentence to write.
    printf("Enter a sentence to write: ");
    fgets(input, 250, stdin);
    
    // Now, open the file given by the file_path.
    FILE* file = fopen(file_path, "w");
    fputs(input, file);

    // Don't forget to close it.
    fclose(file);
    
}


int main(int argc, char** argv) {
    DIR* directory;
    char input[250];

    // We want to check if the directory given is valid.
    
    if (argc != 2) {
        printf("Insufficient amount of arguments.\n");
        return 1;
    }

    char* dir_name = argv[1];
    directory = opendir(dir_name);

    if (directory == NULL) {
        fprintf(stderr, "No directory found.\n");
        return 1;
    }
    closedir(directory);

    
    for ( ;; ) {
        printf("Enter a command [write, read, printfiles, exit]: ");
        fgets(input, 250, stdin);

        if (strncmp(input, "write", 5) == 0) {
            write_to_file(dir_name);
        } else if (strncmp(input, "read", 4) == 0) {
            read_file(dir_name);
        } else if (strncmp(input, "printfiles", 10) == 0) {
            print_all_files(dir_name);
        } else if (strncmp(input, "exit", 4) == 0) {
            break;
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }

    return 0;
}


