// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
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
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
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
    // Suppress libtiff error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size, int mode) {
    // Write data to dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    if (size > 0 && data != NULL) {
        write(fd, data, size);
    }
    close(fd);
    
    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", mode == 0 ? "r" : (mode == 1 ? "w" : "r+"));
    if (!tif) {
        unlink("./dummy_file");
        return NULL;
    }
    
    return tif;
}

static void setup_minimal_tags(TIFF* tif) {
    // Set minimal required tags for a valid TIFF directory
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)256);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)256);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)1);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, (uint16_t)PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, (uint16_t)PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, (uint16_t)COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (uint32_t)256);
}

int LLVMFuzzerTestOneInput_47(const uint8_t *data, size_t size) {
    // Suppress libtiff messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Use first byte to determine mode and operation sequence
    if (size < 1) return 0;
    uint8_t mode = data[0] % 4;
    uint8_t seq = data[1] % 8;
    
    // Create TIFF file with different modes
    TIFF* tif = create_dummy_tiff(data + 2, size > 2 ? size - 2 : 0, mode);
    if (!tif) return 0;
    
    // Setup minimal tags for write operations
    if (mode > 0) {
        setup_minimal_tags(tif);
    }
    
    // Execute different sequences of API calls
    switch (seq) {
        case 0:
            // Basic directory operations
            if (mode > 0) TIFFSetupStrips(tif);
            TIFFCheckpointDirectory(tif);
            if (mode > 0) TIFFWriteDirectory(tif);
            break;
            
        case 1:
            // Deferred strile array writing sequence
            if (mode > 0) {
                TIFFSetupStrips(tif);
                TIFFDeferStrileArrayWriting(tif);
                TIFFWriteDirectory(tif);
                TIFFForceStrileArrayWriting(tif);
            }
            break;
            
        case 2:
            // Rewrite directory sequence
            if (mode > 0) {
                TIFFSetupStrips(tif);
                TIFFWriteDirectory(tif);
                TIFFRewriteDirectory(tif);
            }
            break;
            
        case 3:
            // Mixed operations
            TIFFCheckpointDirectory(tif);
            if (mode > 0) {
                TIFFSetupStrips(tif);
                TIFFDeferStrileArrayWriting(tif);
                TIFFForceStrileArrayWriting(tif);
            }
            break;
            
        case 4:
            // Setup strips with checkpoint
            if (mode > 0) TIFFSetupStrips(tif);
            TIFFCheckpointDirectory(tif);
            TIFFCheckpointDirectory(tif);  // Call twice
            break;
            
        case 5:
            // Write multiple directories
            if (mode > 0) {
                TIFFSetupStrips(tif);
                TIFFWriteDirectory(tif);
                TIFFWriteDirectory(tif);
                TIFFRewriteDirectory(tif);
            }
            break;
            
        case 6:
            // Defer then checkpoint
            if (mode > 0) {
                TIFFSetupStrips(tif);
                TIFFDeferStrileArrayWriting(tif);
                TIFFCheckpointDirectory(tif);
                TIFFForceStrileArrayWriting(tif);
            }
            break;
            
        case 7:
            // All functions in sequence
            if (mode > 0) TIFFSetupStrips(tif);
            TIFFCheckpointDirectory(tif);
            if (mode > 0) {
                TIFFDeferStrileArrayWriting(tif);
                TIFFWriteDirectory(tif);
                TIFFForceStrileArrayWriting(tif);
                TIFFRewriteDirectory(tif);
            }
            break;
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}