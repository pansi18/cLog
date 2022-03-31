#include <stdio.h>
#include <string.h>
#include "cLog.h"

void foo();

int main() {
    cLog(CLOG_INFO, "log to stdout");
    foo();
    cLog(CLOG_INFO, "%s %dth log %s", "hello cLog", 1, "info");
    setLogLevel(CLOG_DEBUG);
    cLog(CLOG_DEBUG, "debug log");

    setLogLevel(CLOG_FILE);
    cLog(CLOG_INFO, "log to file");
    cLog(CLOG_FILE, "%s %dth log %s", "hello cLog", 1, "info");
    cLog(CLOG_INFO, "%s %dth log %s", "hello cLog", 2, "info");
    cLog(CLOG_DEBUG, "log to file");
    setLogLevel(CLOG_ERROR);
    cLog(CLOG_ERROR, "log to terminal");
    stopClog();

    return 0;
}

void foo(){
    cLog(CLOG_ERROR, "log to stderr");
}
