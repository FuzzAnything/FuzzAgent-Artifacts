// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFGetVersion at tif_version.c:28:13 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - just a dummy handler
}

static void dummy_error_handler_ext(thandle_t fd, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just a dummy handler
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - just a dummy handler
}

static void dummy_warning_handler_ext(thandle_t fd, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just a dummy handler
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Set dummy handlers to avoid output during fuzzing
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    
    // Get version - always safe to call
    const char *version = TIFFGetVersion();
    (void)version; // Suppress unused variable warning
    
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        // Try to open the file as TIFF
        TIFF *tif = TIFFOpen("./dummy_file", "r");
        
        // Create module and message strings from input data
        // Use first half for module, second half for message
        size_t module_len = Size / 2;
        size_t msg_len = Size - module_len;
        
        char *module = NULL;
        char *message = NULL;
        
        if (Size > 0) {
            module = (char*)malloc(module_len + 1);
            message = (char*)malloc(msg_len + 1);
            
            if (module && message) {
                memcpy(module, Data, module_len);
                module[module_len] = '\0';
                memcpy(message, Data + module_len, msg_len);
                message[msg_len] = '\0';
            }
        } else {
            // Use default strings if no data
            module = strdup("module");
            message = strdup("message");
        }
        
        if (module && message) {
            // Call all target functions with various parameters
            
            // 1. TIFFWarningExt
            if (tif) {
                TIFFWarningExt((thandle_t)tif, module, "%s", message);
            }
            
            // 2. TIFFError
            TIFFError(module, "%s", message);
            
            // 3. TIFFErrorExtR
            if (tif) {
                TIFFErrorExtR(tif, module, "%s", message);
            }
            
            // 4. TIFFWarning
            TIFFWarning(module, "%s", message);
            
            // 6. TIFFErrorExt
            if (tif) {
                TIFFErrorExt((thandle_t)tif, module, "%s", message);
            }
            
            free(module);
            free(message);
        }
        
        if (tif) {
            TIFFClose(tif);
        }
        
        // Clean up dummy file
        remove("./dummy_file");
    }
    
    return 0;
}