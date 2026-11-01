// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Ensure the file has a valid TIFF magic number (little endian or big endian)
    uint8_t buffer[Size];
    memcpy(buffer, Data, Size);
    
    // Try to make it look like a valid TIFF header (Little Endian)
    buffer[0] = 'I';
    buffer[1] = 'I';
    buffer[2] = 42; // Magic number
    buffer[3] = 0;
    // Set IFD offset to 8 (minimum valid)
    buffer[4] = 8;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = 0;

    write_dummy_file(buffer, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. TIFFReadDirectory
    // Attempt to read the first directory
    (void)TIFFReadDirectory(tif);

    // 2. TIFFSetSubDirectory
    // Try setting subdirectory to various offsets derived from input
    if (Size >= 8) {
        uint64_t offset = 0;
        memcpy(&offset, Data, 8);
        (void)TIFFSetSubDirectory(tif, offset);
        
        // Also try with 0 to reset
        (void)TIFFSetSubDirectory(tif, 0);
    }

    // 3. TIFFSetDirectory
    // Try setting directory index
    if (Size >= 4) {
        uint32_t dir_idx = 0;
        memcpy(&dir_idx, Data, 4);
        // Limit index to a reasonable range to avoid immediate crashes on large values
        if (dir_idx < 1000) {
            (void)TIFFSetDirectory(tif, (tdir_t)dir_idx);
        }
    }
    // Try index 0
    (void)TIFFSetDirectory(tif, 0);

    // 4. TIFFReadEXIFDirectory
    // Try reading EXIF at various offsets
    if (Size >= 8) {
        uint64_t exif_offset = 0;
        memcpy(&exif_offset, Data, 8);
        (void)TIFFReadEXIFDirectory(tif, (toff_t)exif_offset);
    }
    (void)TIFFReadEXIFDirectory(tif, 0);

    // 5. TIFFLastDirectory
    (void)TIFFLastDirectory(tif);

    // 6. TIFFReadGPSDirectory
    // Try reading GPS at various offsets
    if (Size >= 8) {
        uint64_t gps_offset = 0;
        memcpy(&gps_offset, Data, 8);
        (void)TIFFReadGPSDirectory(tif, (toff_t)gps_offset);
    }
    (void)TIFFReadGPSDirectory(tif, 0);

    // Cleanup
    TIFFClose(tif);
    
    return 0;
}