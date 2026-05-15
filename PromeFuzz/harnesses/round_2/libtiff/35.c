// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
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
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
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

static void cleanup_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

static TIFF* create_test_tiff(const char* filename, const uint8_t* data, size_t size) {
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) return NULL;
    
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint32_t rows_per_strip = 16;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    
    size_t scanline_size = TIFFScanlineSize(tif);
    uint8_t* buf = (uint8_t*)_TIFFmalloc(scanline_size);
    if (!buf) {
        TIFFClose(tif);
        return NULL;
    }
    
    for (uint32_t row = 0; row < height; row++) {
        memset(buf, row % 256, scanline_size);
        if (size > 0) {
            size_t copy_size = (scanline_size < size) ? scanline_size : size;
            memcpy(buf, data + (row % (size / copy_size + 1)), copy_size);
        }
        TIFFWriteScanline(tif, buf, row, 0);
    }
    
    _TIFFfree(buf);
    TIFFClose(tif);
    
    tif = TIFFOpen(filename, "r");
    return tif;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    const char* filename = "./dummy_file";
    
    if (Size < 1) {
        return 0;
    }
    
    TIFF* tif = create_test_tiff(filename, Data, Size);
    if (!tif) {
        return 0;
    }
    
    uint32_t strip = 0;
    uint32_t row = 0;
    uint16_t sample = 0;
    
    if (Size >= sizeof(uint32_t)) {
        memcpy(&strip, Data, sizeof(uint32_t));
        strip = strip % (TIFFNumberOfStrips(tif) + 1);
    }
    
    if (Size >= 2 * sizeof(uint32_t)) {
        memcpy(&row, Data + sizeof(uint32_t), sizeof(uint32_t));
        uint32_t image_length;
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &image_length);
        row = row % (image_length + 1);
    }
    
    if (Size >= 2 * sizeof(uint32_t) + sizeof(uint16_t)) {
        memcpy(&sample, Data + 2 * sizeof(uint32_t), sizeof(uint16_t));
        uint16_t samples_per_pixel;
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
        sample = sample % (samples_per_pixel + 1);
    }
    
    uint32_t current_strip = TIFFCurrentStrip(tif);
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    uint32_t current_row = TIFFCurrentRow(tif);
    uint32_t current_tile = TIFFCurrentTile(tif);
    
    tmsize_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0 && num_strips > 0) {
        uint8_t* buf = (uint8_t*)_TIFFmalloc(strip_size);
        if (buf) {
            tmsize_t read_size = -1;
            if (Size >= 3 * sizeof(uint32_t) + sizeof(uint16_t)) {
                memcpy(&read_size, Data + 2 * sizeof(uint32_t) + sizeof(uint16_t), sizeof(tmsize_t));
                if (read_size > strip_size) {
                    read_size = strip_size;
                }
            }
            TIFFReadEncodedStrip(tif, strip % num_strips, buf, read_size);
            _TIFFfree(buf);
        }
    }
    
    uint8_t* scanline_buf = (uint8_t*)_TIFFmalloc(TIFFScanlineSize(tif));
    if (scanline_buf && Size > 0) {
        size_t copy_size = TIFFScanlineSize(tif);
        if (copy_size > Size) copy_size = Size;
        memcpy(scanline_buf, Data, copy_size);
        TIFFWriteScanline(tif, scanline_buf, row, sample);
        _TIFFfree(scanline_buf);
    }
    
    current_strip = TIFFCurrentStrip(tif);
    current_row = TIFFCurrentRow(tif);
    current_tile = TIFFCurrentTile(tif);
    
    cleanup_tiff(tif);
    unlink(filename);
    
    return 0;
}