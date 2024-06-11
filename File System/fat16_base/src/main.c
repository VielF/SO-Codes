#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fat16.h"
#include "commands.h"
#include "output.h"

/* prototypes */
void usage(char *);

/* Show usage help */
void usage(char *executable){
    fprintf(stdout, "Usage:\n");
    fprintf(stdout, "\t%s -h | --help for help\n", executable);
    fprintf(stdout, "\t%s ls <fat16-img> - List files from the FAT16 image\n", executable);
    fprintf(stdout, "\t%s cp <path> <dest> <fat16-img> - Copy files from the image path to local dest.\n", executable);
    fprintf(stdout, "\t%s mv <path> <dest> <fat16-img> - Move files from the path to the FAT16 path\n", executable);
    fprintf(stdout, "\t%s rm <path> <file> <fat16-img> - Remove files from the path to the FAT16 path\n", executable);
    fprintf(stdout, "\n");
    fprintf(stdout, "\tfat16-img needs to be a valid Fat16.\n\n");
}

int main(int argc, char **argv){
    if (argc <= 1){
        usage(argv[0]);
        exit(1);
    }

    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
                strcmp(argv[1], "--help") == 0)){
        usage(argv[0]);
        exit(0);
    }
    else if (argc >= 3 || argc >= 4){
        FILE *fp = fopen(argv[argc - 1], "rb+");
        if (!fp){
            fprintf(stdout, "Could not open file %s\n", argv[2]);
            exit(1);
        }

        struct fat_bpb bpb;
        rfat(fp, &bpb);
        char *command = argv[1];

        if (strcmp(command, "ls") == 0){
            struct fat_dir *dirs = ls(fp, &bpb);
            show_files(dirs);
        }

        if (strcmp(command, "cp") == 0){
            cp(fp, argv[2], &bpb);
            fclose(fp);
        }

        if (strcmp(command, "mv") == 0){
            mv(fp, argv[2], &bpb);
            fclose(fp);
        }
        if (strcmp(command, "rm") == 0){
            rm(fp, argv[2], &bpb);
            fclose(fp);
        }
    }

    return 0;
}
