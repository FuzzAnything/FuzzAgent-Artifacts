// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

static int error_handler(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    // Return 0 to continue processing
    return 0;
}

static int warning_handler(TIFF* tif, void* user_data, const char* module, const char* fmt, va_list ap) {
    // Return 0 to continue processing
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    const char* filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink(filename);
        return 0;
    }
    close(fd);
    
    // Allocate TIFFOpenOptions
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        unlink(filename);
        return 0;
    }
    
    // Set error handler
    TIFFOpenOptionsSetErrorHandlerExtR(opts, error_handler, NULL);
    
    // Set warning handler
    TIFFOpenOptionsSetWarningHandlerExtR(opts, warning_handler, NULL);
    
    // Try different modes for opening
    const char* modes[] = {"r", "w", "a", "r+", "w+", "a+"};
    for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
        // Open TIFF file with options
        TIFF* tif = TIFFOpenExt(filename, modes[i], opts);
        
        if (tif) {
            // Free options (safe to do after opening)
            TIFFOpenOptionsFree(opts);
            
            // Try to trigger error and warning handlers with various messages
            TIFFErrorExtR(tif, "test_module", "Test error message: %d", 42);
            TIFFWarningExtR(tif, "test_module", "Test warning message: %s", "fuzz");
            
            // Close TIFF file
            TIFFClose(tif);
            
            // Clean up file and return
            unlink(filename);
            return 0;
        }
    }
    
    // If we couldn't open in any mode, still free options
    TIFFOpenOptionsFree(opts);
    
    // Clean up file
    unlink(filename);
    
    return 0;
}