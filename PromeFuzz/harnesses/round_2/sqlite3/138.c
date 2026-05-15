// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vmprintf at sqlite3.c:19324:18 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:18175:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_config at sqlite3.c:171315:16 in sqlite3.h
// sqlite3_log at sqlite3.c:19443:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_log_callback(void *pArg, int iErrCode, const char *zMsg) {
    // Do nothing - just a dummy callback for sqlite3_log
    (void)pArg;
    (void)iErrCode;
    (void)zMsg;
}

static char *fuzz_vmprintf_wrapper(const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    char *result = sqlite3_vmprintf(format, ap);
    va_end(ap);
    return result;
}

static void fuzz_str_vappendf_wrapper(sqlite3_str *str, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    sqlite3_str_vappendf(str, format, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create a dummy database connection for sqlite3_str
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Ensure we have some data to work with
    if (Size < 1) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a null-terminated format string from input
    size_t format_len = Size > 100 ? 100 : Size;
    char *format = (char *)malloc(format_len + 1);
    if (!format) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(format, Data, format_len);
    format[format_len] = '\0';
    
    // Create a dummy argument string
    const char *arg_str = "fuzz_argument";
    int arg_int = (int)Size;
    
    // 1. Test sqlite3_snprintf
    char buffer[256];
    sqlite3_snprintf(sizeof(buffer), buffer, format, arg_str, arg_int);
    
    // 2. Test sqlite3_mprintf
    char *mprintf_result = sqlite3_mprintf(format, arg_str, arg_int);
    if (mprintf_result) {
        sqlite3_free(mprintf_result);
    }
    
    // 3. Test sqlite3_vmprintf
    char *vmprintf_result = fuzz_vmprintf_wrapper(format, arg_str, arg_int);
    if (vmprintf_result) {
        sqlite3_free(vmprintf_result);
    }
    
    // 4. & 5. Test sqlite3_str_appendf and sqlite3_str_vappendf
    sqlite3_str *str_obj = sqlite3_str_new(db);
    if (str_obj) {
        sqlite3_str_appendf(str_obj, format, arg_str, arg_int);
        fuzz_str_vappendf_wrapper(str_obj, format, arg_str, arg_int);
        char *str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    // 6. Test sqlite3_log
    // Set up a dummy log callback
    sqlite3_config(SQLITE_CONFIG_LOG, dummy_log_callback, NULL);
    sqlite3_log(SQLITE_NOTICE, format, arg_str, arg_int);
    
    // Cleanup
    free(format);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}