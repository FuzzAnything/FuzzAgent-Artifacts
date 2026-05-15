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
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_extender(TIFF* tif) {
    /* Dummy tag extender that does nothing */
    (void)tif;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    /* Write fuzzer input to dummy file */
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    /* Open TIFF file */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }
    
    /* Set tag extender */
    TIFFExtendProc prev_extender = TIFFSetTagExtender(dummy_extender);
    (void)prev_extender; /* Mark as used to avoid warning */
    
    /* Variables for TIFFGetField */
    uint32_t width = 0;
    uint32_t height = 0;
    uint16_t bits_per_sample = 0;
    uint16_t compression = 0;
    uint16_t samples_per_pixel = 0;
    uint16_t photometric = 0;
    uint32_t rows_per_strip = 0;
    uint16_t planar_config = 0;
    float x_resolution = 0.0f;
    float y_resolution = 0.0f;
    
    /* Call TIFFGetField four times with different tags */
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    
    /* Additional TIFFGetField calls to explore more states */
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &x_resolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &y_resolution);
    
    /* Clean up */
    TIFFClose(tif);
    
    /* Remove dummy file */
    remove("./dummy_file");
    
    return 0;
}