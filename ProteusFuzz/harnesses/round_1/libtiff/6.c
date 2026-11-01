// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write fuzzing data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Step 1: Invoke TIFFIsTiled
    int is_tiled = TIFFIsTiled(tif);
    (void)is_tiled; // Suppress unused variable warning

    // Step 2: Invoke TIFFRGBAImageBegin
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    char err[1024];
    memset(err, 0, sizeof(err));

    int begin_ok = TIFFRGBAImageBegin(&img, tif, 0, err);
    
    // Step 3: Invoke TIFFRGBAImageEnd
    // Always call this to clean up, regardless of whether Begin succeeded
    TIFFRGBAImageEnd(&img);

    // Cleanup
    TIFFClose(tif);

    return 0;
}