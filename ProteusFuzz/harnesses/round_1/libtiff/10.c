// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
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
            // Write minimal valid TIFF header if input is empty to avoid immediate open failure
            uint16_t header[8] = {0x002A, 0x0000, 0x0000, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = NULL;
    tdir_t dir_count = 0;
    int ret = 0;
    uint64_t sub_dir_offset = 0;

    // 1. TIFFOpen
    tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    // 2. TIFFWriteDirectory
    ret = TIFFWriteDirectory(tif);

    // 3. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 4. TIFFOpen
    tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        return 0;
    }

    // 5. TIFFRewriteDirectory
    ret = TIFFRewriteDirectory(tif);

    // 6. TIFFCreateDirectory
    ret = TIFFCreateDirectory(tif);

    // 7. TIFFWriteDirectory
    ret = TIFFWriteDirectory(tif);

    // 8. TIFFNumberOfDirectories
    dir_count = TIFFNumberOfDirectories(tif);

    // 9. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 10. TIFFOpen
    tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        return 0;
    }

    // 11. TIFFSetSubDirectory
    // Use fuzzed data to determine offset, but ensure it's not 0 if we want to test sub-IFD logic specifically
    // However, 0 is valid to reset. Let's try a few variations based on input size.
    if (Size > 0) {
        sub_dir_offset = (uint64_t)Data[0] | ((uint64_t)Data[1] << 8);
        if (sub_dir_offset == 0 && Size > 2) {
            sub_dir_offset = (uint64_t)Data[2] | ((uint64_t)Data[3] << 8);
        }
    }
    ret = TIFFSetSubDirectory(tif, sub_dir_offset);

    // 12. TIFFWriteDirectory
    ret = TIFFWriteDirectory(tif);

    // 13. TIFFNumberOfDirectories
    dir_count = TIFFNumberOfDirectories(tif);

    // 14. TIFFUnlinkDirectory
    // Unlink the first directory if it exists (dir_count > 0)
    if (dir_count > 0) {
        ret = TIFFUnlinkDirectory(tif, 1);
    }

    // 15. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 16. TIFFClose (Redundant close on NULL is safe per docs, but let's ensure we don't double close valid handles)
    // The task requires invoking TIFFClose again. Since tif is NULL now, this is safe.
    TIFFClose(tif);

    return 0;
}