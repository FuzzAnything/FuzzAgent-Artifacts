// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }

    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 1, bitspersample = 8;
    uint32_t rowsperstrip = 0;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    if (rowsperstrip == 0 || rowsperstrip > height) {
        rowsperstrip = height;
    }

    uint32_t num_strips = TIFFNumberOfStrips(tif);
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    uint32_t current_strip = TIFFCurrentStrip(tif);
    
    if (height > 0 && samplesperpixel > 0) {
        uint32_t row = height > 1 ? 1 : 0;
        uint16_t sample = samplesperpixel > 1 ? 1 : 0;
        uint32_t computed_strip = TIFFComputeStrip(tif, row, sample);
        (void)computed_strip;
    }
    
    if (num_strips > 0) {
        uint32_t strip_index = num_strips > 1 ? 1 : 0;
        int err = 0;
        uint64_t byte_count = TIFFGetStrileByteCountWithErr(tif, strip_index, &err);
        (void)byte_count;
        
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0) {
            void* buf = malloc(strip_size);
            if (buf) {
                tmsize_t read_size = TIFFReadEncodedStrip(tif, strip_index, buf, strip_size);
                (void)read_size;
                free(buf);
            }
        }
    }
    
    if (num_tiles > 0 && width > 0 && height > 0) {
        uint32_t x = width > 1 ? 1 : 0;
        uint32_t y = height > 1 ? 1 : 0;
        uint32_t z = 0;
        uint16_t s = samplesperpixel > 1 ? 1 : 0;
        uint32_t tile_index = TIFFComputeTile(tif, x, y, z, s);
        (void)tile_index;
    }
    
    uint32_t tile_count = TIFFNumberOfTiles(tif);
    (void)tile_count;
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}