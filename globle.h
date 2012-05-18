#ifndef _globle_h_
#define _globle_h_ 1

#define INF 0x7FFFFFFF
#define ITEMS_PER_FILE 5000 // ??
#define HOUR_TO_SEC 3600


struct area {
	double min_x;
	double max_x;
	double min_y;
	double max_y;
};


/* info need to be saved 
 * standard trace format tobe handle
 */
struct stand_items {
	int nid;
	int time;
	float x;
	float y;
};


struct grid_map {
	int gridx;
	int gridy;
	int weight;
};


/* 
 * statistics info 
 */
struct statistic_info {
	int hours_id;
	int cnt;
}statistic;

struct variance_info {
	int gid;
	int time;
	double var;
};



enum program_fuc {
	GPS_FORMAT,
	CAB_FORMAT,
	CAB_DISTRI,
	CAB_INFO,
	CAB_DENSITY,
	CONFIG_INFO,
};





#endif
