// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
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

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header (Little Endian)
    // Magic number 0x002A (Little Endian)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD (we will put it at offset 8 + size of fuzz data if size > 0, else 0)
    // For simplicity in fuzzing, we often write a dummy offset or 0 to trigger parsing logic
    uint32_t ifd_offset = 0;
    if (size > 0) {
        // If we have data, pretend the IFD starts after the header
        // But for robust fuzzing, sometimes 0 is better to trigger error paths
        ifd_offset = 8; 
    }
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // Write the fuzz data as the IFD content (even if malformed, libtiff should handle it)
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    
    // Prepare environment: Create a dummy file with the fuzz data
    write_dummy_tiff(Data, Size);
    
    // Open the file for reading
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. TIFFCurrentDirectory
    tdir_t dir1 = TIFFCurrentDirectory(tif);
    (void)dir1; // Suppress unused variable warning

    // 2. TIFFSetDirectory
    // Try setting to directory 0 (common case)
    int ret1 = TIFFSetDirectory(tif, 0);
    (void)ret1;

    // 3. TIFFSetSubDirectory
    // Try setting a subdirectory offset. Use 0 to reset or a value from input if available.
    uint64_t sub_offset = 0;
    if (Size >= 8) {
        sub_offset = *(const uint64_t *)(Data);
    }
    int ret2 = TIFFSetSubDirectory(tif, sub_offset);
    (void)ret2;

    // 4. TIFFReadDirectory
    int ret3 = TIFFReadDirectory(tif);
    (void)ret3;

    // 5. TIFFSetDirectory (again)
    // Try setting to directory 1 to explore state changes
    int ret4 = TIFFSetDirectory(tif, 1);
    (void)ret4;

    // 6. TIFFReadDirectory (again)
    int ret5 = TIFFReadDirectory(tif);
    (void)ret5;

    // 7. TIFFCurrentDirectory (again)
    tdir_t dir2 = TIFFCurrentDirectory(tif);
    (void)dir2;

    // 8. TIFFSetSubDirectory (again)
    // Try a different offset or 0
    uint64_t sub_offset2 = 0;
    if (Size >= 16) {
        sub_offset2 = *(const uint64_t *)(Data + 8);
    }
    int ret6 = TIFFSetSubDirectory(tif, sub_offset2);
    (void)ret6;

    // 9. TIFFNumberOfDirectories
    tdir_t count = TIFFNumberOfDirectories(tif);
    (void)count;

    // Cleanup
    TIFFClose(tif);
    
    return 0;
}