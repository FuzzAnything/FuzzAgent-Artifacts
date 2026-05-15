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
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
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
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t bits_per_sample = 8;
    uint16_t samples_per_pixel = 1;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint32_t rows_per_strip = 32;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    uint16_t fill_order = FILLORDER_MSB2LSB;
    uint32_t strip_offsets = 0;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    TIFFSetField(tif, TIFFTAG_FILLORDER, fill_order);
    TIFFSetField(tif, TIFFTAG_STRIPOFFSETS, &strip_offsets);
    
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size > 0) {
        void* buf = _TIFFmalloc(scanline_size);
        if (buf) {
            size_t copy_size = Size > (size_t)scanline_size ? (size_t)scanline_size : Size;
            _TIFFmemcpy(buf, Data, copy_size);
            
            for (uint32_t row = 0; row < height; row++) {
                TIFFWriteScanline(tif, buf, row, 0);
            }
            
            _TIFFfree(buf);
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}