// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "libtiff/tiffio.h"

/* Dummy warning handler */
static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Dummy error handler */
static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Dummy warning handler for extended version */
static void dummy_warning_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Dummy error handler for extended version */
static void dummy_error_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
    /* Do nothing */
}

/* Helper function to create a dummy TIFF structure */
static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    /* Write some minimal TIFF header data */
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  /* Little-endian, classic TIFF */
        0x08, 0x00, 0x00, 0x00,  /* Offset to first IFD */
        0x00, 0x00              /* End of IFD */
    };
    fwrite(header, 1, sizeof(header), fp);
    
    /* Append fuzzer data */
    if (size > 0 && data != NULL) {
        fwrite(data, 1, size > 1024 ? 1024 : size, fp);
    }
    
    fclose(fp);
    
    /* Open the dummy file as TIFF */
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    
    /* Clean up the file */
    remove("./dummy_file");
    
    return tif;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    /* Set custom handlers to avoid output during fuzzing */
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    
    /* Test TIFFGetVersion - always safe to call */
    const char* version = TIFFGetVersion();
    (void)version; /* Suppress unused variable warning */
    
    /* Create format strings from fuzzer data */
    char module[256] = {0};
    char format[256] = {0};
    char message[256] = {0};
    
    /* Extract module name (first part of data) */
    size_t module_len = Size > 0 ? Data[0] % 255 : 0;
    if (module_len > 0 && module_len < sizeof(module) && module_len < Size) {
        memcpy(module, Data + 1, module_len);
        module[module_len] = '\0';
    } else {
        strcpy(module, "fuzz");
    }
    
    /* Extract format string (second part of data) */
    size_t format_start = 1 + module_len;
    size_t format_len = 0;
    if (format_start < Size) {
        format_len = Data[format_start] % 255;
        size_t format_offset = format_start + 1;
        if (format_len > 0 && format_len < sizeof(format) && 
            format_offset + format_len < Size) {
            memcpy(format, Data + format_offset, format_len);
            format[format_len] = '\0';
        }
    }
    
    /* If no valid format, use a simple one */
    if (format[0] == '\0') {
        strcpy(format, "Test message: %s");
    }
    
    /* Extract message for format string */
    size_t msg_start = 1 + module_len + 1 + format_len;
    size_t msg_len = 0;
    if (msg_start < Size) {
        msg_len = Size - msg_start;
        if (msg_len > sizeof(message) - 1) {
            msg_len = sizeof(message) - 1;
        }
        memcpy(message, Data + msg_start, msg_len);
        message[msg_len] = '\0';
    } else {
        strcpy(message, "fuzzed");
    }
    
    /* Test TIFFWarning */
    TIFFWarning(module, format, message);
    
    /* Test TIFFError */
    TIFFError(module, format, message);
    
    /* Create a dummy TIFF structure for functions that need it */
    TIFF* tif = create_dummy_tiff(Data, Size);
    
    if (tif != NULL) {
        /* Test TIFFWarningExtR */
        TIFFWarningExtR(tif, module, format, message);
        
        /* Test TIFFWarningExt */
        TIFFWarningExt((thandle_t)tif, module, format, message);
        
        /* Test TIFFErrorExt */
        TIFFErrorExt((thandle_t)tif, module, format, message);
        
        /* Clean up */
        TIFFClose(tif);
    } else {
        /* Even without a valid TIFF, we can test some functions with NULL */
        TIFFWarningExtR(NULL, module, format, message);
        TIFFWarningExt(NULL, module, format, message);
        TIFFErrorExt(NULL, module, format, message);
    }
    
    /* Try different variations */
    if (Size > 10) {
        /* Test with integer argument */
        int int_arg = (int)Data[0];
        TIFFWarning(module, "Integer test: %d", int_arg);
        
        /* Test with multiple arguments */
        if (Size > 20) {
            int arg1 = (int)Data[1];
            int arg2 = (int)Data[2];
            TIFFError(module, "Multiple args: %d, %d", arg1, arg2);
        }
    }
    
    /* Test with empty strings */
    TIFFWarning("", "", "");
    TIFFError("", "%s", "");
    
    return 0;
}