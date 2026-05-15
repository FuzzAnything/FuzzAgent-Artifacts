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
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "w");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t bitspersample = 8;
    uint16_t samplesperpixel = 3;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t planarconfig = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    uint32_t rowsperstrip = 10;
    uint16_t fillorder = FILLORDER_MSB2LSB;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    TIFFSetField(tif, TIFFTAG_FILLORDER, fillorder);

    tmsize_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size <= 0) {
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }

    void *buf = _TIFFmalloc(scanline_size);
    if (!buf) {
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }

    size_t copy_size = Size > (size_t)scanline_size ? (size_t)scanline_size : Size;
    _TIFFmemcpy(buf, Data, copy_size);

    for (uint32_t row = 0; row < height; row++) {
        TIFFWriteScanline(tif, buf, row, 0);
    }

    _TIFFfree(buf);
    TIFFClose(tif);
    unlink(filename);

    return 0;
}

#ifdef __cplusplus
}
#endif