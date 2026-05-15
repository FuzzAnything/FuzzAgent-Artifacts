// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libtiff/tiffio.h"

static void dummy_extender(TIFF* tif) {
    // Dummy tag extender function
    (void)tif;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Set tag extender
    TIFFExtendProc prev_extender = TIFFSetTagExtender(dummy_extender);
    
    // Variables for TIFFGetField calls
    uint32_t width = 0, height = 0;
    uint16_t bits_per_sample = 0, samples_per_pixel = 0;
    uint16_t photometric = 0, compression = 0;
    uint32_t rows_per_strip = 0;
    float x_resolution = 0.0, y_resolution = 0.0;
    
    // Call TIFFGetField four times with different tags
    // 1. Get basic image dimensions
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // 2. Get sample information
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    
    // 3. Get photometric and compression
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    
    // 4. Get resolution and strip info
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &x_resolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &y_resolution);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    // Restore previous tag extender
    TIFFSetTagExtender(prev_extender);
    
    // Close the TIFF file
    TIFFClose(tif);
    
    // Clean up temporary file
    remove("./dummy_file");
    
    return 0;
}