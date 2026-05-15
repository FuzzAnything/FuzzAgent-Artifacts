// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the file as a TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // If opening fails, try with different mode
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Get basic image information
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 1, bitspersample = 8;
    uint32_t rowsperstrip = 0;
    uint32_t stripsperimage = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    
    if (rowsperstrip == 0 || rowsperstrip > height) {
        rowsperstrip = height;
    }
    
    stripsperimage = TIFFNumberOfStrips(tif);
    
    // Use fuzzer data to generate parameters
    uint32_t strip_index = 0;
    uint32_t nrows = 0;
    
    if (Size >= 4) {
        strip_index = *(uint32_t*)Data % (stripsperimage > 0 ? stripsperimage : 1);
        if (Size >= 8) {
            nrows = *(uint32_t*)(Data + 4) % (height + 1);
        }
    }
    
    // Test TIFFRawStripSize
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_index);
    
    // Test TIFFVStripSize
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
    
    // Allocate buffers for read/write operations
    tmsize_t max_buffer_size = raw_strip_size > vstrip_size ? raw_strip_size : vstrip_size;
    if (max_buffer_size <= 0) max_buffer_size = 1024;
    
    void *read_buf = _TIFFmalloc(max_buffer_size);
    void *write_buf = _TIFFmalloc(max_buffer_size);
    
    if (read_buf && write_buf) {
        // Initialize write buffer with some data
        size_t init_size = Size > max_buffer_size ? max_buffer_size : Size;
        if (init_size > 0) {
            memcpy(write_buf, Data, init_size);
        } else {
            memset(write_buf, 0, max_buffer_size);
        }
        
        // Test TIFFReadRawStrip
        if (raw_strip_size > 0 && raw_strip_size <= max_buffer_size) {
            TIFFReadRawStrip(tif, strip_index, read_buf, raw_strip_size);
        }
        
        // Test TIFFWriteRawStrip
        if (raw_strip_size > 0 && raw_strip_size <= max_buffer_size) {
            TIFFWriteRawStrip(tif, strip_index, write_buf, raw_strip_size);
        }
        
        // Test TIFFReadEncodedStrip
        if (strip_index < stripsperimage) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && strip_size <= max_buffer_size) {
                TIFFReadEncodedStrip(tif, strip_index, read_buf, strip_size);
            }
        }
        
        // Test TIFFWriteEncodedStrip
        if (strip_index < stripsperimage) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && strip_size <= max_buffer_size) {
                TIFFWriteEncodedStrip(tif, strip_index, write_buf, strip_size);
            }
        }
    }
    
    // Cleanup
    if (read_buf) _TIFFfree(read_buf);
    if (write_buf) _TIFFfree(write_buf);
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}