// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void dummy_extender(TIFF *tif) {
    (void)tif;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";

    // Write input data to the dummy file
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    TIFF *tif = TIFFOpen(filename, mode);
    if (tif == NULL) {
        return 0;
    }

    // Invoke TIFFSetTagExtender with a dummy function
    TIFFExtendProc old_extender = TIFFSetTagExtender(dummy_extender);
    
    // Optionally reset to default
    TIFFSetTagExtender(NULL);

    TIFFClose(tif);

    return 0;
}