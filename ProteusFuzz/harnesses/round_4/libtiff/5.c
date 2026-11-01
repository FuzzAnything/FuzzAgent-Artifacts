// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty to avoid immediate open failure
            // BigTIFF magic: 0x49492a00 (little endian) or 0x4d4d002a (big endian)
            // Classic TIFF magic: 0x4949002a or 0x4d4d2a00
            // We write a minimal classic TIFF header (8 bytes) + 8 bytes padding
            uint8_t header[16] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            fwrite(header, 1, 16, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    TIFF *tif2 = NULL;
    int ret;
    tdir_t dir_count;
    uint64_t sub_dir_off;

    // Prepare environment: create a dummy file from fuzz input
    setup_dummy_file(Data, Size);

    // Open file for reading/writing to get a valid handle
    // Using "r+" to allow both read and write operations if possible, or "w" if creation needed
    // Fuzzing often benefits from trying to open as read-write to trigger different code paths
    tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // If open fails (e.g., file empty or invalid), try opening for writing to create a valid structure
        tif = TIFFOpen("./dummy_file", "w");
    }

    if (!tif) {
        // If still null, clean up and exit to avoid crashes
        remove("./dummy_file");
        return 0;
    }

    // Sequence start
    // 1. TIFFWriteDirectory
    ret = TIFFWriteDirectory(tif);

    // 2. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 0);

    // 3. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 1);

    // 4. TIFFReadDirectory
    ret = TIFFReadDirectory(tif);

    // 5. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 0);

    // 6. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 1);

    // 7. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 2);

    // 8. TIFFSetSubDirectory
    // Use input data to generate a subdirectory offset if available, else 0
    if (Size >= 8) {
        memcpy(&sub_dir_off, Data, 8);
    } else {
        sub_dir_off = 0;
    }
    ret = TIFFSetSubDirectory(tif, sub_dir_off);

    // 9. TIFFSetSubDirectory
    ret = TIFFSetSubDirectory(tif, 0);

    // 10. TIFFIsBigEndian
    ret = TIFFIsBigEndian(tif);

    // 11. TIFFIsBigTIFF
    ret = TIFFIsBigTIFF(tif);

    // 12. TIFFIsByteSwapped
    ret = TIFFIsByteSwapped(tif);

    // 13. TIFFIsByteSwapped
    ret = TIFFIsByteSwapped(tif);

    // 14. TIFFNumberOfDirectories
    dir_count = TIFFNumberOfDirectories(tif);

    // 15. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, (tdir_t)(Size % 10));

    // 16. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, (tdir_t)(Size % 5));

    // 17. TIFFIsByteSwapped
    ret = TIFFIsByteSwapped(tif);

    // 18. TIFFNumberOfDirectories
    dir_count = TIFFNumberOfDirectories(tif);

    // 19. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 0);

    // 20. TIFFSetDirectory
    ret = TIFFSetDirectory(tif, 1);

    // Cleanup: Close the first handle
    TIFFClose(tif);

    // Attempt to open a second handle to test the second TIFFClose in sequence
    // Re-open the file to ensure we have a valid handle for the second close
    tif2 = TIFFOpen("./dummy_file", "r+");
    if (!tif2) {
        tif2 = TIFFOpen("./dummy_file", "w");
    }
    
    if (tif2) {
        // 21. TIFFClose (first close in the final pair)
        TIFFClose(tif2);
        
        // Re-open again for the very last close to ensure valid pointer
        tif2 = TIFFOpen("./dummy_file", "r+");
        if (!tif2) {
            tif2 = TIFFOpen("./dummy_file", "w");
        }
        
        if (tif2) {
            // 22. TIFFClose (second close in the final pair)
            TIFFClose(tif2);
        }
    }

    // Final cleanup of file
    remove("./dummy_file");

    return 0;
}