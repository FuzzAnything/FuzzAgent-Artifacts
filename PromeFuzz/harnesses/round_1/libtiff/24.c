// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to silence output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a dummy file with the fuzz data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the TIFF file in write mode first
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    int is_write_mode = 0;
    
    if (!tif) {
        // Try read mode if write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            remove("./dummy_file");
            return 0;
        }
    } else {
        is_write_mode = 1;
    }
    
    // If we're in write mode, set up basic image parameters
    if (is_write_mode) {
        // Set basic image parameters to make directory writing more likely to succeed
        uint32_t width = 1;
        uint32_t height = 1;
        uint16_t samplesperpixel = 1;
        uint16_t bitspersample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planarconfig = PLANARCONFIG_CONTIG;
        uint32_t rowsperstrip = 1;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
        
        // Try to write directory
        TIFFWriteDirectory(tif);
    }
    
    // Try to set directory to index 0 (first directory)
    TIFFSetDirectory(tif, 0);
    
    // Get current directory offset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset; // Use offset to avoid unused variable warning
    
    // Try to set directory again, maybe with different index
    // Use modulo to keep index in reasonable range
    tdir_t dir_index = Size > 0 ? (Data[0] % 10) : 0;
    TIFFSetDirectory(tif, dir_index);
    
    // Try to force strile array writing
    TIFFForceStrileArrayWriting(tif);
    
    // Flush data
    TIFFFlushData(tif);
    
    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}