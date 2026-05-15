// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silently ignore warnings and errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    static int file_counter = 0;
    char filename[256];
    
    // Create unique filename to avoid collisions
    snprintf(filename, sizeof(filename), "./dummy_file_%d.tiff", file_counter++);
    
    // Write fuzzer data to file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write data if we have any
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Set error handlers to silent mode
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Try to open the file in different modes
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            unlink(filename);
            return 0;
        }
    }
    
    // Get current directory state
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // Try to create a new directory
    int create_result = TIFFCreateDirectory(tif);
    
    // If creation succeeded, try various operations
    if (create_result == 1) {
        // Try to write the directory
        int write_result = TIFFWriteDirectory(tif);
        
        // Try to rewrite directory if we're at a valid position
        if (write_result == 1) {
            // Set some dummy tags to make directory dirty
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            // Try to rewrite
            int rewrite_result = TIFFRewriteDirectory(tif);
            
            // Try checkpoint
            int checkpoint_result = TIFFCheckpointDirectory(tif);
            
            // Free the current directory
            TIFFFreeDirectory(tif);
            
            // Create another directory
            TIFFCreateDirectory(tif);
            
            // Write it
            TIFFWriteDirectory(tif);
        }
    }
    
    // Try to go back to original directory if it existed
    if (current_dir >= 0) {
        TIFFSetDirectory(tif, current_dir);
        
        // Try checkpoint on original directory
        TIFFCheckpointDirectory(tif);
        
        // Try rewrite on original directory
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 50);
        TIFFRewriteDirectory(tif);
        
        // Free and recreate
        TIFFFreeDirectory(tif);
        TIFFCreateDirectory(tif);
        TIFFWriteDirectory(tif);
    }
    
    // Clean up
    TIFFCleanup(tif);
    TIFFClose(tif);
    
    // Remove temporary file
    unlink(filename);
    
    return 0;
}