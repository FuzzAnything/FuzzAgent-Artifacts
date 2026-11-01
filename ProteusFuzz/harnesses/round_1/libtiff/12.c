// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

/* 
 * TIFFExtendProc is defined as: typedef void (*TIFFExtendProc)(TIFF*);
 * It takes a TIFF pointer and returns void.
 */
static void dummy_extender(TIFF *tif) {
    (void)tif;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";

    // Write input data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);

    if (written != Size) {
        return 0;
    }

    // Open the file
    TIFF *tif = TIFFOpen(filename, mode);
    if (tif == NULL) {
        return 0;
    }

    // Set a custom tag extender
    TIFFExtendProc old_extender = TIFFSetTagExtender(dummy_extender);

    // Optionally reset to default
    TIFFSetTagExtender(NULL);

    // Restore the old extender to avoid side effects in subsequent runs
    TIFFSetTagExtender(old_extender);

    // Close the file
    TIFFClose(tif);

    return 0;
}