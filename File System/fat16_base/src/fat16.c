#include "fat16.h"
#include <stdlib.h>

/* calculate FAT address */
uint32_t bpb_faddress(struct fat_bpb *bpb){
    return bpb->reserved_sect * bpb->bytes_p_sect;
}

/* calculate FAT root address */
uint32_t bpb_froot_addr(struct fat_bpb *bpb){
    return bpb_faddress(bpb) + bpb->n_fat * bpb->sect_per_fat * bpb->bytes_p_sect;
}

/* calculate data address */
uint32_t bpb_fdata_addr(struct fat_bpb *bpb){
    return bpb_froot_addr(bpb) + bpb->possible_rentries * 32;
}

/* calculate data sector count */
uint32_t bpb_fdata_sector_count(struct fat_bpb *bpb){
   return bpb->large_n_sects - bpb_fdata_addr(bpb) / bpb->bytes_p_sect;
}

/* allows reading from a specific offset and writting the data to buff
 * returns -1 if seeking or reading failed and 0 if success
 */
int read_bytes(FILE *fp, unsigned int offset, void *buff, unsigned int len){
    if (fseek(fp, offset, SEEK_SET != 0)){
        fprintf(stderr, "Error when seeking to %u\n", offset);
        return -1;
    }
    if (fread(buff, 1, len, fp) != len){
        fprintf(stderr, "Error reading file\n");
        return -1;
    }
    return 0;
}

/* read fat16's bios parameter block */
void rfat(FILE *fp, struct fat_bpb *bpb){
    read_bytes(fp, 0x0, bpb, sizeof(*bpb));
}
