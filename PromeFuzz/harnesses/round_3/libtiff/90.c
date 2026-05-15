// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "libtiff/tiffio.h"

static int dummy_warning_handler(TIFF* tif, void* user_data, const char* module, 
                                 const char* fmt, va_list ap) {
    return 0;
}

static int dummy_error_handler(TIFF* tif, void* user_data, const char* module,
                               const char* fmt, va_list ap) {
    return 0;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test TIFFOpenExt with various options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts) {
        // Set warning handler
        TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warning_handler, NULL);
        
        // Try to open the file with different modes
        const char *modes[] = {"r", "w", "a", "r+", "w+", "a+"};
        for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
            TIFF *tif = TIFFOpenExt("./dummy_file", modes[i], opts);
            if (tif) {
                // Test TIFFFileName
                const char *filename = TIFFFileName(tif);
                
                // Test TIFFSetFileName with various strings
                if (Size > 0) {
                    // Create a null-terminated string from fuzzer data
                    size_t str_len = Size > 100 ? 100 : Size;
                    char *new_name = (char*)malloc(str_len + 1);
                    if (new_name) {
                        memcpy(new_name, Data, str_len);
                        new_name[str_len] = '\0';
                        
                        const char *prev_name = TIFFSetFileName(tif, new_name);
                        free(new_name);
                    }
                }
                
                // Test TIFFSetFileName with NULL
                TIFFSetFileName(tif, NULL);
                
                // Test TIFFSetFileName with empty string
                TIFFSetFileName(tif, "");
                
                // Test error and warning reporting functions
                if (Size > 10) {
                    // Create format strings from fuzzer data
                    char fmt_buf[50];
                    size_t fmt_len = Size > 50 ? 50 : Size;
                    memcpy(fmt_buf, Data, fmt_len);
                    if (fmt_len > 0) {
                        fmt_buf[fmt_len-1] = '\0';
                    } else {
                        fmt_buf[0] = '\0';
                    }
                    
                    // Test with various arguments
                    TIFFErrorExtR(tif, "test_module", fmt_buf);
                    TIFFWarningExtR(tif, "test_module", fmt_buf);
                    
                    // Test with NULL module
                    TIFFErrorExtR(tif, NULL, "test error");
                    TIFFWarningExtR(tif, NULL, "test warning");
                }
                
                TIFFClose(tif);
            }
        }
        
        // Test with NULL options
        TIFF *tif_null_opts = TIFFOpenExt("./dummy_file", "r", NULL);
        if (tif_null_opts) {
            TIFFClose(tif_null_opts);
        }
        
        TIFFOpenOptionsFree(opts);
    }
    
    // Test with invalid file names
    if (Size > 0) {
        // Create a file name from fuzzer data
        size_t name_len = Size > 50 ? 50 : Size;
        char *invalid_name = (char*)malloc(name_len + 1);
        if (invalid_name) {
            memcpy(invalid_name, Data, name_len);
            invalid_name[name_len] = '\0';
            
            TIFF *tif_invalid = TIFFOpenExt(invalid_name, "r", NULL);
            if (tif_invalid) {
                TIFFClose(tif_invalid);
            }
            free(invalid_name);
        }
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}