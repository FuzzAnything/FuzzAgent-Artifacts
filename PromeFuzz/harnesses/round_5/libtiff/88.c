// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint16_t samplesperpixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    uint32_t tilewidth = 0, tilelength = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength);
    
    uint32_t rowsperstrip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    
    uint32_t imagewidth = width;
    uint32_t imagelength = height;
    
    if (imagewidth == 0) imagewidth = 100;
    if (imagelength == 0) imagelength = 100;
    if (rowsperstrip == 0) rowsperstrip = 1;
    
    for (int i = 0; i < 10 && i < Size; i++) {
        uint32_t x = (Data[i] * 17) % (imagewidth + 100);
        uint32_t y = (Data[i] * 23) % (imagelength + 100);
        uint32_t z = (Data[i] * 29) % 10;
        uint16_t s = (Data[i] * 31) % (samplesperpixel + 2);
        
        TIFFComputeTile(tif, x, y, z, s);
    }
    
    uint32_t strile_count = TIFFNumberOfStrips(tif);
    if (strile_count == 0) strile_count = 10;
    
    for (int i = 0; i < 5 && i < Size; i++) {
        uint32_t strile = (Data[i] * 37) % (strile_count + 10);
        int err = 0;
        TIFFGetStrileByteCountWithErr(tif, strile, &err);
    }
    
    for (int i = 0; i < 10 && i < Size; i++) {
        uint32_t row = (Data[i] * 41) % (imagelength + 100);
        uint16_t sample = (Data[i] * 43) % (samplesperpixel + 2);
        TIFFComputeStrip(tif, row, sample);
    }
    
    TIFFCurrentStrip(tif);
    
    TIFFNumberOfTiles(tif);
    
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count == 0) strip_count = 5;
    
    for (int i = 0; i < 3 && i < Size; i++) {
        uint32_t strip = (Data[i] * 47) % (strip_count + 5);
        tmsize_t bufsize = 1024;
        void* buf = malloc(bufsize);
        if (buf) {
            TIFFReadEncodedStrip(tif, strip, buf, bufsize);
            free(buf);
        }
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}