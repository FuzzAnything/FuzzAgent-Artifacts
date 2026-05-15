// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_error_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Set dummy handlers to avoid output to stderr
    TIFFSetWarningHandler((TIFFErrorHandler)dummy_warning_handler);
    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warning_handler_ext);
    TIFFSetErrorHandler((TIFFErrorHandler)dummy_error_handler);
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler_ext);

    // Use first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    
    // Use second byte to determine format string variant
    uint8_t fmt_selector = Data[1];
    
    // Prepare module name
    const char* module = "FUZZ";
    
    // Prepare format strings
    const char* fmt_strings[] = {
        "%s",
        "%d",
        "%s %d",
        "%02x",
        "%s: %d",
        "Error: %s"
    };
    
    const char* fmt = fmt_strings[fmt_selector % (sizeof(fmt_strings)/sizeof(fmt_strings[0]))];
    
    // Prepare arguments based on remaining data
    const char* str_arg = "test";
    int int_arg = 42;
    unsigned int uint_arg = 0xdeadbeef;
    
    if (Size > 10) {
        // Use some data for string argument if available
        static char dynamic_str[256];
        size_t str_len = Size - 2 > 255 ? 255 : Size - 2;
        memcpy(dynamic_str, Data + 2, str_len);
        dynamic_str[str_len] = '\0';
        str_arg = dynamic_str;
        
        // Use data for integer arguments
        if (Size >= 6) {
            int_arg = *(int*)(Data + 2);
            uint_arg = *(unsigned int*)(Data + 2);
        }
    }

    // Create a dummy TIFF structure for functions that need it
    TIFF* dummy_tif = NULL;
    thandle_t dummy_handle = (thandle_t)(void*)0xdeadbeef;
    
    // Try to create a real TIFF file for more realistic testing
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        // Write minimal TIFF header
        uint8_t tiff_header[] = {
            0x49, 0x49, 0x2A, 0x00,  // Little-endian, TIFF magic
            0x08, 0x00, 0x00, 0x00,  // First IFD offset
            0x00, 0x00              // Padding
        };
        fwrite(tiff_header, 1, sizeof(tiff_header), dummy_file);
        fclose(dummy_file);
        
        // Try to open the dummy file
        dummy_tif = TIFFOpen("./dummy_file", "r");
    }

    switch (function_selector) {
        case 0:
            TIFFWarning(module, fmt, str_arg);
            break;
        case 1:
            TIFFErrorExt(dummy_handle, module, fmt, str_arg, int_arg);
            break;
        case 2:
            if (dummy_tif) {
                TIFFWarningExtR(dummy_tif, module, fmt, str_arg);
            }
            break;
        case 3:
            TIFFError(module, fmt, str_arg, int_arg);
            break;
        case 4:
            if (dummy_tif) {
                TIFFErrorExtR(dummy_tif, module, fmt, str_arg, uint_arg);
            }
            break;
        case 5:
            TIFFWarningExt(dummy_handle, module, fmt, str_arg, int_arg);
            break;
    }

    // Clean up
    if (dummy_tif) {
        TIFFClose(dummy_tif);
    }
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}