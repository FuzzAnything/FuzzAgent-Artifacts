// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty
            // BigTIFF magic number (0x000000004949)
            uint8_t header[] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open for writing first to test write path
    TIFF *tif_write = TIFFOpen("./dummy_file", "w8");
    if (tif_write) {
        // 1. TIFFWriteDirectory
        TIFFWriteDirectory(tif_write);

        // 2. TIFFSetDirectory
        TIFFSetDirectory(tif_write, 0);

        // 3. TIFFSetDirectory
        TIFFSetDirectory(tif_write, 1);

        // Close write handle before reading to ensure clean state for read path
        TIFFClose(tif_write);
        tif_write = NULL;
    }

    // Open for reading to test read path
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // 4. TIFFReadDirectory
        TIFFReadDirectory(tif_read);

        // 5. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 0);

        // 6. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 1);

        // 7. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 2);

        // 8. TIFFSetSubDirectory
        TIFFSetSubDirectory(tif_read, 0);

        // 9. TIFFSetSubDirectory
        TIFFSetSubDirectory(tif_read, (uint64_t)Size);

        // 10. TIFFIsBigEndian
        TIFFIsBigEndian(tif_read);

        // 11. TIFFIsBigTIFF
        TIFFIsBigTIFF(tif_read);

        // 12. TIFFIsByteSwapped
        TIFFIsByteSwapped(tif_read);

        // 13. TIFFIsByteSwapped
        TIFFIsByteSwapped(tif_read);

        // 14. TIFFNumberOfDirectories
        TIFFNumberOfDirectories(tif_read);

        // 15. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 0);

        // 16. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 1);

        // 17. TIFFIsByteSwapped
        TIFFIsByteSwapped(tif_read);

        // 18. TIFFNumberOfDirectories
        TIFFNumberOfDirectories(tif_read);

        // 19. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 2);

        // 20. TIFFSetDirectory
        TIFFSetDirectory(tif_read, 3);

        // 21. TIFFClose (First close)
        TIFFClose(tif_read);
        tif_read = NULL;
    }

    // 22. TIFFClose (Second close - handles NULL gracefully)
    TIFFClose(tif_read);

    return 0;
}