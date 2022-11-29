#ifndef LOGS_H
#define LOGS_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <QDateTime>

#define COMPILE_LOGS

#define DATE_FORMAT "dd.MM.yy hh:mm:ss"

typedef enum errors{
    SUCCESS,
    REQUEST_DATA,
    ERROR_NULL_POINTER,
    ERROR_OPEN,
    ERROR_WRITE,
    ERROR_READ,
    ERROR_SQL_QUERY,
    ERROR_ASK_MEMORY,
    ERROR_UNKNOWN
}ErrorCode;

/**
 * @brief logHex
 * log hexadecimal data
 * @param data
 * @param size
 */
void logHex(unsigned char *data, int size);

/**
 * @brief getDate
 * return the date in format dd.MM.yyyy hh:mm:ss
 * @return
 */
char *getDate(void);


#ifdef COMPILE_LOGS
/*The first argument must be contain the format*/
#define LOG_MSG(msg, ...)                                                                       \
    {                                                                                           \
        fprintf(stdout, "%s:%d %s()\t" msg "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__);  \
    }
#define LOG_HEX(data, size) \
    {                       \
        logHex(data, size); \
    }
#else
#define LOG_MSG(msg, ...)
#define LOG_HEX(data, size)
#endif //COMPILE_LOGS

#endif // LOGS_H
