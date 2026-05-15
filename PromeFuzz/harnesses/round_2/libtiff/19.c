// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) return 0;
    
    uint32_t width = *(uint32_t*)Data;
    uint32_t height = *(uint32_t*)(Data + 4);
    uint16_t bitspersample = *(uint16_t*)(Data + 8);
    uint16_t samplesperpixel = *(uint16_t*)(Data + 10);
    uint16_t photometric = *(uint16_t*)(Data + 12);
    uint16_t planarconfig = *(uint16_t*)(Data + 14);
    uint32_t rowsperstrip = *(uint32_t*)(Data + 16);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    
    if (rowsperstrip > 0) {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    } else {
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    }
    
    size_t strip_data_size = Size - 20;
    if (strip_data_size > 0) {
        void* strip_data = (void*)(Data + 20);
        TIFFWriteRawStrip(tif, 0, strip_data, strip_data_size);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t retrieved_width = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &retrieved_width);
    
    if (retrieved_width > 0 && height > 0 && samplesperpixel > 0 && bitspersample > 0) {
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0 && scanline_size < 1024*1024) {
            void* buf = malloc(scanline_size);
            if (buf) {
                TIFFReadScanline(tif, buf, 0, 0);
                free(buf);
            }
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}