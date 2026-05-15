// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = *(uint32_t*)Data;
    uint32_t height = *((uint32_t*)Data + 1);
    uint16_t bitspersample = (uint16_t)(*(Data + 8) % 16 + 1);
    uint16_t samplesperpixel = (uint16_t)(*(Data + 9) % 4 + 1);
    uint16_t compression = (uint16_t)(*(Data + 10) % 6 + 1);
    uint16_t photometric = (uint16_t)(*(Data + 11) % 6);
    uint16_t planarconfig = (uint16_t)(*(Data + 12) % 2 + 1);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    
    uint32_t strip = 0;
    size_t data_offset = 20;
    if (Size > data_offset) {
        size_t write_size = Size - data_offset;
        if (write_size > 0xFFFF) write_size = 0xFFFF;
        TIFFWriteRawStrip(tif, strip, (void*)(Data + data_offset), write_size);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t read_width = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &read_width);
    
    if (read_width > 0 && read_width < 10000) {
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0 && scanline_size < 100000) {
            void *buf = _TIFFmalloc(scanline_size);
            if (buf) {
                TIFFReadScanline(tif, buf, 0, 0);
                _TIFFfree(buf);
            }
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}