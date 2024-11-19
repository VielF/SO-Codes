#include "fat16.h"
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <err.h>

/* calculate FAT address */
uint32_t bpb_faddress(struct fat_bpb *bpb)
{
	return bpb->reserved_sect * bpb->bytes_p_sect;
}

/* calculate FAT root address */
uint32_t bpb_froot_addr(struct fat_bpb *bpb)
{
	return bpb_faddress(bpb) + bpb->n_fat * bpb->sect_per_fat * bpb->bytes_p_sect;
}

/* calculate data address */
uint32_t bpb_fdata_addr(struct fat_bpb *bpb)
{
	return bpb_froot_addr(bpb) + bpb->possible_rentries * 32;
}

/* calculate data sector count */
uint32_t bpb_fdata_sector_count(struct fat_bpb *bpb)
{
	return bpb->large_n_sects - bpb_fdata_addr(bpb) / bpb->bytes_p_sect;
}

static uint32_t bpb_fdata_sector_count_s(struct fat_bpb* bpb)
{
	return bpb->snumber_sect - bpb_fdata_addr(bpb) / bpb->bytes_p_sect;
}

/* calculate data cluster count */
uint32_t bpb_fdata_cluster_count(struct fat_bpb* bpb)
{
	uint32_t sectors = bpb_fdata_sector_count_s(bpb);

	return sectors / bpb->sector_p_clust;
}

/*
 * allows reading from a specific offset and writting the data to buff
 * returns RB_ERROR if seeking or reading failed and RB_OK if success
 */
int read_bytes(FILE *fp, unsigned int offset, void *buff, unsigned int len)
{

	if (fseek(fp, offset, SEEK_SET) != 0)
	{
		error_at_line(0, errno, __FILE__, __LINE__, "warning: error when seeking to %u", offset);
		return RB_ERROR;
	}
	if (fread(buff, 1, len, fp) != len)
	{
		error_at_line(0, errno, __FILE__, __LINE__, "warning: error reading file");
		return RB_ERROR;
	}

	return RB_OK;
}

/* read fat16's bios parameter block */
void rfat(FILE *fp, struct fat_bpb *bpb)
{

	read_bytes(fp, 0x0, bpb, sizeof(struct fat_bpb));

	return;
}
