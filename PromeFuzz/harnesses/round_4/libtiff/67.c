// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t nstrips = TIFFNumberOfStrips(tif);
    
    if (nstrips > 0) {
        tmsize_t strip_size = TIFFStripSize(tif);
        
        if (strip_size > 0 && strip_size < 1024*1024) {
            void* buf = malloc(strip_size);
            if (buf) {
                for (uint32_t strip = 0; strip < nstrips && strip < 4; strip++) {
                    TIFFReadRawStrip(tif, strip, buf, strip_size);
                    TIFFReadEncodedStrip(tif, strip, buf, strip_size);
                }
                free(buf);
            }
        }
        
        tmsize_t scanline_size = TIFFRasterScanlineSize(tif);
        
        if (nstrips < 100) {
            for (uint32_t strip = 0; strip < nstrips && strip < 2; strip++) {
                tmsize_t strip_size2 = TIFFStripSize(tif);
                if (strip_size2 > 0 && strip_size2 < 1024*1024) {
                    void* buf2 = malloc(strip_size2);
                    if (buf2) {
                        TIFFWriteEncodedStrip(tif, strip, buf2, strip_size2);
                        free(buf2);
                    }
                }
            }
        }
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    return 0;
}