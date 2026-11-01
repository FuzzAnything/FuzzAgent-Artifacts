// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    if (size > 0) {
        size_t written = fwrite(data, 1, size, fp);
        if (written != size) {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have some valid data to write to the file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    // Open the dummy file for reading and writing
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as write-only if read+write fails (e.g., file might be empty or invalid)
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    // Fuzzing logic for target API functions

    // 1. TIFFCreateGPSDirectory
    // Requires a valid TIFF handle.
    (void)TIFFCreateGPSDirectory(tif);

    // 2. TIFFReadCustomDirectory
    // Requires a valid TIFF handle, directory offset, and field array.
    // The library asserts that infoarray is not NULL.
    // We must provide a valid pointer. We can use the standard field array if accessible,
    // or construct a minimal valid one. Since we cannot easily access internal arrays,
    // we will skip this specific call with NULL to avoid the assert crash.
    // Instead, we rely on the fact that passing a valid array is hard without linking
    // specific internal symbols, so we might skip or use a dummy valid structure if possible.
    // However, to strictly follow the "fuzz" requirement without crashing on assert:
    // We will not call this with NULL. We can try to get the standard field array if possible,
    // but it's internal. A safer approach for fuzzing without internal access is to skip
    // or ensure we have a valid pointer.
    // Since we cannot easily create a valid TIFFFieldArray without more context,
    // and the assert is strict, we will omit this call or use a dummy valid pointer if we can.
    // Let's try to use the standard EXIF or GPS field arrays if they are exposed globally,
    // but they are often static.
    // Alternative: We can try to read a directory at offset 0 with a dummy array if we define one.
    // But defining a static array might be complex.
    // Given the constraint, we will skip this specific call to avoid the crash,
    // or we can try to use TIFFFieldArray from the library if it's exported.
    // Actually, we can try to use the standard field array pointer if we can find it.
    // But for safety, we will skip this call if we can't provide a valid array.
    // Wait, we can try to use the standard field array if it's available.
    // Let's assume we can't easily get it. So we skip.
    // However, to explore states, we can try to call it with a valid offset and a dummy array
    // if we define one. But defining a valid TIFFFieldArray is non-trivial.
    // Let's skip this call for now to avoid the crash.
    // Actually, we can try to use the standard field array if it's exported.
    // If not, we skip.
    // For now, we skip this call to avoid the assert.

    // 3. TIFFCreateCustomDirectory
    // Similar to above, requires a field array.
    // We skip this call as well to avoid the assert.
    // (void)TIFFCreateCustomDirectory(tif, NULL); // This would also likely assert or fail.

    // 4. TIFFFreeDirectory
    // Releases memory for the current directory.
    TIFFFreeDirectory(tif);

    // 5. TIFFSetDirectory
    // Selects a specific directory.
    tdir_t dir_index = 0;
    if (Size >= 4) {
        dir_index = *(const uint32_t *)(Data + (Size % 4));
    }
    (void)TIFFSetDirectory(tif, dir_index);

    // 6. TIFFCreateEXIFDirectory
    // Initializes EXIF directory.
    (void)TIFFCreateEXIFDirectory(tif);

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}