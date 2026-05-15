// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // First TIFFClose sequence
    TIFF *tif1 = TIFFOpen("./dummy_file", "r");
    if (tif1) {
        TIFFClose(tif1);
    }
    
    // TIFFReadFromUserBuffer sequence
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        uint32_t strile = 0;
        tmsize_t insize = Size > 1024 ? 1024 : Size;
        tmsize_t outsize = 1024;
        void *inbuf = malloc(insize);
        void *outbuf = malloc(outsize);
        
        if (inbuf && outbuf && Size > 0) {
            memcpy(inbuf, Data, insize);
            TIFFReadFromUserBuffer(tif2, strile, inbuf, insize, outbuf, outsize);
        }
        
        free(inbuf);
        free(outbuf);
        TIFFClose(tif2);
    }
    
    // TIFFReadEncodedStrip sequence
    TIFF *tif3 = TIFFOpen("./dummy_file", "r");
    if (tif3) {
        uint32_t strip = 0;
        tmsize_t size = TIFFStripSize(tif3);
        void *buf = NULL;
        
        if (size > 0) {
            buf = malloc(size);
            if (buf) {
                TIFFReadEncodedStrip(tif3, strip, buf, size);
                free(buf);
            }
        }
        TIFFClose(tif3);
    }
    
    // TIFFGetStrileOffsetWithErr and TIFFGetStrileByteCountWithErr sequence
    TIFF *tif4 = TIFFOpen("./dummy_file", "r");
    if (tif4) {
        uint32_t strile = 0;
        int err1 = 0, err2 = 0;
        
        TIFFGetStrileOffsetWithErr(tif4, strile, &err1);
        TIFFGetStrileByteCountWithErr(tif4, strile, &err2);
        
        TIFFClose(tif4);
    }
    
    cleanup_dummy_file();
    return 0;
}