// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/mprintf.h"
#include <stdarg.h>
#include <time.h>

static void fuzz_curl_maprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create a format string from the input
    char *format = malloc(size + 1);
    if (!format) return;
    memcpy(format, data, size);
    format[size] = '\0';
    
    // Create some arguments for the format string
    int int_arg = size;
    const char *str_arg = (const char *)data;
    
    // Call curl_maprintf with various format strings
    char *result1 = curl_maprintf("%s", str_arg);
    if (result1) curl_free(result1);
    
    char *result2 = curl_maprintf("%d", int_arg);
    if (result2) curl_free(result2);
    
    char *result3 = curl_maprintf(format, int_arg);
    if (result3) curl_free(result3);
    
    free(format);
}

static void fuzz_curl_getdate(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create a date string from the input
    char *date_str = malloc(size + 1);
    if (!date_str) return;
    memcpy(date_str, data, size);
    date_str[size] = '\0';
    
    // Try to parse the date string
    time_t result = curl_getdate(date_str, NULL);
    (void)result; // Result is unused but we want to see if it crashes
    
    // Try with different variations
    curl_getdate("now", NULL);
    curl_getdate("today", NULL);
    curl_getdate("yesterday", NULL);
    curl_getdate("tomorrow", NULL);
    
    free(date_str);
}

// Helper variadic function for curl_mvsprintf
static void call_curl_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void fuzz_curl_mvsprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create format string from input
    char *format = malloc(size + 1);
    if (!format) return;
    memcpy(format, data, size);
    format[size] = '\0';
    
    // Test with various format strings
    call_curl_mvsprintf("%s", (const char *)data);
    call_curl_mvsprintf("%d", (int)size);
    call_curl_mvsprintf(format, (const char *)data);
    
    free(format);
}

// Helper variadic function for curl_mvaprintf
static char *call_curl_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

static void fuzz_curl_mvaprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create format string from input
    char *format = malloc(size + 1);
    if (!format) return;
    memcpy(format, data, size);
    format[size] = '\0';
    
    // Test with various format strings
    char *result1 = call_curl_mvaprintf("%s", (const char *)data);
    if (result1) curl_free(result1);
    
    char *result2 = call_curl_mvaprintf("%d", (int)size);
    if (result2) curl_free(result2);
    
    char *result3 = call_curl_mvaprintf(format, (const char *)data);
    if (result3) curl_free(result3);
    
    free(format);
}

// Helper variadic function for curl_mvprintf
static void call_curl_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void fuzz_curl_mvprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create format string from input
    char *format = malloc(size + 1);
    if (!format) return;
    memcpy(format, data, size);
    format[size] = '\0';
    
    // Test with various format strings
    call_curl_mvprintf("%s", (const char *)data);
    call_curl_mvprintf("%d", (int)size);
    call_curl_mvprintf(format, (const char *)data);
    
    free(format);
}

// Helper variadic function for curl_mvsnprintf
static void call_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
}

static void fuzz_curl_mvsnprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Create buffers for output
    char buffer1[512];
    char buffer2[256];
    char buffer3[1024];
    char small_buffer[10];
    
    // Create format string from input
    char *format = malloc(size + 1);
    if (!format) return;
    memcpy(format, data, size);
    format[size] = '\0';
    
    // Test with different buffer sizes and formats
    call_curl_mvsnprintf(buffer1, sizeof(buffer1), "%s", (const char *)data);
    call_curl_mvsnprintf(buffer2, sizeof(buffer2), "%d", (int)size);
    call_curl_mvsnprintf(buffer3, sizeof(buffer3), format, (const char *)data);
    call_curl_mvsnprintf(small_buffer, sizeof(small_buffer), "%s", (const char *)data);
    call_curl_mvsnprintf(buffer1, 0, "%s", (const char *)data);
    
    free(format);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Initialize curl if needed (some functions might need initialization)
    // curl_global_init(CURL_GLOBAL_ALL);
    
    // Fuzz each target function
    fuzz_curl_maprintf(Data, Size);
    fuzz_curl_getdate(Data, Size);
    fuzz_curl_mvsprintf(Data, Size);
    fuzz_curl_mvaprintf(Data, Size);
    fuzz_curl_mvprintf(Data, Size);
    fuzz_curl_mvsnprintf(Data, Size);
    
    // Cleanup
    // curl_global_cleanup();
    
    return 0;
}