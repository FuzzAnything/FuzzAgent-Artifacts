// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Test TIFFScanlineSize
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Test TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Test TIFFRasterScanlineSize
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    
    // Test TIFFNumberOfStrips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    
    // Test TIFFReadEncodedStrip if there are strips
    if (num_strips > 0 && strip_size > 0) {
        void *buf = malloc(strip_size);
        if (buf) {
            // Try reading different strips to explore more states
            for (uint32_t strip = 0; strip < num_strips && strip < 5; strip++) {
                TIFFReadEncodedStrip(tif, strip, buf, strip_size);
            }
            free(buf);
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}