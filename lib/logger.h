#ifndef LOGGING_HEADERS
#define LOGGING_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define LOG_FG_BLACK		"\x1b[31m"
#define LOG_FG_RED     	"\x1b[31m"
#define LOG_FG_GREEN   	"\x1b[32m"
#define LOG_FG_YELLOW  	"\x1b[33m"
#define LOG_FG_BLUE    	"\x1b[34m"
#define LOG_FG_MAGENTA 	"\x1b[35m"
#define LOG_FG_CYAN    	"\x1b[36m"
#define LOG_FG_WHITE  	"\x1b[36m"

#define LOG_BG_BLACK		"\x1b[40m"
#define LOG_BG_RED    	"\x1b[41m"
#define LOG_BG_GREEN  	"\x1b[42m"
#define LOG_BG_YELLOW 	"\x1b[43m"
#define LOG_BG_BLUE   	"\x1b[44m"
#define LOG_BG_MAGENTA	"\x1b[45m"
#define LOG_BG_CYAN   	"\x1b[46m"
#define LOG_BG_WHITE  	"\x1b[47m"

#define LOG_BOLD   			"\x1b[1m"
#define LOG_ITALIC   		"\x1b[3m"
#define LOG_INVERT   		"\x1b[7m"
#define LOG_RESET   		"\x1b[0m"

enum log_level_t { DEBUG, WARN, ERROR, OFF };
typedef enum log_level_t log_level;

void set_log_level(log_level level);
void set_log_output(FILE *level);
void logger(log_level, char *s, ...);

#endif
