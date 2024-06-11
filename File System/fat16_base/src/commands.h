#ifndef COMMANDS_H
#define COMMANDS_H

#include "fat16.h"

/* list files in fat_bpb */
struct fat_dir *ls(FILE *, struct fat_bpb *);

/* write content to directory */
int write_dir (FILE *, char *, struct fat_dir *);

/* write file content to fat directory */
int write_data(FILE *, char *, struct fat_dir *, struct fat_bpb *);

/* move file from source to destination */
void mv(FILE *, char *, struct fat_bpb *);

/* delete the file from the fat directory */
void rm(FILE *, char *, struct fat_bpb *);

/* copy the file to the fat directory */
void cp(FILE *fp, char *filename, struct fat_bpb *bpb);

/* helper function: find specific filename in fat_dir */
struct fat_dir find(struct fat_dir *, char *, struct fat_bpb *);

#endif
