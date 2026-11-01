// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write a minimal valid-ish TIFF header to avoid immediate open failure
            // BigTIFF magic: 0x49492A00 (Little Endian)
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: create a dummy file with input data
    write_dummy_file(Data, Size);

    // Open file for reading and writing to allow directory operations
    TIFF *tif = TIFFOpen("./dummy_file", "rw+");
    if (!tif) {
        return 0;
    }

    // 1. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 2. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 3. TIFFSetDirectory
    TIFFSetDirectory(tif, 1);

    // 4. TIFFReadDirectory
    TIFFReadDirectory(tif);

    // 5. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 6. TIFFSetDirectory
    TIFFSetDirectory(tif, 1);

    // 7. TIFFSetDirectory
    TIFFSetDirectory(tif, 2);

    // 8. TIFFSetSubDirectory
    // Use a derived offset from input to explore states, or 0 to reset
    uint64_t sub_offset = 0;
    if (Size >= 8) {
        sub_offset = *(const uint64_t *)(Data + Size - 8);
    }
    TIFFSetSubDirectory(tif, sub_offset);

    // 9. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, 0);

    // 10. TIFFIsBigEndian
    TIFFIsBigEndian(tif);

    // 11. TIFFIsBigTIFF
    TIFFIsBigTIFF(tif);

    // 12. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif);

    // 13. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif);

    // 14. TIFFNumberOfDirectories
    TIFFNumberOfDirectories(tif);

    // 15. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 16. TIFFSetDirectory
    TIFFSetDirectory(tif, 1);

    // 17. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif);

    // 18. TIFFNumberOfDirectories
    TIFFNumberOfDirectories(tif);

    // 19. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 20. TIFFSetDirectory
    TIFFSetDirectory(tif, 1);

    // Cleanup
    // 21. TIFFClose
    TIFFClose(tif);

    // 22. TIFFClose (Handle NULL gracefully as per spec)
    TIFFClose(tif);

    return 0;
}