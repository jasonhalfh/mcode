#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "cab.h"

/*
 *  For GPS data handle
 *  [lat : lng : c : time]
 */
void gps_to_loc(double lat, double lng, double ox, double oy,
		float *x, float *y)
{
	double tmp_x = (double)(A * A);
	tmp_x /= sqrt(tmp_x + B * B * pow((sin(lat * PI / 180) / cos(lat * PI / 180)), 2));
	
	double tmp_y = (double)(B * B);
	tmp_y /= sqrt(tmp_y + A * A * pow((cos(lat * PI / 180) / sin(lat * PI / 180)), 2));
	
	if(ox == 0 && oy == 0) {
		*x = tmp_x;
		*y = tmp_y;
	} else {
		*x = sqrt((tmp_x - ox) * (tmp_x - ox) + (tmp_y - oy) * (tmp_y - oy));
		*y = ox * (lng - OLNG) * PI /180 ;
	}
}

void gps_to_loc_by_file(char *infile, double ox, double oy,
			struct stand_items *items,
			int min_time, int items_size, int id)
{
	if(infile == NULL || items == NULL) {
		printf("file not exist\n");
		return ;
	}
	FILE *fp = fopen(infile, "r");
	if(fp == NULL) {
		printf("open file error in gps_to_loc\n");
		return;
	}
	
	struct stand_items *p = items;
	double lng, lat;
	int flag, time;
	int cnt = 0;
	while(fscanf(fp,"%lf\t%lf\t%d\t%d\n", &lat, &lng, &flag, &time) != EOF &&
	      cnt < items_size) {
		p->nid = id;
		p->time =  time - min_time;
		gps_to_loc(lat, lng, ox, oy, &(p->x), &(p->y));
		p ++;
		cnt ++;
	}
	fclose(fp);
}

void gps_files_format(char *indir, char *outdir, 
		      struct stand_items *items, 
		      int min_time, int items_size) 
{
	DIR *dp;
	struct dirent *file;
	char infile[256];
	char outfile[256];
	int id = 0;
	float ox = 0.0;
	float oy = 0.0;
	gps_to_loc(OLAT, OLNG, 0, 0, &ox, &oy);
	printf("o point lng: %lf, lat: %lf\n", ox, oy);

	dp = opendir(indir);
        if(!dp) {
                printf("open dir error in density_in_area\n");
		dp = opendir(indir);
	}

	while(file = readdir(dp)) {
		if(file->d_type != 8) { // not a normal file
			continue;
		}
		memset(infile, 0, sizeof(infile));
		memset(outfile, 0, sizeof(outfile));
		strcat(infile, indir);
		strcat(infile, file->d_name);
		strcat(outfile, outdir);
		strcat(outfile, file->d_name);

		memset(items, 0, items_size * sizeof(struct stand_items));
		gps_to_loc_by_file(infile, ox, oy, items, min_time, items_size, id);
		write_file_by_items(outfile, items, items_size);
		id ++;
	}
	closedir(dp);
}


#define PERIOD_TIME 60 //???

void gps_nodes_distribution(char *indir,char *outdir, struct stand_items **items,
			    struct stand_items *ntrack, int time_call, int total)
{
	struct stand_items **p = items;
	DIR *dp;
	struct dirent *file;
	char ab_file[256];
	
	struct stand_items **heads = 
		(struct stand_items**)malloc(total * sizeof(struct items*));
	if (heads == NULL) {
		printf("malloc failed\n");
		return ;
	}
	memset(heads, 0, total * sizeof(struct items*));
	

	struct statistic_info *all =
		(struct statistic_info*)malloc(0xfff * sizeof(struct statistic_info));
	if (all == NULL) {
		printf("malloc failed\n");
		return ;
	} 
	memset(all, 0, 0xfff * sizeof(struct statistic_info));
	
	int j = 0;
	while(j < total) {
		*(heads + j) = *(p + j);
		(all + j)->hours_id = j;
		j ++;
	}

	dp = opendir(indir);
	if(!dp) {
		printf("open dir error in nodes_distribution_at_time\n");
		return ;
	}
	j = 0;
	while(file = readdir(dp)) {
		if(file->d_type != 8) { // not a normal file
			continue;
		}
		
		memset(ntrack, 0, 0x1ffff * sizeof(struct stand_items));
		struct stand_items *tmp = ntrack;
		int time, nid;
		float x,y;    
		memset(ab_file, 0, sizeof(ab_file));  
		strcat(ab_file, indir);   
		strcat(ab_file, file->d_name);  
		import_items(ab_file, ntrack);  
		
		while(tmp->time) {
			time = tmp->time;
			x = tmp->x;
			y = tmp->y;
			nid = tmp->nid;
			int i = 0;
			while(i < total) {
				int now = time_call + i * 60;
				if(time >= now && time < now + PERIOD_TIME) {
					if ((*(p + i) -1)->nid == nid) {
						break;
					}
					(*(p + i))->time = time;
					(*(p + i))->x = x;
					(*(p + i))->y = y;
					(*(p + i))->nid = nid;
					(all + i)->cnt ++; // for statistic
					(*(p + i)) ++;
				}
				i ++;
			}
			tmp ++;
		}
		j++;
		printf("file num: %d\n",j);
	}
	j = 0;
	while(j < total) {
		char str[16];
		memset(ab_file, 0, sizeof(ab_file));  
		strcat(ab_file, outdir);
		sprintf(str, "%d", time_call + j * 60);
		strcat(ab_file, str); 
		p = items;
		if( (all + j)->cnt >= 300) {
			write_file_by_items(ab_file, *(heads + j), 0x400);
		}
		j ++;
	}
	/* get a real node num by time */
	write_file_by_sinfo("statistic_reall.info", all, 0xfff);

	free(all);
	free(heads);
	closedir(dp);
}
