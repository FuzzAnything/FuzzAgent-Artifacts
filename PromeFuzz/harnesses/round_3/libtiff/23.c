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
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
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

static void dummyExtender(TIFF* tif) {
    // Empty tag extender function
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Set tag extender
    TIFFExtendProc prevExtender = TIFFSetTagExtender(dummyExtender);
    
    // Call TIFFGetField four times with different tags
    uint32_t width = 0, height = 0;
    uint16_t samples = 0, bits = 0;
    float xres = 0.0f, yres = 0.0f;
    char *software = NULL;
    
    // First call - try to get image dimensions
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    
    // Second call - try to get image length
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Third call - try to get samples per pixel
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    
    // Fourth call - try to get bits per sample
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
    
    // Additional calls for more coverage (beyond the required 4)
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
    TIFFGetField(tif, TIFFTAG_SOFTWARE, &software);
    
    // Restore previous tag extender
    TIFFSetTagExtender(prevExtender);
    
    // Close TIFF file
    TIFFClose(tif);
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}