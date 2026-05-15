// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* create_test_tiff(const uint8_t* data, size_t size, int write_mode) {
    static int file_counter = 0;
    char filename[256];
    snprintf(filename, sizeof(filename), "./dummy_file_%d", file_counter++);
    
    // Write data to file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
    
    // Open TIFF file
    const char* mode = write_mode ? "w" : "r";
    TIFF* tif = TIFFOpen(filename, mode);
    
    // Clean up file
    unlink(filename);
    
    return tif;
}

static void setup_tiff_directory(TIFF* tif) {
    if (!tif) return;
    
    // Set some basic tags to make directory valid
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 10);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to silent
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Test with different modes and data sizes
    for (int mode = 0; mode < 2; mode++) {
        // Create TIFF file with fuzzer data
        TIFF* tif = create_test_tiff(Data, Size, mode);
        if (!tif) continue;
        
        // Setup directory for write mode
        if (mode) {
            setup_tiff_directory(tif);
        }
        
        // Try different function combinations based on input
        uint8_t selector = Size > 0 ? Data[0] % 6 : 0;
        
        switch (selector) {
            case 0:
                if (mode) TIFFSetupStrips(tif);
                break;
            case 1:
                TIFFFlush(tif);
                break;
            case 2:
                TIFFCheckpointDirectory(tif);
                break;
            case 3:
                if (mode) TIFFDeferStrileArrayWriting(tif);
                break;
            case 4:
                if (mode) TIFFRewriteDirectory(tif);
                break;
            case 5:
                if (mode) {
                    TIFFDeferStrileArrayWriting(tif);
                    TIFFForceStrileArrayWriting(tif);
                }
                break;
        }
        
        // Try all functions in sequence for some inputs
        if (Size > 1 && Data[1] % 3 == 0) {
            if (mode) {
                TIFFSetupStrips(tif);
                TIFFFlush(tif);
                TIFFCheckpointDirectory(tif);
            }
        }
        
        // Clean up
        TIFFClose(tif);
    }
    
    return 0;
}