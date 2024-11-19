#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "commands.h"
#include "fat16.h"
#include "support.h"

#include <errno.h>
#include <err.h>
#include <error.h>
#include <assert.h>

#include <sys/types.h>

/*
 * Função de busca na pasta raíz. Codigo original do professor,
 * altamente modificado.
 *
 * Ela itera sobre todas as bpb->possible_rentries do struct fat_dir* dirs, e
 * retorna a primeira entrada com nome igual à filename.
 */
struct far_dir_searchres find_in_root(struct fat_dir *dirs, char *filename, struct fat_bpb *bpb)
{

	struct far_dir_searchres res = { .found = false };

	for (size_t i = 0; i < bpb->possible_rentries; i++)
	{

		if (dirs[i].name[0] == '\0') continue;

		if (memcmp((char *) dirs[i].name, filename, FAT16STR_SIZE) == 0)
		{
			res.found = true;
			res.fdir  = dirs[i];
			res.idx   = i;
			break;
		}
	}

	return res;
}

/*
 * Função de ls
 *
 * Ela itéra todas as bpb->possible_rentries do diretório raiz
 * e as lê via read_bytes().
 */
struct fat_dir *ls(FILE *fp, struct fat_bpb *bpb)
{

	struct fat_dir *dirs = malloc(sizeof (struct fat_dir) * bpb->possible_rentries);

	for (int i = 0; i < bpb->possible_rentries; i++)
	{
		uint32_t offset = bpb_froot_addr(bpb) + i * sizeof(struct fat_dir);
		read_bytes(fp, offset, &dirs[i], sizeof(dirs[i]));
	}

	return dirs;
}

void mv(FILE *fp, char *source, char* dest, struct fat_bpb *bpb)
{

	char source_rname[FAT16STR_SIZE_WNULL], dest_rname[FAT16STR_SIZE_WNULL];

	/*
	 * Aqui converte-se os nomes dos arquivos, que estão por padrão no formato
	 * C-String, misturado minúsculo/maiusculo, com ponto e delimitado por NULL,
	 * para o formato usado pelo FAT16, tudo maiusculo, ponto implícito, sem
	 * delimitador e tamanho fixo de 8/3.
	 *
	 * Note que o padding() do professor mesmo assim incluí NULL byte, por isso
	 * a diferença de tramanho entre FAT16STR_SIZE v. FAT16STR_SIZE_WNULL.
	 */
	bool badname = cstr_to_fat16wnull(source, source_rname)
	            || cstr_to_fat16wnull(dest,   dest_rname);

	/*
	 * Não foi possivel converter de C-String → FAT16str.
	 */
	if (badname)
	{
		fprintf(stderr, "Nome de arquivo inválido.\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * Aqui usa-se a função para achar o endereço em
	 * disco do começo da lista de diretórios (da pasta raíz).
	 *
	 * Esta informação esta no boot parameter block, que já foi lida fora desta
	 * função (no main()).
	 */
	uint32_t root_address = bpb_froot_addr(bpb);

	/*
	 * Aqui calcula-se o tamanho, em bytes, da estrutura que guarda as entradas
	 * de diretório da pasta raíz.
	 *
	 * bpb->possible_rentries é a quantidade de entradas alocadas em disco para
	 * a pasta raíz (padrão 512 entradas).
	 */
	uint32_t root_size    = sizeof (struct fat_dir) * bpb->possible_rentries;

	/*
	 * Le-se, usando read_bytes() função provida pelo professor, do disco as
	 * entradas de diretório → struct fat_dir root[root_size]
	 */
	struct fat_dir root[root_size];

	if (read_bytes(fp, root_address, &root, root_size) == RB_ERROR)
		error_at_line(EXIT_FAILURE, EIO, __FILE__, __LINE__, "erro ao ler struct fat_dir");

	/*
	 * Então é usado a função find_in_root() para tentar achar alguma entrada
	 * com os nomes source_rname e dest_rname.
	 *
	 * Ela não consegue pesquisar em subdiretórios.
	 */
	struct far_dir_searchres dir1 = find_in_root(&root[0], source_rname, bpb);
	struct far_dir_searchres dir2 = find_in_root(&root[0], dest_rname,   bpb);

	/*
	 * Como regra de negócio, não se deve substituir um arquivo existente com o
	 * comando mv. Falha-se o comando caso dest já exista.
	 */
	if (dir2.found == true)
		error(EXIT_FAILURE, 0, "Não permitido substituir arquivo %s via mv.", dest);

	/*
	 * Obviamente, o arquivo que queremos mover, source, deve já existir.
	 */
	if (dir1.found == false)
		error(EXIT_FAILURE, 0, "Não foi possivel encontrar o arquivo %s.", source);

	/*
	 * Mover o arquivo dentro do mesmo diretório consiste somente em renomeálo.
	 * Copia-se o nome de destino à estrutura fat_dir que achamos via find_int_root().
	 */
	memcpy(dir1.fdir.name, dest_rname, sizeof(char) * FAT16STR_SIZE);

	/*
	 * Onde em disco está dir1
	 */
	uint32_t source_address = sizeof (struct fat_dir) * dir1.idx + root_address;

	/*
	 * Por fim, aplica-se dir1 em disco.
	 */
	(void) fseek (fp, source_address, SEEK_SET);
	(void) fwrite
	(
		&dir1.fdir,
		sizeof (struct fat_dir),
		1,
		fp
	);

	printf("mv %s → %s.\n", source, dest);

	return;
}

void rm(FILE* fp, char* filename, struct fat_bpb* bpb)
{
	char fat16_rname[FAT16STR_SIZE_WNULL];

	// Converte o nome do arquivo para o formato FAT16.
	if (cstr_to_fat16wnull(filename, fat16_rname))
	{
		fprintf(stderr, "Nome de arquivo inválido.\n");
		exit(EXIT_FAILURE);
	}

	// Determina o endereço do diretório raiz e seu tamanho.
	uint32_t root_address = bpb_froot_addr(bpb);
	uint32_t root_size = sizeof(struct fat_dir) * bpb->possible_rentries;

	// Lê o diretório raiz do disco.
	struct fat_dir root[root_size];
	if (read_bytes(fp, root_address, &root, root_size) == RB_ERROR)
	{
		error_at_line(EXIT_FAILURE, EIO, __FILE__, __LINE__, "erro ao ler struct fat_dir");
	}

	// Encontra a entrada do diretório correspondente ao arquivo.
	struct far_dir_searchres dir = find_in_root(&root[0], fat16_rname, bpb);

	// Verifica se o arquivo foi encontrado.
	if (dir.found == false)
	{
		error(EXIT_FAILURE, 0, "Não foi possível encontrar o arquivo %s.", filename);
	}

	// Marca a entrada como livre.
	dir.fdir.name[0] = DIR_FREE_ENTRY; // DIR_FREE_ENTRY é definido como 0xE5 que é o valor que indica que a entrada está livre.

	// Calcula o endereço da entrada do diretório a ser deletada.
	uint32_t file_address = sizeof(struct fat_dir) * dir.idx + root_address;

	// Escreve a entrada atualizada de volta ao disco.
	(void) fseek(fp, file_address, SEEK_SET);
	(void) fwrite(&dir.fdir, sizeof(struct fat_dir), 1, fp);

	/* Após zerar a entrada de diretório, liberar espaço em disco */

	/* Leitura da tabela FAT explicado na função cat() */
	uint32_t fat_address    = bpb_faddress(bpb);
	uint16_t cluster_number = dir.fdir.starting_cluster;
	uint16_t null           = 0x0;
	size_t   count          = 0;

	/* Continua a zerar os clusters até chegar no End Of File */
	while (cluster_number < FAT16_EOF_LO)
	{
		uint32_t infat_cluster_address = fat_address + cluster_number * sizeof (uint16_t);
		read_bytes(fp, infat_cluster_address, &cluster_number, sizeof (uint16_t));

		/* Setar o cluster number como NULL */
		(void) fseek(fp, infat_cluster_address, SEEK_SET);
		(void) fwrite(&null, sizeof (uint16_t), 1, fp);

		count++;
	}

	printf("rm %s, %li clusters apagados.\n", filename, count);

	return;
}

struct fat16_newcluster_info fat16_find_free_cluster(FILE* fp, struct fat_bpb* bpb)
{

	/* Essa implementação de FAT16 não funciona com discos grandes. */
	assert(bpb->large_n_sects == 0);

	uint16_t cluster        = 0x0;
	uint32_t fat_address    = bpb_faddress(bpb);
	uint32_t total_clusters = bpb_fdata_cluster_count(bpb);

	for (cluster = 0x2; cluster < total_clusters; cluster++)
	{
		uint16_t entry;
		uint32_t entry_address = fat_address + cluster * 2;

		(void) read_bytes(fp, entry_address, &entry, sizeof (uint16_t));

		if (entry == 0x0)
			return (struct fat16_newcluster_info) { .cluster = cluster, .address = entry_address };
	}

	return (struct fat16_newcluster_info) {0};
}

void cp(FILE *fp, char* source, char* dest, struct fat_bpb *bpb)
{

	/* Manipulação de diretório explicado em mv() */
	char source_rname[FAT16STR_SIZE_WNULL], dest_rname[FAT16STR_SIZE_WNULL];

	bool badname = cstr_to_fat16wnull(source, source_rname)
	            || cstr_to_fat16wnull(dest,   dest_rname);

	if (badname)
	{
		fprintf(stderr, "Nome de arquivo inválido.\n");
		exit(EXIT_FAILURE);
	}

	uint32_t root_address = bpb_froot_addr(bpb);
	uint32_t root_size = sizeof(struct fat_dir) * bpb->possible_rentries;

	struct fat_dir root[root_size];

	if (read_bytes(fp, root_address, &root, root_size) == RB_ERROR)
		error_at_line(EXIT_FAILURE, EIO, __FILE__, __LINE__, "erro ao ler struct fat_dir");

	struct far_dir_searchres dir1 = find_in_root(root, source_rname, bpb);

	if (!dir1.found)
		error(EXIT_FAILURE, 0, "Não foi possível encontrar o arquivo %s.", source);

	if (find_in_root(root, dest_rname, bpb).found)
		error(EXIT_FAILURE, 0, "Não permitido substituir arquivo %s via cp.", dest);

	struct fat_dir new_dir = dir1.fdir;
	memcpy(new_dir.name, dest_rname, FAT16STR_SIZE);

	/* Dentry */

	bool dentry_failure = true;

	/* Procura-se uma entrada livre no diretório raíz */
	for (int i = 0; i < bpb->possible_rentries; i++) if (root[i].name[0] == DIR_FREE_ENTRY || root[i].name[0] == '\0')
	{

		/* Então calcula-se seu endereço final */
		uint32_t dest_address = sizeof (struct fat_dir) * i + root_address;

		/* Aplica new_dir ao diretório raiz */
		(void) fseek (fp, dest_address, SEEK_SET);
		(void) fwrite
		(
			&new_dir,
			sizeof (struct fat_dir),
			1,
			fp
		);

		dentry_failure = false;

		break;
	}

	if (dentry_failure)
		error_at_line(EXIT_FAILURE, ENOSPC, __FILE__, __LINE__, "Não foi possivel alocar uma entrada no diretório raiz.");

	/* Agora é necessário alocar os clusters para o novo arquivo. */

	int count = 0;

	/* Clusters */
	{

		/*
		 * Informações de novo cluster
		 *
		 * É alocado os clusters de tráz para frente; o último é alocado primeiro,
		 * primariamente devido a necessidade de seu valor ser FAT16_EOF_HI.
		 */
		struct fat16_newcluster_info next_cluster,
		                             prev_cluster = { .cluster = FAT16_EOF_HI };

		/* Quantos clusters o arquivo necessita */
		uint32_t cluster_count = dir1.fdir.file_size / bpb->bytes_p_sect / bpb->sector_p_clust + 1;

		/* Aloca-se os clusters, gravando-os na FAT */
		while (cluster_count--)
		{
			prev_cluster = next_cluster;
			next_cluster = fat16_find_free_cluster(fp, bpb); /* Busca-se novo cluster */

			if (next_cluster.cluster == 0x0)
				error_at_line(EXIT_FAILURE, EIO, __FILE__, __LINE__, "Disco cheio (imagem foi corrompida)");

			(void) fseek (fp, next_cluster.address, SEEK_SET);
			(void) fwrite(&prev_cluster.cluster, sizeof (uint16_t), 1, fp);

			count++;
		}

		/* Ao final, o cluster de início é guardado na entrada de diretório. */
		new_dir.starting_cluster = next_cluster.cluster;
	}

	/* Copy */
	{

		/* Iteração de clusters explicado em cat() */
		const uint32_t fat_address       = bpb_faddress(bpb);
		const uint32_t data_region_start = bpb_fdata_addr(bpb);
		const uint32_t cluster_width     = bpb->bytes_p_sect * bpb->sector_p_clust;

		size_t bytes_to_copy = new_dir.file_size;

		/* É iterado ambos arquivo fonte e arquivo destino ao mesmo tempo */

		uint16_t source_cluster_number = dir1.fdir.starting_cluster;
		uint16_t destin_cluster_number = new_dir .starting_cluster;

		while (bytes_to_copy != 0)
		{

			uint32_t source_cluster_address = (source_cluster_number - 2) * cluster_width + data_region_start;
			uint32_t destin_cluster_address = (destin_cluster_number - 2) * cluster_width + data_region_start;

			size_t copied_in_this_sector = MIN(bytes_to_copy, cluster_width);

			char filedata[cluster_width];

			/* Le-se da fonte e escreve-se no destino. */
			(void) read_bytes(fp, source_cluster_address, filedata, copied_in_this_sector);
			(void) fseek     (fp, destin_cluster_address, SEEK_SET);
			(void) fwrite    (filedata, sizeof (char), copied_in_this_sector, fp);

			bytes_to_copy -= copied_in_this_sector;

			uint32_t source_next_cluster_address = fat_address + source_cluster_number * sizeof (uint16_t);
			uint32_t destin_next_cluster_address = fat_address + destin_cluster_number * sizeof (uint16_t);

			(void) read_bytes(fp, source_next_cluster_address, &source_cluster_number, sizeof (uint16_t));
			(void) read_bytes(fp, destin_next_cluster_address, &destin_cluster_number, sizeof (uint16_t));
		}
	}

	printf("cp %s → %s, %i clusters copiados.\n", source, dest, count);

	return;
}

void cat(FILE* fp, char* filename, struct fat_bpb* bpb)
{

	/*
	 * Leitura do diretório raiz explicado em mv().
	 */

	char rname[FAT16STR_SIZE_WNULL];

	bool badname = cstr_to_fat16wnull(filename, rname);

	if (badname)
	{
		fprintf(stderr, "Nome de arquivo inválido.\n");
		exit(EXIT_FAILURE);
	}

	uint32_t root_address = bpb_froot_addr(bpb);
	uint32_t root_size    = sizeof (struct fat_dir) * bpb->possible_rentries;

	struct fat_dir root[root_size];

	if (read_bytes(fp, root_address, &root, root_size) == RB_ERROR)
		error_at_line(EXIT_FAILURE, EIO, __FILE__, __LINE__, "erro ao ler struct fat_dir");

	struct far_dir_searchres dir = find_in_root(&root[0], rname, bpb);

	if (dir.found == false)
		error(EXIT_FAILURE, 0, "Não foi possivel encontrar o %s.", filename);

	/*
	 * Descobre-se quantos bytes o arquivo tem
	 */
	size_t   bytes_to_read     = dir.fdir.file_size;

	/*
	 * Endereço da região de dados e da tabela de alocação.
	 */
	uint32_t data_region_start = bpb_fdata_addr(bpb);
	uint32_t fat_address       = bpb_faddress(bpb);

	/*
	 * O primeiro cluster do arquivo esta guardado na struct fat_dir.
	 */
	uint16_t cluster_number    = dir.fdir.starting_cluster;

	const uint32_t cluster_width = bpb->bytes_p_sect * bpb->sector_p_clust;

	while (bytes_to_read != 0)
	{

		/* read */
		{

			/* Onde em disco está o cluster atual */
			uint32_t cluster_address     = (cluster_number - 2) * cluster_width + data_region_start;

			/* Devemos ler no máximo cluster_width. */
			size_t read_in_this_sector = MIN(bytes_to_read, cluster_width);

			char filedata[cluster_width];

			/* Lemos o cluster atual */
			read_bytes(fp, cluster_address, filedata, read_in_this_sector);
			printf("%.*s", (signed) read_in_this_sector, filedata);

			bytes_to_read -= read_in_this_sector;
		}

		/*
		 * Calculamos o endereço, na tabela de alocação, de onde está a entrada
		 * que diz qual é o próximo cluster.
		 */
		uint32_t next_cluster_address = fat_address + cluster_number * sizeof (uint16_t);

		/* Lemos esta entrada, assim descobrindo qual é o próximo cluster. */
		read_bytes(fp, next_cluster_address, &cluster_number, sizeof (uint16_t));

	}

	return;
}