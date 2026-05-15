// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

/* Dummy handlers to avoid output during fuzzing */
static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    /* Do nothing */
}

static void dummy_handler_ext(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    /* Do nothing */
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    /* Set dummy handlers to avoid output */
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandlerExt(dummy_handler_ext);
    TIFFSetErrorHandlerExt(dummy_handler_ext);
    
    /* Create dummy TIFF structure */
    TIFF* tif = NULL;
    thandle_t handle = NULL;
    
    /* Try to create a dummy TIFF file for testing */
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy);
        }
        fclose(dummy);
        
        /* Try to open as TIFF - may fail but that's OK */
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    /* Prepare module and format strings from fuzzed data */
    const char* module = "fuzz_module";
    const char* format = "fuzz_format";
    
    if (Size > 0) {
        /* Use first byte to determine which format string to use */
        uint8_t selector = Data[0];
        
        /* Create strings from fuzzed data (ensure null termination) */
        char* module_buf = malloc(256);
        char* format_buf = malloc(256);
        
        if (module_buf && format_buf) {
            size_t module_len = Size < 128 ? Size : 128;
            size_t format_len = Size < 128 ? Size : 128;
            
            memcpy(module_buf, Data, module_len);
            module_buf[module_len] = '\0';
            
            /* Use different offset for format string */
            size_t format_offset = Size > 1 ? 1 : 0;
            if (Size - format_offset > 0) {
                size_t copy_len = (Size - format_offset) < 128 ? (Size - format_offset) : 128;
                memcpy(format_buf, Data + format_offset, copy_len);
                format_buf[copy_len] = '\0';
            } else {
                strcpy(format_buf, "fuzz");
            }
            
            module = module_buf;
            format = format_buf;
            
            /* Call target functions with various combinations */
            switch (selector % 6) {
                case 0:
                    TIFFWarning(module, format);
                    break;
                case 1:
                    TIFFWarningExtR(tif, module, format);
                    break;
                case 2:
                    TIFFWarningExt(handle, module, format);
                    break;
                case 3:
                    TIFFError(module, format);
                    break;
                case 4:
                    TIFFErrorExtR(tif, module, format);
                    break;
                case 5:
                    TIFFErrorExt(handle, module, format);
                    break;
            }
            
            /* Also try with additional arguments */
            if (Size > 2) {
                int arg1 = Data[1];
                const char* arg2 = "test";
                
                TIFFWarning(module, "%s: %d", format, arg1);
                TIFFWarningExtR(tif, module, "%s: %s", format, arg2);
                TIFFError(module, "Error: %s (%d)", format, arg1);
                TIFFErrorExtR(tif, module, "Error: %s", format);
            }
            
            free(module_buf);
            free(format_buf);
        }
        
        if (module_buf) free(module_buf);
        if (format_buf) free(format_buf);
    } else {
        /* Test with minimal input */
        TIFFWarning(module, format);
        TIFFError(module, format);
    }
    
    /* Cleanup */
    if (tif) {
        TIFFClose(tif);
    }
    
    /* Remove dummy file */
    remove("./dummy_file");
    
    return 0;
}