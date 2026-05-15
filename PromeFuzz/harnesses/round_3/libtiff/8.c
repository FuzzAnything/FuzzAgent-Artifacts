// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsBigTIFF at tif_open.c:907:5 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Write fuzz data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Clean up dummy file
        remove("./dummy_file");
        return 0;
    }
    
    // Call functions in specified order
    int isBigTIFF = TIFFIsBigTIFF(tif);
    
    TIFFSeekProc seekProc = TIFFGetSeekProc(tif);
    
    TIFFReadWriteProc readProc = TIFFGetReadProc(tif);
    
    int isSwapped = TIFFIsByteSwapped(tif);
    
    // For TIFFSwabShort, we need a uint16_t value
    // Use some data from the input if available, otherwise use a default
    uint16_t shortVal = 0x1234;
    if (Size >= 2) {
        memcpy(&shortVal, Data, 2);
    }
    TIFFSwabShort(&shortVal);
    
    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}