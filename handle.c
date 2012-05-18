#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>

#include "handle.h"

int get_min_start_time(char *dir, int flag)
{
	DIR *dp;
	struct dirent *file;
	
	int res = INF;
	int time, sflag;
	float x,y;
	char ab_file[256];  // store the new file name   
	char mtfile[256];
	
	struct area a;
	a.min_x = INF;
	a.min_y = INF;
	a.max_x = -1;
	a.max_y = -1;
	
	dp = opendir(dir);
	if(!dp) { 
		printf("open dir error\n");         
		return 1;     
	}          
	int i = 0;
	while(file = readdir(dp)) {   
		if(file->d_type != 8 || 
		   0 == strcmp(file->d_name,"README")) { // not a normal file  
			continue;      
		}  
		memset(ab_file, 0, sizeof(ab_file)); 
		strcat(ab_file, dir);   
		strcat(ab_file, file->d_name);
		// for debug
		//printf("%d %s\n",i, ab_file);
		FILE *fp = fopen(ab_file, "r");
		if(fp == NULL) {
			printf("open file error int get_min_start_time\n");
			continue;
		}
		//printf("%s\n",file->d_name);
		if(flag == GPS_FORMAT ) {
			while(fscanf(fp, "%f\t%f\t%d\t%d\n", &x, &y, &sflag, &time) != EOF) { 
				if(time < res) {
					res = time;
					strcpy(mtfile, ab_file);
				}
			}
			printf("%d\n",i);
			i ++;
			fclose(fp);
		} else {
			while(fscanf(fp, "%d\t%f\t%f\n", &time, &x, &y) != EOF) { 
				if(time < res) {
					res = time;
					strcpy(mtfile, ab_file);
				}
				if(x > a.max_x) 
					a.max_x = x;
				if(x < a.min_x)
					a.min_x = x;
				if(y > a.max_y)
					a.max_y = y;
				if(y < a.min_y)
					a.min_y = y;
			}
			//printf("%d\n", i);
			i ++;
			fclose(fp);
		}
	}
	closedir(dp);
	printf("the min_start_time is %d, in file: %s\n", res, mtfile);
	if(flag != GPS_FORMAT) {
		printf("the area x %f:%f, y %f:%f\n", a.min_x, a.max_x, a.min_y, a.max_y);
	}
	return res;
}


void calc_area(struct area *a, char *dir)
{
	DIR *dp;
	struct dirent *file;
	int time , flag;
	double x,y;
	char ab_file[256], mtfile[256];
	
	a->min_x = INF;
	a->min_y = INF;  
	a->max_x = -1;
	a->max_y = -1;    
	
	dp = opendir(dir);
	if(!dp) {    
		printf("open dir error\n");
		exit(0);
	}    
	
	int i = 0;
	while (file = readdir(dp)) {
		if(file->d_type != 8 ||
		   0 == strcmp(file->d_name,"README")) { // not a normal file 
			continue;  
		} 
		printf("OPEN\n");
		memset(ab_file, 0, sizeof(ab_file)); 
		strcat(ab_file, dir);  
		strcat(ab_file, file->d_name); 
		FILE *fp = fopen(ab_file, "r");  
		if(fp == NULL) {    
			printf("open file error int calc_area\n");
			continue;
		}       
		
		while(fscanf(fp, "%d\t%lf\t%lf\t%d\n", &time, &x, &y, &flag) != EOF) {
			//if(time < res) {
			//	res = time;
			//	strcpy(mtfile, ab_file);
			//}
			if(x > a->max_x) 
				a->max_x = x;
			if(x < a->min_x)
				a->min_x = x;
			if(y > a->max_y)
				a->max_y = y;
			if(y < a->min_y)
				a->min_y = y;	
		}	
		i ++;
		fclose(fp);
		printf("%d\t",i);
	}
	
	closedir(dp);	
}


void calc_density(char *inf, char *of, int gsize)
{
	FILE *fp, *outfp;
	struct grid_map *map;
	int time, nid;
	double x, y;
	int i,j, flag;
	int gx, gy;
	
	
	map = (struct grid_map*)malloc(ITEMS_PER_FILE * sizeof(struct grid_map));
	if (map == NULL) {
		printf("map malloc error\n");
		return;
	}
	memset(map, 0, ITEMS_PER_FILE * sizeof(struct grid_map));
	
	outfp = fopen(of, "w");
	fp = fopen(inf, "r");
	if(fp == NULL || outfp == NULL) { 
		printf("open file error int calc_density\n");  
		return;
	}    
	
	printf("*****\n");
	i = 0;
	while (fscanf(fp, "%d\t%lf\t%lf\t%d\n", &time, &x, &y, &nid) != EOF) {
		/*if (x < 0 ) {
			gx = 0 - ((0-(int)x) % gsize);
		} else 
			gx = (int)x % gsize;
		
		if (y < 0 ) {
			gy = 0 - ((0-(int)y) % gsize);
			printf("gx %d\t", gy);
			gy = (int)y % gsize;
			printf("gx %d\n", gy);
		} else 
			gy = (int)y % gsize;
		*/
		gx = x / gsize;
		gy = y / gsize;
		printf("gx %d\t gy %d\n", gx, gy);
		
		flag = 0;
		j = 0;
		while (j < ITEMS_PER_FILE && map[j].weight > 0) {
			//printf("j:%d\t", j);
			if (map[j].gridx == gx && 
			    map[j].gridy == gy) {
				map[j].weight ++;
				flag = 1;
				break;
			}				
			j ++;
		}
		
		if (flag == 0) {			
			map[i].gridx = gx;
			map[i].gridy = gy;
			map[i].weight ++;
			i ++;
		}		
		//printf("\nmap item %d\n", i);
		
	}
	
	fclose(fp);
	
	i = 0;
	while (map[i].weight > 0 ) {
		fprintf(outfp, "%d\t%d\t%d\n", 
			map[i].gridx, map[i].gridy, map[i].weight);
		i ++;
	}
	
	fclose(outfp);
	free(map);
	printf("grid:%d\n", gsize);
}
