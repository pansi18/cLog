// A simple tool for logging in C program
// Created by Si Pan on 2022/3/26.
//

#ifndef CLOG_CLOG_H
#define CLOG_CLOG_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
//#include <process.h>
#define PID (int)GetCurrentProcessId()
#define TID GetCurrentThreadId()
#define COLORFUL false
#define path_sep '\\'
#elif linux
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define PID getpid()
#define TID syscall(SYS_gettid)
#define COLORFUL true
#define path_sep '/'
#endif

/* log level */
#define CLOG_INFO 1
#define CLOG_ERROR 2
#define CLOG_DEBUG 3
#define CLOG_FILE 4

/* colorful log */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KRESET "\033[0m"

static int CLOG_LEVEL = 2;
FILE* clog_file = NULL;
char clog_name[100];
static char clog_date[] = "yyyy-mm-dd hh:mm::ss";
static bool colorful_log = COLORFUL, log2file = false;

/**
 * Release the file if setLogLevel(CLOG_FILE) has been called.
 */
void stopClog(){
    if(clog_file != NULL){
        fclose(clog_file);
        clog_file = NULL;

        log2file = false;
        colorful_log = COLORFUL;
    }
}

/**
 * Set the output log levels
 * @param level : CLOG_INFO, CLOG_ERROR, CLOG_DEBUG or CLOG_FILE
 * @attention
 * If level = CLOG_FILE, all logs are output to a file in the current directory with default name \n
 * and you should call stopClog() to release the file
 *
 */
void setLogLevel(const int level){
    CLOG_LEVEL = level;

    if(level == CLOG_FILE){
        time_t raw_time;
        time(&raw_time);
        struct tm* time_info = localtime(&raw_time);
        sprintf(clog_name, "clog_%d%02d%02d%02d%02d%02d.txt", time_info->tm_year, time_info->tm_mon,
                time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
        clog_file = fopen(clog_name, "w");

        log2file = true;
        colorful_log = false;
    }else
        stopClog();
}

/**
 * Get date and time in format: YYYY-MM-DD hh:mm:ss
 */
char* getDateTime(){
    time_t raw_time;
    time(&raw_time);
    struct tm* time_info = localtime(&raw_time);
    sprintf (clog_date,"%4d-%02d-%02d %02d:%02d:%02d",1900 + time_info->tm_year, 1 + time_info->tm_mon,
            time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    return clog_date;
}

/**
 * @brief Get date and time in format: YYYY-MM-DD hh:mm:ss
 * @deprecated The time get by marco __TIME__ is not accurate! \n
 *             Use getDateTime() instead。
 */
char* getDateFromMarco(){
    char month_str[5];
    int month, day, year;
    sscanf(__DATE__, "%s %d %d", month_str, &day, &year);
    const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    month = (int)(strstr(month_names, month_str)-month_names)/3 + 1;

    //static char data[] = "yyyy-mm-dd";
    sprintf(clog_date, "%d-%02d-%02d", year, month, day);

    return clog_date;
}

/**
 * @brief A simple log tool in C program
 * @author Si Pan
 * @usage The syntax of cLog is same to fprintf() except for the first param \n
 *        using setLogLevel() to determine the output log types: \n
 *        CLOG_INFO = 1, CLOG_ERROR = 2, CLOG_DEBUG = 3, CLOG_FILE = 4 \n
 *        Set to LOGFILE so that logs are output to the file
 * @attention
 * You should always call stopClog() in the end
 */
#define cLog(type,info,...) \
    if(type == CLOG_INFO){       \
        if(CLOG_LEVEL >= CLOG_INFO){ \
            fprintf(log2file ? clog_file : stdout, "I[%s %d][%s]%s%s:%d%s > "info"\n", getDateTime(), PID,\
                strrchr(__FILE__, path_sep) + 1, colorful_log ? KBLU:"", __FUNCTION__, __LINE__, colorful_log ? KRESET:"", ##__VA_ARGS__);}}\
    else if(type == CLOG_ERROR){  \
        if(CLOG_LEVEL >= CLOG_ERROR){ \
            fprintf(log2file ? clog_file : stderr, "%sE[%s %d][%s]%s%s:%d%s > "info"\n%s", colorful_log ? KRED:"", getDateTime(), PID,\
                strrchr(__FILE__, path_sep) + 1, colorful_log ? KBLU:"", __FUNCTION__, __LINE__, colorful_log ? KRED:"", ##__VA_ARGS__, colorful_log ? KRESET:"");}}\
    else if(type == CLOG_DEBUG){  \
        if(CLOG_LEVEL >= CLOG_DEBUG){\
            fprintf(log2file ? clog_file : stdout, "%sD[%s %d][%s]%s%s:%d%s > "info"\n%s", colorful_log ? KYEL:"", getDateTime(), PID,\
                strrchr(__FILE__, path_sep) + 1, colorful_log ? KBLU:"", __FUNCTION__, __LINE__, colorful_log ? KYEL:"", ##__VA_ARGS__, colorful_log ? KRESET:"");}}\
    else{\
        fprintf(stderr, "%sE[%s %d][%s]%s%s:%d%s > unknown type\n%s", colorful_log ? KRED:"", getDateTime(), PID,\
            strrchr(__FILE__, path_sep) + 1, colorful_log ? KBLU:"", __FUNCTION__, __LINE__, colorful_log ? KRED:"", colorful_log ? KRESET:"");}
#endif //CLOG_CLOG_H
