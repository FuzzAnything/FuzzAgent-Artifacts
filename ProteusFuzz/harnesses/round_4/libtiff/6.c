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
#include <tiffio.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write fuzzing data to a temporary file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Step 1: Invoke TIFFIsTiled
    int is_tiled = TIFFIsTiled(tif);
    (void)is_tiled; // Use the result to avoid unused variable warnings

    // Step 2: Initialize TIFFRGBAImage
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    char err[1024];
    memset(err, 0, sizeof(err));

    // Invoke TIFFRGBAImageBegin
    // stoponerr: 0 (continue on error) or 1 (stop on error) - fuzz both
    int stoponerr = (Size > 0) ? (Data[0] % 2) : 0;
    int begin_result = TIFFRGBAImageBegin(&img, tif, stoponerr, err);

    // Step 3: Invoke TIFFRGBAImageEnd
    // Always call end to clean up, regardless of begin success
    TIFFRGBAImageEnd(&img);

    // Cleanup
    TIFFClose(tif);

    return 0;
}