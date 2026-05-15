// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    uint32_t current_strip = TIFFCurrentStrip(tif);
    uint32_t current_row = TIFFCurrentRow(tif);
    uint32_t current_tile = TIFFCurrentTile(tif);
    
    if (strip_count > 0) {
        for (uint32_t strip = 0; strip < strip_count && strip < 10; strip++) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && strip_size < 1024 * 1024) {
                void *buf = malloc(strip_size);
                if (buf) {
                    TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                    free(buf);
                }
            }
            TIFFReadEncodedStrip(tif, strip, NULL, -1);
        }
    }
    
    if (width > 0 && height > 0) {
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0 && scanline_size < 1024 * 1024) {
            void *scanline_buf = malloc(scanline_size);
            if (scanline_buf) {
                memset(scanline_buf, 0, scanline_size);
                for (uint32_t row = 0; row < height && row < 10; row++) {
                    TIFFReadScanline(tif, scanline_buf, row, 0);
                }
                free(scanline_buf);
            }
        }
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "a");
    if (tif) {
        if (width > 0 && height > 0) {
            tmsize_t scanline_size = TIFFScanlineSize(tif);
            if (scanline_size > 0 && scanline_size < 1024 * 1024) {
                void *write_buf = malloc(scanline_size);
                if (write_buf) {
                    memset(write_buf, 0xAA, scanline_size);
                    for (uint32_t row = 0; row < 5 && row < height; row++) {
                        TIFFWriteScanline(tif, write_buf, row, 0);
                    }
                    free(write_buf);
                }
            }
        }
        
        current_strip = TIFFCurrentStrip(tif);
        current_row = TIFFCurrentRow(tif);
        current_tile = TIFFCurrentTile(tif);
        strip_count = TIFFNumberOfStrips(tif);
        
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}