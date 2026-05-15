// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }
    
    // Try to get various TIFF fields
    uint32_t width = 0;
    uint32_t height = 0;
    uint16_t samples_per_pixel = 0;
    uint16_t bits_per_sample = 0;
    uint16_t photometric = 0;
    uint16_t compression = 0;
    uint16_t orientation = 0;
    uint32_t rows_per_strip = 0;
    float x_resolution = 0.0;
    float y_resolution = 0.0;
    
    // Call TIFFGetField multiple times with different tags
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &x_resolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &y_resolution);
    
    // Prepare some memory blocks for comparison
    static uint8_t mem_block1[256];
    static uint8_t mem_block2[256];
    static uint8_t mem_block3[128];
    static uint8_t mem_block4[128];
    
    // Initialize with some data
    for (int i = 0; i < 256; i++) {
        mem_block1[i] = i;
        mem_block2[i] = 255 - i;
    }
    for (int i = 0; i < 128; i++) {
        mem_block3[i] = i * 2;
        mem_block4[i] = i * 2;
    }
    
    // Call _TIFFmemcmp three times with different comparisons
    _TIFFmemcmp(mem_block1, mem_block2, 256);
    _TIFFmemcmp(mem_block3, mem_block4, 128);
    
    // Third comparison: use part of fuzzer input if available
    if (Size > 0) {
        size_t cmp_size = Size > 100 ? 100 : Size;
        _TIFFmemcmp(Data, mem_block1, (tmsize_t)cmp_size);
    } else {
        _TIFFmemcmp(mem_block1, mem_block1, 256);
    }
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}