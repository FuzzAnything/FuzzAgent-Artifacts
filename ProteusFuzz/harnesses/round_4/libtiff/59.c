// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0 && Data != NULL) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty/null to avoid immediate crash on open
            uint16_t little_endian = 0x002A;
            uint16_t version = 42;
            uint32_t ifd_offset = 8;
            fwrite(&little_endian, 2, 1, f);
            fwrite(&version, 2, 1, f);
            fwrite(&ifd_offset, 4, 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a dummy file with fuzzed data
    write_dummy_file(Data, Size);

    // Open the file for reading
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read == NULL) {
        // Try opening for writing if read fails (file might be too corrupted for read)
        TIFF *tif_write = TIFFOpen("./dummy_file", "w");
        if (tif_write != NULL) {
            // Test write-side functions
            uint64_t diroff = 0;
            TIFFWriteCustomDirectory(tif_write, &diroff);
            
            // Set a directory index (0 is safe for a new file)
            TIFFSetDirectory(tif_write, 0);
            
            // Set subdirectory offset (0 is safe)
            TIFFSetSubDirectory(tif_write, 0);
            
            // Check if last directory
            TIFFLastDirectory(tif_write);
            
            TIFFClose(tif_write);
        }
        return 0;
    }

    // Test Read Side Functions
    toff_t diroff = 0;
    uint64_t pdir_off = 0;
    tdir_t dir_index = 0;

    // 1. TIFFSetDirectory: Select a specific directory
    // We try index 0, and also a potentially invalid one derived from input to test error handling
    TIFFSetDirectory(tif_read, 0);
    if (Size > 0) {
        TIFFSetDirectory(tif_read, (tdir_t)Data[0]);
    }

    // 2. TIFFLastDirectory: Check if current is last
    TIFFLastDirectory(tif_read);

    // 3. TIFFReadEXIFDirectory: Read EXIF tags
    // We use 0 as offset, or a value from input if size permits
    toff_t exif_offset = 0;
    if (Size >= 8) {
        exif_offset = *(const toff_t*)(Data);
    }
    TIFFReadEXIFDirectory(tif_read, exif_offset);

    // 4. TIFFReadCustomDirectory: Read custom directory
    // We need a field array. We can use the global TIFFField array if accessible, 
    // but usually we pass NULL or a valid custom array. 
    // Since we don't have a custom array defined in the fuzzer context easily,
    // we pass NULL or rely on default behavior if the function allows, 
    // but the signature requires const TIFFFieldArray *.
    // To be safe and avoid segfaults with NULL if not handled, we skip or use a static dummy if possible.
    // However, libtiff often handles NULL gracefully or expects specific arrays.
    // Given the constraints, we will not pass a constructed array to avoid complex setup,
    // but we can try passing NULL if the API allows, or skip. 
    // Looking at libtiff source, it often expects a valid pointer. 
    // We will skip this specific call with a custom array unless we can safely construct one.
    // Instead, we rely on TIFFReadEXIFDirectory which internally uses a static array.
    
    // 5. TIFFSetSubDirectory: Set sub-IFD offset
    uint64_t sub_offset = 0;
    if (Size >= 8) {
        sub_offset = *(const uint64_t*)(Data);
    }
    TIFFSetSubDirectory(tif_read, sub_offset);

    // 6. TIFFWriteCustomDirectory: Not typically called on read-only handle, but we can try if writable
    // We opened as "r", so this might fail, which is a valid test case.
    TIFFWriteCustomDirectory(tif_read, &pdir_off);

    // Cleanup
    TIFFClose(tif_read);

    return 0;
}