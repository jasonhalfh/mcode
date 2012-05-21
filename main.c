#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "globle.h"
#include "cab.h"

int parse_cmd(int argc, char *argv[])
{
	int res;
	char *sflag = argv[1];
	if (0 == strcmp("-f", sflag)) {
		res = CAB_FORMAT;
	}
	
	if (0 == strcmp("-d", sflag)) {
		res = CAB_DISTRI;
	}
	
	if (0 == strcmp("-i", sflag)) {
		res = CAB_INFO;
	}
	
	if (0 == strcmp("-g", sflag)) {
		res = CAB_DENSITY;
	}

	return res;
}


void gps_format(char *in_dir, char *out_dir) 
{
	int min_time;
	struct stand_items *gps_items = 
		(struct stand_items*)malloc(0x1ffff * sizeof(struct stand_items));
	if (gps_items == NULL) {
		printf("malloc failed\n");
		return ;
	}
	memset(gps_items, 0, 0x1ffff * sizeof(struct stand_items));
	min_time = get_min_start_time(in_dir, GPS_FORMAT);
	
	printf("min start is:%d\n", min_time);
	gps_files_format(in_dir, out_dir, gps_items, min_time, 0x1ffff);
	free(gps_items);
}


void gps_distribute(char *in_dir, char* out_dir,
		    int start_time, int end_time)
{
	struct stand_items *ntrack =
		(struct stand_items*)malloc(0x1ffff * sizeof(struct stand_items));
	int cnt = (end_time - start_time) / 60;
	printf("the during hours calc is: %d\n", cnt);
	struct stand_items **all = 
		(struct stand_items**)malloc(cnt * sizeof(struct stand_items*));
	int i = 0;
	while(i < cnt) {
		*(all + i) = (struct stand_items*)malloc(0x400 * sizeof(struct stand_items));
		memset(*(all+i), 0, 0x400 * sizeof(struct stand_items));
		i ++;
	}
	
	gps_nodes_distribution(in_dir, out_dir, 
			       all, ntrack, start_time, cnt);
	free(all);
	free(ntrack);
}


void area(char *dir)
{
	struct area a;
	calc_area(&a, dir);
	printf("x [%lf : %lf]\n y [%lf : %lf]\n\n", 
	       a.min_x, a.max_x, a.min_y, a.max_y);
}

void density(char *indir, char *outdir, int gsize)
{	
	DIR *dp;
	struct dirent *file;
	char infile[256], outfile[256];
	FILE *fp;
	fp = fopen("all_in_one", "w");
	
	dp = opendir(indir);
	if (!dp) {
		printf("open dir error in density_in_area\n");  
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
		int t = atoi(file->d_name);
		int hour = t/3600;
		int sec = t%3600;
		sprintf(outfile, "%s-%d-%d", outfile, hour, sec);
		//strcat(outfile, file->d_name);
		printf("%s\n", outfile);
		calc_density(infile, outfile, gsize, fp);
	}
	closedir(dp);
	fclose(fp);
}

int main(int argc, char *argv[])
{
	char *in_dir;
	char *out_dir;
	char *in_file;
	char *out_file;
	int start_time, end_time, val;
	int fuc;

	fuc = parse_cmd(argc, argv);
	
	switch (fuc) {
	case CAB_FORMAT:
		in_dir = *(argv + 2);
		out_dir = *(argv + 3);
		gps_format(in_dir, out_dir);
		break;
	case CAB_DISTRI:
		in_dir = *(argv + 2);
		out_dir = *(argv + 3);
		start_time = atoi(*(argv + 4)) * HOUR_TO_SEC;
		end_time = atoi(*(argv + 5))   * HOUR_TO_SEC;   
		gps_distribute(in_dir, out_dir, start_time, end_time);
		break;
	case CAB_INFO:
		in_dir = *(argv + 2);
		area(in_dir);
		break;
		
	case CAB_DENSITY:
		in_dir = *(argv + 2);
		out_dir = *(argv + 3);
		val = atoi(*(argv + 4));
		density(in_dir, out_dir, val);
		break;

	case CONFIG_INFO:
		break;
	default:
		return 0;		
	}
	
	return 0;
}
