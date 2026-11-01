// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

/* 
 * TIFFExtendProc is defined in libtiff as:
 * typedef void (*TIFFExtendProc)(TIFF*);
 * It takes a TIFF pointer and returns void.
 */
static void dummy_extender(TIFF *tif) {
    (void)tif;
    return;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";

    // Write input data to the dummy file to ensure it exists and has content
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, mode);
    if (tif == NULL) {
        return 0;
    }

    // Set a custom tag extender
    // Pass our dummy function which matches the signature void (*)(TIFF*)
    TIFFExtendProc old_extender = TIFFSetTagExtender(dummy_extender);

    // Close the TIFF file to clean up
    TIFFClose(tif);

    // Reset the extender to default (NULL) to clean up global state
    TIFFSetTagExtender(NULL);

    // Suppress unused variable warning
    (void)old_extender;

    return 0;
}