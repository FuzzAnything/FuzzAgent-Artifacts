// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings and errors during fuzzing
}

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer data to dummy file
    if (write_to_file("./dummy_file", Data, Size) != 0) {
        return 0;
    }
    
    // Open the TIFF file with different modes to explore states
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // If can't open for reading, try to create new file
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
        
        // Create initial directory for new file
        TIFFCreateDirectory(tif);
        
        // Try to write directory
        TIFFWriteDirectory(tif);
        
        // Cleanup and reopen for further operations
        TIFFCleanup(tif);
        TIFFClose(tif);
        
        tif = TIFFOpen("./dummy_file", "r+");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Try to read existing directories
    int dir_count = 0;
    while (TIFFReadDirectory(tif) == 1) {
        dir_count++;
        if (dir_count > 10) break; // Prevent infinite loops
        
        // Occasionally try to rewrite current directory
        if (Size > 0 && (Data[0] % 3 == 0)) {
            TIFFRewriteDirectory(tif);
        }
        
        // Occasionally create new directory
        if (Size > 1 && (Data[1] % 4 == 0)) {
            TIFFCreateDirectory(tif);
            
            // Sometimes write the new directory
            if (Size > 2 && (Data[2] % 2 == 0)) {
                TIFFWriteDirectory(tif);
            }
            
            // Sometimes free the directory without writing
            if (Size > 3 && (Data[3] % 2 == 0)) {
                TIFFFreeDirectory(tif);
            }
        }
    }
    
    // Reset to first directory if any exist
    if (dir_count > 0) {
        TIFFSetDirectory(tif, 0);
        
        // Try various operations on first directory
        if (Size > 4) {
            switch (Data[4] % 6) {
                case 0:
                    TIFFFreeDirectory(tif);
                    // After freeing, create new directory
                    TIFFCreateDirectory(tif);
                    break;
                case 1:
                    TIFFCreateDirectory(tif);
                    break;
                case 2:
                    TIFFWriteDirectory(tif);
                    break;
                case 3:
                    TIFFCleanup(tif);
                    // Reopen after cleanup
                    TIFFClose(tif);
                    tif = TIFFOpen("./dummy_file", "r");
                    if (!tif) {
                        unlink("./dummy_file");
                        return 0;
                    }
                    break;
                case 4:
                    TIFFReadDirectory(tif);
                    break;
                case 5:
                    TIFFRewriteDirectory(tif);
                    break;
            }
        }
    } else {
        // No directories exist, try to create one
        TIFFCreateDirectory(tif);
        
        // Try to write it
        if (Size > 5 && (Data[5] % 2 == 0)) {
            TIFFWriteDirectory(tif);
        }
    }
    
    // Clean up
    if (tif) {
        TIFFCleanup(tif);
        TIFFClose(tif);
    }
    
    unlink("./dummy_file");
    return 0;
}