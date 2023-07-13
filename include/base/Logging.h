#ifndef _INTERNAL_LOGGING_H
#define _INTERNAL_LOGGING_H

#pragma warning(disable: 26812)

#include <iostream>

#ifndef _DEBUG

#define INFO(x, ...) (void)0;
#define WARN(x, ...) (void)0;
#define ERROR(x, ...)(void)0;
#define FATAL(x, ...)(void)0;

#else

#define LOG_STR(level, msg) << "[" << level << " | " << __FUNCTION__ << " ] : " << msg << "\n"

#define INFO(x, ...) std::cout LOG_STR("INFO", x);
#define WARN(x, ...) std::cout LOG_STR("WARN", x);
#define ERROR(x, ...) std::cout LOG_STR("ERROR", x);
#define FATAL(x, ...) std::cout LOG_STR("FATAL", x);


#endif




#endif