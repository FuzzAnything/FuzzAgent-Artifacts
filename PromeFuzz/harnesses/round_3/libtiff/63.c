// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence warnings and errors during fuzzing
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Initialize error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write input data to a dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);

    // Try to open the file in different modes
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try write mode if read fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            // Try read/write mode
            tif = TIFFOpen("./dummy_file", "r+");
            if (!tif) {
                // Try write mode with different parameters
                tif = TIFFOpen("./dummy_file", "w8");
                if (!tif) return 0;
            }
        }
    }

    // Test TIFFGetTagListCount
    int tag_count = TIFFGetTagListCount(tif);
    (void)tag_count; // Use result to avoid unused variable warning

    // Test directory operations
    if (Size > 0) {
        // Try to write a directory if we have some data
        TIFFWriteDirectory(tif);
        
        // Try to rewrite directory
        TIFFRewriteDirectory(tif);
    }

    // Test custom directory operations
    // We cannot create a complete TIFFFieldArray since it's an opaque type,
    // but we can still test the functions with NULL or try to get existing arrays
    
    // Try to create custom directory with NULL
    if (Size > 8) {
        TIFFCreateCustomDirectory(tif, NULL);
        
        // Try to write custom directory
        uint64_t offset = 0;
        TIFFWriteCustomDirectory(tif, &offset);
        
        // Try to read custom directory at different offsets
        // Use bytes from input as offset values
        uint64_t diroff = 0;
        if (Size >= 16) {
            memcpy(&diroff, Data + 8, sizeof(uint64_t));
        }
        TIFFReadCustomDirectory(tif, diroff, NULL);
    }

    // Clean up
    TIFFClose(tif);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}