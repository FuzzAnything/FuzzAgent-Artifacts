// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
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
#include "tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Get basic image information
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 1;
    uint16_t bitspersample = 8;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test TIFFCurrentStrip
    uint32_t current_strip = TIFFCurrentStrip(tif);
    
    // Test TIFFComputeStrip with various parameters
    if (height > 0 && samplesperpixel > 0) {
        uint32_t row = height > 1 ? 1 : 0;
        uint16_t sample = samplesperpixel > 1 ? 1 : 0;
        uint32_t strip = TIFFComputeStrip(tif, row, sample);
        
        // Test TIFFReadEncodedStrip if we have a valid strip
        if (strip < TIFFNumberOfStrips(tif)) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && strip_size < 1024 * 1024) { // Limit to 1MB
                void* buf = malloc(strip_size);
                if (buf) {
                    TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                    free(buf);
                }
            }
        }
    }
    
    // Test TIFFComputeTile with various parameters
    if (width > 0 && height > 0) {
        uint32_t x = width > 1 ? 1 : 0;
        uint32_t y = height > 1 ? 1 : 0;
        uint32_t z = 0;
        uint16_t s = samplesperpixel > 1 ? 1 : 0;
        uint32_t tile = TIFFComputeTile(tif, x, y, z, s);
        
        // Test TIFFGetStrileByteCountWithErr for both strips and tiles
        int err = 0;
        if (TIFFIsTiled(tif)) {
            if (tile < num_tiles) {
                uint64_t byte_count = TIFFGetStrileByteCountWithErr(tif, tile, &err);
            }
        } else {
            if (current_strip < TIFFNumberOfStrips(tif)) {
                uint64_t byte_count = TIFFGetStrileByteCountWithErr(tif, current_strip, &err);
            }
        }
    }
    
    // Additional tests with different parameters
    if (height > 10) {
        // Test TIFFComputeStrip with middle row
        uint32_t mid_row = height / 2;
        uint16_t sample = 0;
        TIFFComputeStrip(tif, mid_row, sample);
        
        // Test TIFFComputeTile with middle coordinates
        if (width > 10) {
            uint32_t mid_x = width / 2;
            uint32_t mid_y = height / 2;
            TIFFComputeTile(tif, mid_x, mid_y, 0, 0);
        }
    }
    
    // Test edge cases
    if (height > 0) {
        // Last row
        TIFFComputeStrip(tif, height - 1, 0);
    }
    
    if (width > 0 && height > 0) {
        // Last pixel
        TIFFComputeTile(tif, width - 1, height - 1, 0, 0);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}