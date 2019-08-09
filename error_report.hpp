/*Logs text into a text file with unique name.
 *Tested on May 26, 2019*/

#ifndef ERROR_REPORT_HPP
#define ERROR_REPORT_HPP

#include <stdio.h>

int init_error_log();
char *ascdate();
void close_error_log();

int error_log_number();
const char *error_log_filename();
extern FILE *error_log;

/*check if there is c99 support or newer*/
#if defined __STDC_VERSION__
#if __STDC_VERSION__ >= 199901L
#define log_error(...) \
do { \
	if(error_log_number() == 0){ \
	} \
	else{ \
		error_log = fopen(error_log_filename(), "at"); \
		fprintf(error_log, "[%s] ", ascdate()); \
		fprintf(error_log, __VA_ARGS__); \
		fprintf(error_log, "\n"); \
		fclose(error_log); \
		error_log = NULL; \
	} \
}while(0)
#endif /*#if __STDC_VERSION__ >= 199901L*/

#elif defined __cplusplus
#if __cplusplus >= 199903L
#define log_error(...) \
do { \
	if(error_log_number() == 0){ \
	} \
	else{ \
		error_log = fopen(error_log_filename(), "at"); \
		fprintf(error_log, "[%s] ", ascdate()); \
		fprintf(error_log, __VA_ARGS__); \
		fprintf(error_log, "\n"); \
		fclose(error_log); \
		error_log = NULL; \
	} \
}while(0)
#endif /*#if __cplusplus >= 199903L*/

#else
#define C99_NOT_SUPPORTED
void log_error(const char *str);
#endif /*#ifdef __STDC_VERSION__*/

#endif /*ERROR_REPORT_HPP*/
