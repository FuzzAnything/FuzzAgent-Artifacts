// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    // Silence libtiff warnings
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Create dummy file with fuzzer data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink("./dummy_file");
        return 0;
    }
    close(fd);
    
    // Open TIFF file for reading and writing
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only if r+ fails
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    if (tif) {
        // Fuzz TIFFCreateDirectory
        if (Size > 0) {
            TIFFCreateDirectory(tif);
        }
        
        // Fuzz TIFFWriteCustomDirectory
        if (Size > 8) {
            uint64_t offset = 0;
            TIFFWriteCustomDirectory(tif, &offset);
        }
        
        // Fuzz TIFFCreateCustomDirectory - pass NULL for field array
        if (Size > 16) {
            TIFFCreateCustomDirectory(tif, NULL);
        }
        
        // Fuzz TIFFFreeDirectory
        if (Size > 24) {
            TIFFFreeDirectory(tif);
        }
        
        // Fuzz TIFFReadCustomDirectory
        if (Size > 32) {
            // Extract offset from fuzzer data
            toff_t diroff = 0;
            if (Size >= sizeof(toff_t)) {
                memcpy(&diroff, Data, sizeof(toff_t));
            }
            
            // Try with NULL field array
            TIFFReadCustomDirectory(tif, diroff, NULL);
        }
        
        // Fuzz TIFFAccessTagMethods
        if (Size > 40) {
            TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
            (void)methods; // Use variable to avoid unused warning
        }
        
        // Clean up and close
        TIFFClose(tif);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}