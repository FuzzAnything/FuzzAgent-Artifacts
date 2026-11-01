// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsBigTIFF at tif_open.c:907:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid-ish TIFF header if input is empty to avoid immediate crash on open
            uint16_t be = 0x4949; // Little endian magic
            uint16_t version = 42;
            uint32_t offset = 8;
            fwrite(&be, sizeof(be), 1, f);
            fwrite(&version, sizeof(version), 1, f);
            fwrite(&offset, sizeof(offset), 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open for writing to allow TIFFWriteDirectory
    TIFF *tif_w = TIFFOpen("./dummy_file", "w8");
    if (!tif_w) {
        return 0;
    }

    // 1. TIFFWriteDirectory
    TIFFWriteDirectory(tif_w);

    // 2. TIFFSetDirectory
    TIFFSetDirectory(tif_w, 0);

    // 3. TIFFSetDirectory
    TIFFSetDirectory(tif_w, 1);

    // Re-open for reading to properly test read functions
    TIFFClose(tif_w);
    tif_w = NULL;

    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (!tif_r) {
        return 0;
    }

    // 4. TIFFReadDirectory
    TIFFReadDirectory(tif_r);

    // 5. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 0);

    // 6. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 1);

    // 7. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 2);

    // 8. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif_r, 0);

    // 9. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif_r, (uint64_t)Size);

    // 10. TIFFIsBigEndian
    TIFFIsBigEndian(tif_r);

    // 11. TIFFIsBigTIFF
    TIFFIsBigTIFF(tif_r);

    // 12. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif_r);

    // 13. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif_r);

    // 14. TIFFNumberOfDirectories
    TIFFNumberOfDirectories(tif_r);

    // 15. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 0);

    // 16. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 1);

    // 17. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif_r);

    // 18. TIFFNumberOfDirectories
    TIFFNumberOfDirectories(tif_r);

    // 19. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 0);

    // 20. TIFFSetDirectory
    TIFFSetDirectory(tif_r, 1);

    // 21. TIFFClose (for tif_r)
    TIFFClose(tif_r);
    tif_r = NULL;

    // 22. TIFFClose (for tif_w, which is NULL, handled gracefully)
    TIFFClose(tif_w);

    return 0;
}