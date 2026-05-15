// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzzing data to a dummy file
    const char* filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) {
        unlink(filename);
        return 0;
    }

    // Try to open the file as TIFF
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        // If can't open for reading, try to create a new TIFF file
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            unlink(filename);
            return 0;
        }
        
        // Set some basic tags to make the file valid
        uint32_t width = 100;
        uint32_t height = 100;
        uint16_t samplesperpixel = 1;
        uint16_t bitspersample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planarconfig = PLANARCONFIG_CONTIG;
        uint16_t compression = COMPRESSION_NONE;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, 0));
        
        // Write some dummy data
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0) {
            unsigned char* buf = (unsigned char*)malloc(scanline_size);
            if (buf) {
                memset(buf, 0, scanline_size);
                for (uint32_t row = 0; row < height; row++) {
                    TIFFWriteScanline(tif, buf, row, 0);
                }
                free(buf);
            }
        }
        
        TIFFClose(tif);
        
        // Reopen for reading
        tif = TIFFOpen(filename, "r");
        if (!tif) {
            unlink(filename);
            return 0;
        }
    }

    // Fuzz the target API functions
    // 1. TIFFNumberOfStrips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    
    // 2. TIFFCurrentStrip
    uint32_t current_strip = TIFFCurrentStrip(tif);
    
    // 3. TIFFCurrentRow
    uint32_t current_row = TIFFCurrentRow(tif);
    
    // 4. TIFFWriteEncodedStrip and 5. TIFFReadEncodedStrip
    if (num_strips > 0) {
        // Try to read a strip
        uint32_t strip_to_read = current_strip % num_strips;
        tmsize_t strip_size = TIFFStripSize(tif);
        
        if (strip_size > 0) {
            void* buf = malloc(strip_size);
            if (buf) {
                // Read encoded strip
                tmsize_t read_bytes = TIFFReadEncodedStrip(tif, strip_to_read, buf, strip_size);
                
                // Try to write it back (need to switch to write mode)
                TIFF* tif_write = TIFFOpen(filename, "a");
                if (tif_write) {
                    // Set up the same tags
                    uint32_t width, height;
                    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
                    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
                    
                    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
                    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
                    TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, 
                                TIFFDefaultStripSize(tif_write, 0));
                    
                    // Write encoded strip
                    tmsize_t write_bytes = TIFFWriteEncodedStrip(tif_write, 
                                                                strip_to_read, 
                                                                buf, 
                                                                read_bytes > 0 ? read_bytes : strip_size);
                    TIFFClose(tif_write);
                }
                
                free(buf);
            }
        }
    }
    
    // 6. TIFFDefaultStripSize - test with various requests
    uint32_t default_strip_size1 = TIFFDefaultStripSize(tif, 0);
    uint32_t default_strip_size2 = TIFFDefaultStripSize(tif, 100);
    uint32_t default_strip_size3 = TIFFDefaultStripSize(tif, 10000);
    
    // Clean up
    TIFFClose(tif);
    unlink(filename);
    
    // Use the results to avoid optimization
    volatile uint32_t dummy = num_strips + current_strip + current_row + 
                             default_strip_size1 + default_strip_size2 + default_strip_size3;
    (void)dummy;
    
    return 0;
}