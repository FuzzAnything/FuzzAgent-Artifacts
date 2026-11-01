// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header (Classic TIFF)
    // Byte order: Little Endian (II)
    uint16_t byte_order = 0x002A; // II
    uint16_t magic = 42;
    uint32_t first_ifd_offset = 8; // Offset to first IFD
    
    fwrite(&byte_order, sizeof(byte_order), 1, f);
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&first_ifd_offset, sizeof(first_ifd_offset), 1, f);
    
    // Write a minimal IFD (0 entries, next IFD offset 0)
    uint16_t num_entries = 0;
    uint32_t next_ifd = 0;
    fwrite(&num_entries, sizeof(num_entries), 1, f);
    fwrite(&next_ifd, sizeof(next_ifd), 1, f);
    
    // Pad with fuzz data if available to make file larger
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure we have a dummy file to work with
    write_dummy_tiff(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. Test TIFFSetDirectory
    // Try setting directory to 0 (usually safe)
    TIFFSetDirectory(tif, 0);
    
    // Try setting to a random directory index derived from input
    tdir_t dir_idx = (Size > 0) ? Data[0] : 0;
    TIFFSetDirectory(tif, dir_idx);

    // 2. Test TIFFReadCustomDirectory
    // We need a valid TIFFFieldArray. Since the structures are opaque in tiffio.h,
    // we cannot instantiate them directly. However, we can pass the standard
    // EXIF or GPS field arrays if they are accessible via global variables or
    // by using the library's internal helpers if exposed.
    // Since we cannot access internal globals easily without internal headers,
    // and we cannot define the structs, we will skip passing a custom array
    // for ReadCustomDirectory in this specific constrained environment, 
    // OR we rely on the fact that the fuzzer might trigger paths with NULL if handled,
    // but the prompt implies we should test the API.
    // A safer approach for opaque types in fuzzing without internal headers is 
    // to rely on the library's public API to get field arrays if possible, 
    // or simply skip the call if we can't construct the argument safely.
    // However, to maximize coverage, let's try to call it with a NULL pointer 
    // if the API allows it (often it returns an error), or skip it.
    // Given the error, we cannot construct the struct. 
    // Let's try to call TIFFReadCustomDirectory with NULL to see if it handles it gracefully,
    // or simply omit this specific call if it requires a valid struct we can't build.
    // Actually, many libtiff versions have global arrays like _TIFFFieldArrayTIFFFieldArray.
    // But without internal headers, we can't access them.
    // Let's skip the custom directory read/write that requires constructing the struct
    // and focus on the other APIs that don't require constructing opaque structs.
    
    // 3. Test TIFFFreeDirectory
    // This should be safe to call on an open file
    TIFFFreeDirectory(tif);

    // 4. Test Write-side functions
    // Re-open for writing to test CreateEXIF, CreateGPS, CreateCustomDirectory
    TIFFClose(tif);
    
    // Re-write dummy file to ensure clean state for writing
    write_dummy_tiff(Data, Size);
    
    tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    // Test TIFFCreateEXIFDirectory
    // This uses internal EXIF field array, so no need to construct one.
    TIFFCreateEXIFDirectory(tif);
    
    // Test TIFFCreateGPSDirectory
    // This uses internal GPS field array.
    TIFFCreateGPSDirectory(tif);
    
    // Test TIFFCreateCustomDirectory
    // We cannot construct a valid TIFFFieldArray due to opaque types.
    // We will skip this specific call to avoid compilation errors, 
    // as we cannot safely create the argument without internal headers.
    // If we pass NULL, it might crash or be undefined behavior as per prompt warning.
    // So we omit this specific test case in this constrained environment.
    // TIFFCreateCustomDirectory(tif, &dummy_array); // Cannot compile

    // Cleanup
    TIFFClose(tif);
    
    return 0;
}