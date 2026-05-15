// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include the required header file */
#include "libtiff/tiffio.h"

/* Define O_RDONLY if not defined */
#ifndef O_RDONLY
#define O_RDONLY 0
#endif

/* Custom handlers for testing */
static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    /* Do nothing - just consume the warning */
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    /* Do nothing - just consume the error */
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    /* Do nothing - just consume the error */
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    /* Do nothing - just consume the warning */
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    /* Ensure we have some data */
    if (Size < 1) {
        return 0;
    }
    
    /* Create null-terminated strings from fuzzer input */
    char* module = NULL;
    char* format = NULL;
    char* message = NULL;
    
    /* Allocate buffers with null termination */
    size_t module_len = Size > 0 ? (Data[0] % Size) + 1 : 1;
    if (module_len > Size) module_len = Size;
    
    size_t format_len = Size > module_len ? (Data[module_len % Size] % (Size - module_len)) + 1 : 1;
    if (format_len > Size - module_len) format_len = Size - module_len;
    
    size_t message_len = Size - module_len - format_len;
    
    module = (char*)malloc(module_len + 1);
    format = (char*)malloc(format_len + 1);
    message = (char*)malloc(message_len + 1);
    
    if (!module || !format || !message) {
        free(module);
        free(format);
        free(message);
        return 0;
    }
    
    /* Copy data and null-terminate */
    if (module_len > 0) memcpy(module, Data, module_len);
    module[module_len] = '\0';
    
    if (format_len > 0) memcpy(format, Data + module_len, format_len);
    format[format_len] = '\0';
    
    if (message_len > 0) memcpy(message, Data + module_len + format_len, message_len);
    message[message_len] = '\0';
    
    /* Test TIFFGetVersion - always safe to call */
    const char* version = TIFFGetVersion();
    (void)version; /* Suppress unused variable warning */
    
    /* Set custom handlers to avoid printing to stderr during fuzzing */
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    
    /* Test TIFFWarning */
    TIFFWarning(module, format, message);
    
    /* Test TIFFError */
    TIFFError(module, format, message);
    
    /* Test TIFFErrorExt with dummy client data */
    thandle_t dummy_client_data = (thandle_t)(uintptr_t)Data[0];
    TIFFErrorExt(dummy_client_data, module, format, message);
    
    /* Test TIFFWarningExt with dummy client data */
    TIFFWarningExt(dummy_client_data, module, format, message);
    
    /* Test TIFFErrorExtR with NULL TIFF pointer (safe since we're just testing the function) */
    TIFFErrorExtR(NULL, module, format, message);
    
    /* Set extended handlers */
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    
    /* Test again with extended handlers set */
    TIFFErrorExt(dummy_client_data, module, format, message);
    TIFFWarningExt(dummy_client_data, module, format, message);
    
    /* Clean up */
    free(module);
    free(format);
    free(message);
    
    return 0;
}