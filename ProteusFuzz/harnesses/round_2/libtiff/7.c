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
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    /* Sequence of TIFFSetField calls */
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    TIFFWriteDirectory(tif);

    if (TIFFCreateEXIFDirectory(tif)) {
        /* Use a generic tag or a valid EXIF tag that exists in tiffio.h. 
           Using TIFFTAG_MAKE as a placeholder for EXIF metadata or a standard tag. */
        TIFFSetField(tif, TIFFTAG_MAKE, "Fuzzer");
    }

    TIFFFreeDirectory(tif);

    /* TIFFCreateCustomDirectory requires a valid TIFFFieldArray pointer. 
       Passing NULL causes a segmentation fault as seen in the error log.
       We skip this call or pass a valid structure if available. 
       Since we don't have a custom array defined here, we skip the call to avoid the crash. */
    // TIFFCreateCustomDirectory(tif, NULL); 
    
    /* Set some fields after freeing directory, though behavior depends on libtiff state */
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 32);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 32);
    
    /* Attempt to set directory. Index 0 is usually valid for the first IFD. */
    TIFFSetDirectory(tif, 0);

    TIFFClose(tif);

    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFClose(tif);
    }

    TIFFClose(NULL);

    return 0;
}