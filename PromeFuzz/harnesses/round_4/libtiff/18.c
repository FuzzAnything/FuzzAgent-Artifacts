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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) return 0;
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    uint32_t rows_per_strip = 10;
    uint16_t sample_format = SAMPLEFORMAT_UINT;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sample_format);
    
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size <= 0) {
        TIFFClose(tif);
        return 0;
    }
    
    uint8_t* buffer = (uint8_t*)_TIFFmalloc(scanline_size);
    if (!buffer) {
        TIFFClose(tif);
        return 0;
    }
    
    memset(buffer, 0, scanline_size);
    size_t copy_size = Size > scanline_size ? scanline_size : Size;
    _TIFFmemcpy(buffer, Data, copy_size);
    
    for (uint32_t row = 0; row < height; row++) {
        TIFFWriteScanline(tif, buffer, row, 0);
    }
    
    _TIFFfree(buffer);
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}