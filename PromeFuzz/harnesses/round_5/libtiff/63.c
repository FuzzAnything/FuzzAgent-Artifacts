// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }

    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    tmsize_t strip_size = TIFFStripSize(tif);

    if (num_strips > 0) {
        uint32_t strip_to_read = num_strips - 1;
        if (strip_size > 0) {
            void *buf = malloc(strip_size);
            if (buf) {
                TIFFReadEncodedStrip(tif, strip_to_read, buf, strip_size);
                free(buf);
            }
        }
        
        if (num_strips > 1) {
            strip_to_read = 0;
            if (strip_size > 0) {
                void *buf = malloc(strip_size);
                if (buf) {
                    TIFFReadEncodedStrip(tif, strip_to_read, buf, strip_size);
                    free(buf);
                }
            }
        }
    }

    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}