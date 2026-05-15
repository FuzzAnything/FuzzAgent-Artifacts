// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzz data to dummy file
    write_dummy_file(Data, Size);
    
    // First TIFFClose - need to open a TIFF first
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFClose(tif);
    }
    
    // TIFFReadFromUserBuffer
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t strile = 0;
        void *inbuf = malloc(Size);
        if (inbuf) {
            memcpy(inbuf, Data, Size);
            tmsize_t insize = (tmsize_t)Size;
            if (insize < 0) insize = 0; // Handle potential overflow
            
            tmsize_t outsize = 1024;
            void *outbuf = malloc(outsize);
            if (outbuf) {
                TIFFReadFromUserBuffer(tif, strile, inbuf, insize, outbuf, outsize);
                free(outbuf);
            }
            free(inbuf);
        }
        TIFFClose(tif);
    }
    
    // Another TIFFClose sequence
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFClose(tif);
    }
    
    // TIFFReadEncodedStrip
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t strip = 0;
        tmsize_t buf_size = 1024;
        void *buf = malloc(buf_size);
        if (buf) {
            TIFFReadEncodedStrip(tif, strip, buf, buf_size);
            free(buf);
        }
        TIFFClose(tif);
    }
    
    // TIFFGetStrileOffsetWithErr and TIFFGetStrileByteCountWithErr
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t strile = 0;
        int err = 0;
        
        TIFFGetStrileOffsetWithErr(tif, strile, &err);
        TIFFGetStrileByteCountWithErr(tif, strile, &err);
        
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}