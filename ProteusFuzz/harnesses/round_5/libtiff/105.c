// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/*
 * Fuzz driver for libtiff API functions:
 * - TIFFCreateGPSDirectory
 * - TIFFWriteDirectory
 * - TIFFFreeDirectory
 * - TIFFCreateCustomDirectory
 * - TIFFCreateEXIFDirectory
 * - TIFFSetDirectory
 *
 * This driver creates a temporary file, opens it for writing, and exercises
 * the target functions with various inputs derived from the fuzzing data.
 */

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Write input data to a dummy file to simulate a real TIFF file */
    write_dummy_file(Data, Size);

    /* Open the file for writing */
    TIFF *tif = TIFFOpen("./dummy_file", "w8");
    if (tif == NULL) {
        return 0;
    }

    /* Set some basic image properties to ensure a valid directory can be written */
    /* Ensure bounds checking before accessing Data array */
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)(Data[0] % 100 + 1));
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)((Data[1] % 100) + 1));
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

    /* Attempt to create GPS directory */
    TIFFCreateGPSDirectory(tif);

    /* Attempt to create EXIF directory */
    TIFFCreateEXIFDirectory(tif);

    /* Write the directory */
    TIFFWriteDirectory(tif);

    /* Try to set directory to index 0 */
    TIFFSetDirectory(tif, 0);

    /* Free the directory */
    TIFFFreeDirectory(tif);

    /* Close the TIFF file */
    TIFFClose(tif);

    /* Clean up the dummy file */
    remove("./dummy_file");

    return 0;
}