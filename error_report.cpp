#include "error_report.hpp"
#include <time.h>

static int number = 0;
static char filename[32];
FILE *error_log = NULL;

int init_error_log(){
	number = time(NULL);
	sprintf(filename, "error_log%d.txt", number);
	
	return 0;
}

int error_log_number(){
	return number;
}
	
const char *error_log_filename(){
	return filename;
}

void close_error_log(){
}

char *ascdate(){
	time_t rawtime;
	struct tm *timeptr;
	static char result[26];
	static const char wday_name[][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	time(&rawtime);
	timeptr = localtime(&rawtime);
	sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
		wday_name[timeptr->tm_wday],
		mon_name[timeptr->tm_mon],
		timeptr->tm_mday, timeptr->tm_hour,
		timeptr->tm_min, timeptr->tm_sec,
		1900 + timeptr->tm_year);
	return result;
}

#ifdef C99_NOT_SUPPORTED
void log_error(const char *str){
	if(number == 0){
	}
	else{
		error_log = fopen(error_log_filename(), "at");
		fprintf(error_log, "[%s] %s\n", ascdate(), str);
		fclose(error_log);
		error_log = NULL;
	}
}
#endif