#ifndef FILE_OPERATE 
#define FILE_OPERATE 1

#include "globle.h"

//struct area *import_aploc(char *file_name, struct ap_info *apinfo);

//int import_aptrack(char *file_name, struct  aptrack_items *oitems); 

//void fill_ap_info(struct area *xy, float x, float y);

//void write_file_by_apinfo(char *file, struct ap_info *ap, int max_num);

int import_items(char *file_name, struct  stand_items *items);

void write_file_by_items(char *file, struct stand_items *items, int max_num);

void write_file_by_sinfo(char *file, struct statistic_info *sinfo, int max_num);

void write_file_by_denstiy(char *dir, int time, double density);

#endif
