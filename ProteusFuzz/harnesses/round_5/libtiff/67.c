// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal TIFF header if input is empty to ensure valid structure
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open the file in read-write mode to allow directory manipulation
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as write-only if read+write fails (e.g., file might be too small/corrupt for read)
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    // 1. TIFFAccessTagMethods
    // Requires valid TIFF*. If tif is valid, this returns the internal struct.
    // We do not dereference the result to avoid undefined behavior if the internal state is corrupt.
    TIFFTagMethods *methods = TIFFAccessTagMethods(tif);
    (void)methods; // Suppress unused variable warning

    // 2. TIFFCreateCustomDirectory
    // Requires a valid TIFFFieldArray. We cannot easily construct a full valid one from fuzz data
    // without risking crashes in libtiff internals due to complex dependencies.
    // Instead, we test TIFFCreateEXIFDirectory which uses a built-in array.
    int ret_exif = TIFFCreateEXIFDirectory(tif);
    (void)ret_exif;

    // 3. TIFFRewriteDirectory
    // Rewrites the current directory. May fail if directory is empty or invalid, which is expected.
    int ret_rewrite = TIFFRewriteDirectory(tif);
    (void)ret_rewrite;

    // 4. TIFFFreeDirectory
    // Frees memory for the current directory.
    TIFFFreeDirectory(tif);

    // 5. TIFFCleanup
    // The documentation states: "Only call TIFFCleanup(tif) if TIFFClose(tif) was not called".
    // We have not called TIFFClose yet. We call cleanup here to test resource release logic.
    // Note: After cleanup, the tif handle should not be used for other operations.
    TIFFCleanup(tif);

    // Do not call TIFFClose after TIFFCleanup as per documentation to avoid double-free.
    
    return 0;
}