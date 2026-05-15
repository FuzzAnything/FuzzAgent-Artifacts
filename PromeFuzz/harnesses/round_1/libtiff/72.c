// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
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
    // Suppress libtiff error/warning messages during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size, int mode) {
    // Create a dummy file with the fuzz data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    // Write fuzz data to file
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
    
    // Open TIFF file with the specified mode
    TIFF* tif = TIFFOpen("./dummy_file", mode == 0 ? "w" : "r+");
    
    // Set up minimal required tags for a valid TIFF
    if (tif && mode == 0) { // Write mode
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    }
    
    return tif;
}

static void cleanup_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_72(const uint8_t* data, size_t size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Use first byte to determine mode and operation selection
    if (size < 1) return 0;
    uint8_t selector = data[0];
    
    // Create TIFF in different modes based on selector
    TIFF* tif = NULL;
    int mode = selector & 1; // 0 for write, 1 for read-write
    
    // Use different subsets of data for file content
    size_t file_data_size = size > 1 ? size - 1 : 0;
    const uint8_t* file_data = size > 1 ? data + 1 : data;
    
    tif = create_dummy_tiff(file_data, file_data_size, mode);
    if (!tif) return 0;
    
    // Use selector to determine which operations to perform
    uint8_t op_mask = selector >> 1;
    
    // Perform setup operations first
    if (op_mask & 0x01) {
        TIFFSetupStrips(tif);
    }
    
    if (op_mask & 0x02) {
        TIFFDeferStrileArrayWriting(tif);
    }
    
    // Perform directory operations
    if (op_mask & 0x04) {
        TIFFWriteDirectory(tif);
    }
    
    if (op_mask & 0x08) {
        TIFFRewriteDirectory(tif);
    }
    
    if (op_mask & 0x10) {
        TIFFCheckpointDirectory(tif);
    }
    
    if (op_mask & 0x20) {
        TIFFForceStrileArrayWriting(tif);
    }
    
    // Try to explore more states by calling functions multiple times with different conditions
    if (size > 2) {
        uint8_t extra_selector = data[1];
        
        // Try different combinations
        if (extra_selector & 0x01) {
            // Try to write another directory
            if (mode == 0) {
                TIFFSetDirectory(tif, 0);
                TIFFWriteDirectory(tif);
            }
        }
        
        if (extra_selector & 0x02 && mode == 0) {
            // Try to defer and force strile array writing in sequence
            TIFFDeferStrileArrayWriting(tif);
            TIFFForceStrileArrayWriting(tif);
        }
        
        if (extra_selector & 0x04) {
            // Try checkpoint then rewrite
            TIFFCheckpointDirectory(tif);
            TIFFRewriteDirectory(tif);
        }
    }
    
    cleanup_tiff(tif);
    return 0;
}