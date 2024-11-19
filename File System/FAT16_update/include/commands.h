#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include "fat16.h"

/*
 * Esta struct encapsula o resultado de find(), carregando informações sobre a
 * busca consigo.
 */
struct far_dir_searchres
{
	struct fat_dir fdir; // Diretório encontrado
	bool          found; // Encontrou algo?
	int             idx; // Index relativo ao diretório de busca
};

/*
 * Esta struct encapsula o resultado de fat16_find_free_cluster()
 *
 */
struct fat16_newcluster_info
{
	uint16_t cluster;
	uint32_t address;
};


/* list files in fat_bpb */
struct fat_dir *ls(FILE *, struct fat_bpb *);


/* move um arquivo da fonte ao destino */
void mv(FILE* fp, char* source, char* dest, struct fat_bpb* bpb);

/* delete the file from the fat directory */
void rm(FILE* fp, char* filename, struct fat_bpb* bpb);

/* copy the file to the fat directory */
void cp(FILE* fp, char* source, char* dest, struct fat_bpb* bpb);

/*
 * Esta função escreve no terminal os conteúdos de um arquivo.
 */
void cat(FILE* fp, char* filename, struct fat_bpb* bpb);

/* helper function: find specific filename in fat_dir */
struct far_dir_searchres find_in_root(struct fat_dir *dirs, char *filename, struct fat_bpb *bpb);

/* Procura cluster vazio */
struct fat16_newcluster_info fat16_find_free_cluster(FILE* fp, struct fat_bpb* bpb);

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

///

/* int write_dir (FILE *, char *, struct fat_dir *); */
/* int write_data(FILE *, char *, struct fat_dir *, struct fat_bpb *); */

#endif
