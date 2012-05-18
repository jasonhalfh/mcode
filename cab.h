#ifndef _cab_h_
#define _cab_h_ 1

#include "globle.h"

#define A 6378136.49
#define B 6356755.00
#define OLAT 37.77328   // first item in [new_ictmuog.txt]
#define OLNG -122.40836  // chose by random
#define PI 3.1415926535897932





struct cab_data {
	double time;
	double lat;
	double lng;
	int flag;
};

void gps_to_loc(double lat, double lng, double ox, double oy,
		float *x, float *y);


void gps_to_loc_by_file(char *infile, double ox, double oy,
			struct stand_items *items,
			int min_time, int items_size, int id);


void gps_files_format(char *indir, char *outdir, 
		      struct stand_items *items, 
		      int min_time, int items_size);


void gps_nodes_distribution(char *indir,char *outdir, struct stand_items **items,
			    struct stand_items *ntrack, int time_call, int total);





#endif
