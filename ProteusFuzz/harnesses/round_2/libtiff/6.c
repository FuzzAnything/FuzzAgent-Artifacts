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

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) {
        return 0;
    }

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Step 1: Invoke TIFFIsTiled
    int is_tiled = TIFFIsTiled(tif);
    (void)is_tiled; // Use the return value to suppress unused variable warnings

    // Step 2: Prepare and invoke TIFFRGBAImageBegin
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    char errbuf[1024];
    memset(errbuf, 0, sizeof(errbuf));

    // Try with stoponerr = 0 (default)
    int begin_result = TIFFRGBAImageBegin(&img, tif, 0, errbuf);
    (void)begin_result; // Suppress unused variable warning

    // Step 3: Invoke TIFFRGBAImageEnd
    // This should be called regardless of whether TIFFRGBAImageBegin succeeded
    // to ensure proper cleanup, as it handles NULL pointers safely.
    TIFFRGBAImageEnd(&img);

    // Cleanup: Close the TIFF file
    TIFFClose(tif);

    return 0;
}