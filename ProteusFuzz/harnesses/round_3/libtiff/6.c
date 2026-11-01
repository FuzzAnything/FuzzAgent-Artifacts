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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static int write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return 0;
    }
    size_t written = fwrite(Data, 1, Size, f);
    fclose(f);
    return (written == Size) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure we have enough data to create a minimal valid-ish TIFF header */
    if (Size < 8) {
        return 0;
    }

    /* Write input data to a temporary file */
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    /* Open the TIFF file */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Step 1: Invoke TIFFIsTiled */
    int is_tiled = TIFFIsTiled(tif);
    (void)is_tiled; /* Suppress unused variable warning */

    /* Step 2: Initialize TIFFRGBAImage */
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    char err[1024];
    memset(err, 0, sizeof(err));

    /* Attempt to begin RGBA image processing */
    int begin_result = TIFFRGBAImageBegin(&img, tif, 0, err);
    
    if (begin_result != 0) {
        /* Step 3: Invoke TIFFRGBAImageEnd if begin succeeded */
        TIFFRGBAImageEnd(&img);
    }

    /* Cleanup: Close the TIFF file */
    TIFFClose(tif);

    return 0;
}