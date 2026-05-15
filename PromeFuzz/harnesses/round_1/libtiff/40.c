// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void cleanup_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        remove(filename);
        return 0;
    }

    uint32_t num_strips = TIFFNumberOfStrips(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);

    if (num_strips > 0 && strip_size > 0 && strip_size < 1024*1024) {
        void* buffer = malloc(strip_size);
        if (buffer) {
            for (uint32_t strip = 0; strip < num_strips && strip < 10; strip++) {
                tmsize_t read_bytes = TIFFReadEncodedStrip(tif, strip, buffer, strip_size);
                if (read_bytes > 0) {
                    TIFFWriteEncodedStrip(tif, strip, buffer, read_bytes);
                }
            }
            free(buffer);
        }
    }

    cleanup_tiff(tif);
    remove(filename);
    return 0;
}