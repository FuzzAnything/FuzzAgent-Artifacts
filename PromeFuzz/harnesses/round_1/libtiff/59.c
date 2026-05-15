// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);

    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Get basic image information
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 1, bitspersample = 8;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);

    // Calculate strips information
    uint32_t rowsperstrip = (uint32_t)-1;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    if (rowsperstrip == (uint32_t)-1) {
        rowsperstrip = height;
    }
    
    uint32_t stripcount = TIFFNumberOfStrips(tif);
    
    // Use fuzzer data to generate parameters
    uint32_t strip = 0;
    uint32_t nrows = 1;
    
    if (Size >= 8) {
        strip = *(uint32_t*)Data % (stripcount > 0 ? stripcount : 1);
        nrows = *(uint32_t*)(Data + 4) % (height > 0 ? height : 1);
        if (nrows == 0) nrows = 1;
    }
    
    // Test TIFFRawStripSize
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip);
    
    // Test TIFFVStripSize
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
    
    // Allocate buffers for read/write operations
    tmsize_t max_buffer_size = raw_strip_size > vstrip_size ? raw_strip_size : vstrip_size;
    if (max_buffer_size <= 0) max_buffer_size = 1024;
    if (max_buffer_size > 10 * 1024 * 1024) max_buffer_size = 10 * 1024 * 1024; // Limit to 10MB
    
    void *read_buffer = malloc(max_buffer_size);
    void *write_buffer = malloc(max_buffer_size);
    
    if (read_buffer && write_buffer) {
        // Initialize write buffer with some data
        size_t write_data_size = Size > max_buffer_size ? max_buffer_size : Size;
        memcpy(write_buffer, Data, write_data_size);
        
        // Test TIFFReadRawStrip
        if (stripcount > 0 && strip < stripcount) {
            TIFFReadRawStrip(tif, strip, read_buffer, max_buffer_size);
        }
        
        // Test TIFFReadEncodedStrip
        if (stripcount > 0 && strip < stripcount) {
            TIFFReadEncodedStrip(tif, strip, read_buffer, max_buffer_size);
        }
        
        // Test TIFFWriteRawStrip
        if (stripcount > 0 && strip < stripcount) {
            TIFFWriteRawStrip(tif, strip, write_buffer, 
                             write_data_size > 0 ? write_data_size : max_buffer_size);
        }
        
        // Test TIFFWriteEncodedStrip
        if (stripcount > 0 && strip < stripcount) {
            TIFFWriteEncodedStrip(tif, strip, write_buffer, 
                                 write_data_size > 0 ? write_data_size : max_buffer_size);
        }
    }
    
    // Cleanup
    if (read_buffer) free(read_buffer);
    if (write_buffer) free(write_buffer);
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}