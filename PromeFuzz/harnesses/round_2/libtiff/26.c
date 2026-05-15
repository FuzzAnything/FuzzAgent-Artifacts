// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write input data to dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    close(fd);
    
    // Open the dummy file as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only mode if read-write fails
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    if (tif) {
        // Use fuzz data to determine which operations to perform
        // Use first byte to select operation mode
        uint8_t mode = Size > 0 ? Data[0] : 0;
        
        // Try different sequences of operations based on mode
        switch (mode % 6) {  // Reduced from 7 to 6 cases
            case 0:
                // Create GPS directory and write it
                if (TIFFCreateGPSDirectory(tif)) {
                    uint64_t offset = 0;
                    TIFFWriteCustomDirectory(tif, &offset);
                }
                break;
                
            case 1:
                // Read EXIF directory at different offsets
                if (Size >= sizeof(uint64_t) + 1) {
                    uint64_t diroff;
                    memcpy(&diroff, Data + 1, sizeof(uint64_t));
                    TIFFReadEXIFDirectory(tif, diroff);
                } else {
                    // Try some default offsets
                    TIFFReadEXIFDirectory(tif, 0);
                    TIFFReadEXIFDirectory(tif, 100);
                }
                break;
                
            case 2:
                // Create directory and write it
                if (TIFFCreateDirectory(tif)) {
                    TIFFWriteDirectory(tif);
                }
                break;
                
            case 3:
                // Create EXIF directory and write it
                if (TIFFCreateEXIFDirectory(tif)) {
                    uint64_t offset = 0;
                    TIFFWriteCustomDirectory(tif, &offset);
                }
                break;
                
            case 4:
                // Combination: Create directory, then GPS directory
                TIFFCreateDirectory(tif);
                TIFFWriteDirectory(tif);
                TIFFCreateGPSDirectory(tif);
                {
                    uint64_t offset = 0;
                    TIFFWriteCustomDirectory(tif, &offset);
                }
                break;
                
            case 5:
                // Combination: Create EXIF directory, read EXIF directory
                TIFFCreateEXIFDirectory(tif);
                {
                    uint64_t offset = 0;
                    TIFFWriteCustomDirectory(tif, &offset);
                }
                if (Size >= sizeof(uint64_t) + 1) {
                    uint64_t diroff;
                    memcpy(&diroff, Data + 1, sizeof(uint64_t));
                    TIFFReadEXIFDirectory(tif, diroff);
                }
                break;
        }
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}