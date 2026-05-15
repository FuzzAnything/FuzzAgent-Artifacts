// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
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
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "libtiff/tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint32_t rows_per_strip = TIFFDefaultStripSize(tif, width);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    uint8_t *buf = (uint8_t *)_TIFFmalloc(scanline_size);
    if (buf) {
        memset(buf, 0, scanline_size);
        size_t data_offset = 0;
        for (uint32_t row = 0; row < height; row++) {
            if (data_offset + scanline_size <= Size) {
                memcpy(buf, Data + data_offset, scanline_size);
                data_offset += scanline_size;
            }
            TIFFWriteScanline(tif, buf, row, 0);
        }
        _TIFFfree(buf);
    }
    
    TIFFWriteDirectory(tif);
    
    TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
    if (close_proc) {
        void* clientdata = TIFFClientdata(tif);
        close_proc(clientdata);
    }
    
    TIFFFreeDirectory(tif);
    
    TIFFCleanup(tif);
    
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}