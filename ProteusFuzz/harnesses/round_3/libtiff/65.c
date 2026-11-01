// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
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
            // Write a minimal valid TIFF header if input is empty
            // Little-endian, offset to first IFD at 8
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open the file in read mode
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Fuzzing logic for the target API functions

    // 1. TIFFSetSubDirectory
    // Try setting subdirectory with various offsets derived from input
    if (Size >= 8) {
        uint64_t offset = 0;
        memcpy(&offset, Data, 8);
        // Ensure offset is not too large to avoid potential issues, though libtiff handles it
        TIFFSetSubDirectory(tif, offset);
    }
    // Try resetting
    TIFFSetSubDirectory(tif, 0);

    // 2. TIFFLastDirectory
    // Check if current directory is last
    (void)TIFFLastDirectory(tif);

    // 3. TIFFReadCustomDirectory
    // Try reading custom directory at various offsets
    if (Size >= 4) {
        toff_t diroff = 0;
        memcpy(&diroff, Data, 4);
        // Use NULL for infoarray to test default behavior or error handling
        TIFFReadCustomDirectory(tif, diroff, NULL);
    }
    // Try with offset 0
    TIFFReadCustomDirectory(tif, 0, NULL);

    // 4. TIFFSetDirectory
    // Try setting directory with various indices
    if (Size >= 4) {
        tdir_t dir_idx = 0;
        memcpy(&dir_idx, Data, 4);
        TIFFSetDirectory(tif, dir_idx);
    }
    // Try setting to 0
    TIFFSetDirectory(tif, 0);

    // 5. TIFFWriteCustomDirectory
    // This function requires write mode. Re-open for write to test.
    TIFFClose(tif);
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        uint64_t diroff = 0;
        // Try writing custom directory
        TIFFWriteCustomDirectory(tif, &diroff);
        TIFFClose(tif);
    }

    // Re-open for read to continue testing read functions if needed
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 6. TIFFReadEXIFDirectory
    // Try reading EXIF directory at various offsets
    if (Size >= 4) {
        toff_t exif_diroff = 0;
        memcpy(&exif_diroff, Data, 4);
        TIFFReadEXIFDirectory(tif, exif_diroff);
    }
    // Try with offset 0
    TIFFReadEXIFDirectory(tif, 0);

    // Cleanup
    TIFFClose(tif);

    return 0;
}