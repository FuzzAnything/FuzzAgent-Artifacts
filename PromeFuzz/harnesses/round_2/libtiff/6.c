// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write input data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // First open for writing
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) return 0;
    
    // Set up some basic image parameters for writing
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samplesperpixel = 1;
    uint16_t bitspersample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint32_t rowsperstrip = 16;
    uint32_t tilewidth = 64;
    uint32_t tilelength = 64;
    
    // Call TIFFSetField 8 times as specified
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    
    // Continue with remaining functions in specified order
    TIFFDeferStrileArrayWriting(tif);
    TIFFWriteCheck(tif, 0, "test");
    TIFFWriteDirectory(tif);
    
    // Set directory to 0
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    TIFFSetDirectory(tif, 0);
    
    // Prepare some dummy data for writing
    size_t buffer_size = width * height;
    uint8_t *write_buffer = (uint8_t*)malloc(buffer_size);
    if (write_buffer) {
        memset(write_buffer, 0x80, buffer_size);
        
        // Set up tiling if needed
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tilewidth);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tilelength);
        
        // Write encoded tile (tile 0)
        TIFFWriteEncodedTile(tif, 0, write_buffer, tilewidth * tilelength);
        
        // Write encoded strip (strip 0)
        TIFFWriteEncodedStrip(tif, 0, write_buffer, width * rowsperstrip);
        
        free(write_buffer);
    }
    
    TIFFClose(tif);
    
    // Now open for reading
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;
    
    // Prepare read buffer
    uint8_t *read_buffer = (uint8_t*)malloc(4096);
    if (read_buffer) {
        // Read encoded tile
        TIFFReadEncodedTile(tif, 0, read_buffer, 4096);
        free(read_buffer);
    }
    
    TIFFClose(tif);
    
    // Open again for reading strip
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;
    
    // Prepare another read buffer
    read_buffer = (uint8_t*)malloc(4096);
    if (read_buffer) {
        // Read encoded strip
        TIFFReadEncodedStrip(tif, 0, read_buffer, 4096);
        free(read_buffer);
    }
    
    TIFFClose(tif);
    
    // Final TIFFClose (on NULL to match sequence)
    TIFFClose(NULL);
    
    return 0;
}