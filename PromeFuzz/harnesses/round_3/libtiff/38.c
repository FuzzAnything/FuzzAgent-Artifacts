// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tiffio.h"

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_error_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_error_handler_ext_r(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

static void dummy_warning_handler_ext_r(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create dummy file with TIFF header
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write minimal TIFF header
    uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(header, 1, sizeof(header), fp);
    
    // Write some fuzzed data
    if (Size > 0) {
        size_t write_size = Size > 1024 ? 1024 : Size;
        fwrite(Data, 1, write_size, fp);
    }
    fclose(fp);
    
    // Open the dummy file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    
    // Set up dummy handlers to avoid console output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);
    
    if (tif) {
        // Extract strings from fuzzed data
        size_t module_len = Size / 3;
        size_t fmt_len = Size / 3;
        size_t arg_len = Size - module_len - fmt_len;
        
        if (module_len > 256) module_len = 256;
        if (fmt_len > 256) fmt_len = 256;
        if (arg_len > 256) arg_len = 256;
        
        char* module_str = (char*)malloc(module_len + 1);
        char* fmt_str = (char*)malloc(fmt_len + 1);
        char* arg_str = (char*)malloc(arg_len + 1);
        
        if (module_str && fmt_str && arg_str) {
            memcpy(module_str, Data, module_len);
            module_str[module_len] = '\0';
            
            memcpy(fmt_str, Data + module_len, fmt_len);
            fmt_str[fmt_len] = '\0';
            
            memcpy(arg_str, Data + module_len + fmt_len, arg_len);
            arg_str[arg_len] = '\0';
            
            // Create dummy thandle_t
            thandle_t dummy_handle = (thandle_t)arg_str;
            
            // Call all target functions with various combinations
            int choice = Data[0] % 6;
            
            switch (choice) {
                case 0:
                    TIFFWarning(module_str, fmt_str, arg_str);
                    break;
                case 1:
                    TIFFErrorExtR(tif, module_str, fmt_str, arg_str);
                    break;
                case 2:
                    TIFFWarningExtR(tif, module_str, fmt_str, arg_str);
                    break;
                case 3:
                    TIFFErrorExt(dummy_handle, module_str, fmt_str, arg_str);
                    break;
                case 4:
                    TIFFError(module_str, fmt_str, arg_str);
                    break;
                case 5:
                    TIFFWarningExt(dummy_handle, module_str, fmt_str, arg_str);
                    break;
            }
            
            // Try with different argument counts
            if (Size > 10) {
                int int_arg = Data[1];
                const char* str_arg = arg_str;
                
                TIFFWarning(module_str, "Test %d: %s", int_arg, str_arg);
                TIFFErrorExtR(tif, module_str, "Error %d: %s", int_arg, str_arg);
            }
        }
        
        free(module_str);
        free(fmt_str);
        free(arg_str);
        
        TIFFClose(tif);
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}