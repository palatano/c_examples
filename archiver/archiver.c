#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archive_func.h"
#include "archive_info.h"
#include "archive_size.h"
#include "archive_constants.h"

int main(int argc, char** argv) {

    // At least 3 arguments should be supplied.
    if (argc < 3) {
        fprintf(stderr, "ERROR. You must supply a flag and the appropriate arguments.\n");
        return 1;
    }

    // Now, we need to check the flag.
    char* flag = argv[1];
    char* archive_name = argv[2];
    if (strncmp(flag, A_FLAG, 2) == 0) {
     
        // Check the number of arguments.
        if (argc == 3) {
            fprintf(stderr, "ERROR. No files input for archiving.\n");
            return 1;
        }

        // Now, we just pass in the archive name and the files. To
        // get the files to archive, increment the argv double pointer
        // by three, which essentially skips the first three arguments.
        argv = argv + 3;

        // Get the number of files, by subtracting three from argc.
        int numFiles = argc - 3;

        archive(argv, numFiles, archive_name);
    } else if (strncmp(flag, U_FLAG, 2) == 0) {

        if (argc > 3) {
            fprintf(stderr, "ERROR. Only provide the archive name after -u.\n");
            return 1;
        }
        
        // Prepare unarchiving the archive.
        unarchive(archive_name);
    } else if (strncmp(flag, L_FLAG, 2) == 0) {

        // Check the number of arguments.
        if (argc >= 3) {
            fprintf(stderr, "ERROR. Only provide the archive name after -l.\n");
            return 1;
        }
        
        // List out the information of the archive.
        list_archive_info(archive_name);
    } else if (strncmp(flag, V_FLAG, 2) == 0) {
        
        // Check the number of arguments.
        if (argc == 3) {
            fprintf(stderr, "ERROR. No files input for verifying archive.\n");
            return 1;
        }

        // Advance the argv pointer by three, as shown above.
        argv = argv + 3;

        // Get the number of files, by subtracting three from argc.
        int numFiles = argc - 3;

        verify_archive(argv, numFiles, archive_name);
    } else {
        fprintf(stderr, "ERROR. Invalid flag. You must use -a, -u, -l, or -v.");
        return 1;
    }

    return 0;
}
