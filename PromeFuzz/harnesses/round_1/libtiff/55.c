// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_cleanup(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Create dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);
    
    // Open TIFF file for reading
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        dummy_cleanup();
        return 0;
    }
    
    // Test TIFFNumberOfStrips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    
    // Test TIFFDefaultStripSize with various requests
    uint32_t default_strip_sizes[] = {0, 100, 1000, 8192, 65535};
    for (size_t i = 0; i < sizeof(default_strip_sizes)/sizeof(default_strip_sizes[0]); i++) {
        TIFFDefaultStripSize(tif, default_strip_sizes[i]);
    }
    
    // Test TIFFCurrentRow
    uint32_t current_row = TIFFCurrentRow(tif);
    
    // Test TIFFCurrentStrip
    uint32_t current_strip = TIFFCurrentStrip(tif);
    
    // Test TIFFReadEncodedStrip if we have strips
    if (num_strips > 0) {
        // Try reading different strips
        for (uint32_t strip = 0; strip < num_strips && strip < 10; strip++) {
            // Get strip size
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0) {
                void* buf = malloc(strip_size);
                if (buf) {
                    // Try reading with different sizes
                    TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                    TIFFReadEncodedStrip(tif, strip, buf, strip_size/2);
                    TIFFReadEncodedStrip(tif, strip, buf, -1); // Read full strip
                    free(buf);
                }
            }
        }
    }
    
    // Close and reopen for writing
    TIFFClose(tif);
    tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        dummy_cleanup();
        return 0;
    }
    
    // Set up minimal TIFF directory for writing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // Test TIFFWriteEncodedStrip
    num_strips = TIFFNumberOfStrips(tif);
    if (num_strips > 0) {
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0) {
            void* write_buf = malloc(strip_size);
            if (write_buf) {
                // Initialize with some data
                memset(write_buf, 0xAA, strip_size);
                
                // Try writing different strips
                for (uint32_t strip = 0; strip < num_strips && strip < 5; strip++) {
                    TIFFWriteEncodedStrip(tif, strip, write_buf, strip_size);
                    TIFFWriteEncodedStrip(tif, strip, write_buf, strip_size/2);
                }
                free(write_buf);
            }
        }
    }
    
    // Test other functions again in write mode
    TIFFDefaultStripSize(tif, 8192);
    TIFFCurrentRow(tif);
    TIFFCurrentStrip(tif);
    TIFFNumberOfStrips(tif);
    
    // Clean up
    TIFFClose(tif);
    dummy_cleanup();
    
    return 0;
}