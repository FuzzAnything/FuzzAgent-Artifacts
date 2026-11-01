// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

/* 
 * TIFFExtendProc is defined as: void (*TIFFExtendProc)(TIFF *tif)
 * The function should only take a TIFF pointer.
 */
static void dummy_extender(TIFF *tif) {
    (void)tif;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    const char *mode = "w";
    TIFF *tif = NULL;
    TIFFExtendProc old_extender = NULL;

    if (Size < 1) {
        return 0;
    }

    // Write input data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Try opening the file
    tif = TIFFOpen(filename, mode);
    if (tif == NULL) {
        // If open fails, we can still test TIFFSetTagExtender and TIFFClose(NULL)
        old_extender = TIFFSetTagExtender(dummy_extender);
        TIFFSetTagExtender(old_extender); // Reset
        TIFFClose(NULL);
        return 0;
    }

    // Invoke TIFFSetTagExtender
    old_extender = TIFFSetTagExtender(dummy_extender);

    // Optionally reset to default or previous
    TIFFSetTagExtender(old_extender);

    // Close the file
    TIFFClose(tif);

    // Test closing NULL explicitly
    TIFFClose(NULL);

    return 0;
}