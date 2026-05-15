// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write input data to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    
    // Get image dimensions and strip information
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 1, bitspersample = 8;
    uint32_t rowsperstrip = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    
    if (rowsperstrip == 0 || rowsperstrip > height) {
        rowsperstrip = height;
    }
    
    uint32_t stripsperimage = TIFFNumberOfStrips(tif);
    
    // Call different functions based on selector
    switch (selector % 6) {
        case 0: { // TIFFReadRawStrip
            if (stripsperimage > 0) {
                uint32_t strip = (selector >> 1) % stripsperimage;
                tmsize_t raw_size = TIFFRawStripSize(tif, strip);
                if (raw_size > 0) {
                    void* buf = malloc(raw_size);
                    if (buf) {
                        TIFFReadRawStrip(tif, strip, buf, raw_size);
                        free(buf);
                    }
                }
            }
            break;
        }
        
        case 1: { // TIFFStripSize
            tmsize_t strip_size = TIFFStripSize(tif);
            // Use strip_size to allocate buffer if needed
            if (strip_size > 0) {
                void* buf = malloc(strip_size);
                if (buf) {
                    free(buf);
                }
            }
            break;
        }
        
        case 2: { // TIFFVStripSize
            if (height > 0) {
                uint32_t nrows = (selector >> 2) % height;
                if (nrows == 0) nrows = 1;
                tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
                if (vstrip_size > 0) {
                    void* buf = malloc(vstrip_size);
                    if (buf) {
                        free(buf);
                    }
                }
            }
            break;
        }
        
        case 3: { // TIFFWriteEncodedStrip
            if (stripsperimage > 0) {
                uint32_t strip = (selector >> 3) % stripsperimage;
                tmsize_t strip_size = TIFFStripSize(tif);
                if (strip_size > 0) {
                    void* data = malloc(strip_size);
                    if (data) {
                        // Fill with some data
                        memset(data, selector, strip_size);
                        TIFFWriteEncodedStrip(tif, strip, data, strip_size);
                        free(data);
                    }
                }
            }
            break;
        }
        
        case 4: { // TIFFRawStripSize
            if (stripsperimage > 0) {
                uint32_t strip = (selector >> 4) % stripsperimage;
                tmsize_t raw_size = TIFFRawStripSize(tif, strip);
                if (raw_size > 0) {
                    void* buf = malloc(raw_size);
                    if (buf) {
                        free(buf);
                    }
                }
            }
            break;
        }
        
        case 5: { // TIFFReadEncodedStrip
            if (stripsperimage > 0) {
                uint32_t strip = (selector >> 5) % stripsperimage;
                tmsize_t strip_size = TIFFStripSize(tif);
                if (strip_size > 0) {
                    void* buf = malloc(strip_size);
                    if (buf) {
                        TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                        free(buf);
                    }
                }
            }
            break;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    cleanup_file();
    
    return 0;
}