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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty to avoid immediate crash on open
            // Basic header: II (little endian), 42 (magic), 8 (offset to first IFD)
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare the environment
    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    char errbuf[1024];
    memset(errbuf, 0, sizeof(errbuf));

    // Step 2: Invoke target functions in order
    // 1. TIFFIsTiled
    int is_tiled = TIFFIsTiled(tif);
    (void)is_tiled; // Use the result to avoid unused variable warning, logic handled by fuzzer state

    // 2. TIFFRGBAImageBegin
    // We pass 0 for the 'stoponerr' flag to allow exploration of error paths
    int begin_result = TIFFRGBAImageBegin(&img, tif, 0, errbuf);

    // 3. TIFFRGBAImageEnd
    // Always call end to clean up, regardless of begin success
    TIFFRGBAImageEnd(&img);

    // Cleanup
    TIFFClose(tif);

    return 0;
}