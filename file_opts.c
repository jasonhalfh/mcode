#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "file_opts.h"


#ifdef AP_COMPUT

void fill_ap_xy(struct area *xy, float x, float y)
{
	if(x < 1 && y <1)
		return;
	if(x < xy->min_x) {
		xy->min_x = x;
	} else if(x > xy->max_x) {
		xy->max_x = x;
	}
	
	if(y < xy->min_y) {
		xy->min_y = y;
	} else if(y > xy->max_y) {
		xy->max_y = y;
	} 
	
	return ;
}

struct area *import_aploc(char *file_name, struct ap_info *apinfo)
{
	if(apinfo == NULL || file_name == NULL)
		return NULL;
	
	struct area *res = (struct area*)malloc(sizeof(struct area));
	res->min_x = INF;
	res->min_y = INF;
	res->max_x = 0;
	res->max_y = 0;
	
	int cnt = 0;
	FILE *fp = fopen(file_name, "r");
	if(fp == NULL) {
                return NULL;
        }
	
	float floor;
        while(fscanf(fp, "%[^,]%*c%f,%f,%f\n", 
		     apinfo[cnt].apname, 
		     &apinfo[cnt].x, &apinfo[cnt].y, &floor) != EOF) {
		
		fill_ap_xy(res, apinfo[cnt].x, apinfo[cnt].y);
#if 0
		printf("%s\t%f\t%f\t%f\n", 
		       apinfo[cnt].apname,
		       apinfo[cnt].x,
		       apinfo[cnt].y,
		       floor);
#endif
		cnt ++;
        }
        fclose(fp);

	//* this is for "OFF" state in the movement files

	strcpy(apinfo[cnt].apname, "OFF");
	apinfo[cnt].x = 0;
	apinfo[cnt].y = 0;
	
        return res;
}

int import_aptrack(char *file_name, struct aptrack_items *oitems)
{
	int cnt = 0;

        FILE *fp = fopen(file_name, "r");
        if(fp == NULL) {
                return -1;
        }
	
        while(fscanf(fp, "%d\t%s\n",
                     &oitems[cnt].time, oitems[cnt].apname) != EOF &&
	      cnt < ITEMS_PER_FILE) {
                cnt ++;
        }
        fclose(fp);
        return cnt;
}


void write_file_by_apinfo(char *file, struct ap_info *ap, int max_num)
{
	if(!file || !ap)
                return ;

        struct ap_info *p = ap;
        FILE *fp = fopen(file, "w");
        if(fp == NULL) {
                printf("open file error\n");
                return ;
        }

        int i = 0;
        while(i < max_num){
                fprintf(fp,"%f\t%f\n",p->x, p->y);
                p ++;
                i ++;
        }
        fclose(fp);
        return ;

}

#endif



int import_items(char *file_name, struct stand_items *items)
{
        int cnt = 0;

        FILE *fp = fopen(file_name, "r");
        if(fp == NULL) {
                return -1;
        }

        while(fscanf(fp, "%d\t%f\t%f\t%d\n",
                     &items[cnt].time, &items[cnt].x, 
                     &items[cnt].y, &items[cnt].nid) != EOF &&
	      cnt < ITEMS_PER_FILE) {
                cnt ++;
        }
        fclose(fp);
        return cnt;
}

void write_file_by_items(char *file, struct stand_items *items, int max_num)
{
	if(!file || !items) 
		return ;
	
	struct stand_items *p = items;
	FILE *fp = fopen(file, "w");   
	if(fp == NULL) {  
		printf("open file error\n");
		return ;    
	} 
	
	int i = 0;
	while(p->time && i < max_num){
		fprintf(fp,"%d\t%f\t%f\t%d\n",p->time, p->x, p->y, p->nid);
		p ++;
		i ++;
	}
	fclose(fp); 
	return ;

}

void write_file_by_sinfo(char *file, struct statistic_info *sinfo, int max_num)
{
	if(!file || !sinfo)
                return ;

        struct statistic_info *p = sinfo;
        FILE *fp = fopen(file, "w");
        if(fp == NULL) {
                printf("open file error\n");
                return ;
        }

        int i = 0;
        while(i < max_num){
                fprintf(fp,"%d\t%d\n",p->hours_id, p->cnt);
                p ++;
                i ++;
        }
        fclose(fp);
        return ;


}

void write_file_by_density(char *dir, int time, double density)
{      
	char ab_file[256];
	memset(ab_file, 0, sizeof(ab_file)); 
	strcat(ab_file, dir);
	strcat(ab_file, "density"); 
	
        FILE *fp = fopen(ab_file, "a");
        if(fp == NULL) {
                printf("open file error\n");
                return ;
        }
	
	fprintf(fp,"%d\t%lf\n",time, density);
 
        fclose(fp);
        return ;


}
