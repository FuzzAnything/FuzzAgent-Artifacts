// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // Byte order: II (Little Endian)
            // Magic number: 42
            // First IFD offset: 8
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open the file in read-write mode to allow directory creation
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as write mode if read-write fails (e.g., file doesn't exist or permissions)
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    // 1. TIFFCreateDirectory
    // Initializes a new directory. Always returns 0.
    TIFFCreateDirectory(tif);

    // 2. TIFFCreateEXIFDirectory
    // Initializes for EXIF metadata. Returns 1 on success, 0 on failure.
    int exif_result = TIFFCreateEXIFDirectory(tif);
    (void)exif_result;

    // 3. TIFFGetTagListCount
    // Returns count of custom tags.
    int tag_count = TIFFGetTagListCount(tif);
    (void)tag_count;

    // 4. TIFFSetDirectory
    // Selects a specific directory. We try index 0.
    tdir_t dir_index = 0;
    int set_dir_result = TIFFSetDirectory(tif, dir_index);
    (void)set_dir_result;

    // 5. TIFFCreateCustomDirectory
    // The library crashes if passed NULL because it dereferences the pointer immediately.
    // To fix the crash in the fuzz driver, we must not pass NULL.
    // We skip this call or pass a valid structure. Since we cannot easily construct a valid
    // TIFFFieldArray on the fly without complex setup, we skip this specific API call
    // to prevent the crash while still fuzzing other parts of the library.
    // Note: In a real scenario, the library should be patched to handle NULL, but here
    // we fix the driver to avoid triggering the bug.
    // TIFFCreateCustomDirectory(tif, NULL); // Removed to prevent crash

    // 6. TIFFFreeDirectory
    // Frees memory for the current directory.
    TIFFFreeDirectory(tif);

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}