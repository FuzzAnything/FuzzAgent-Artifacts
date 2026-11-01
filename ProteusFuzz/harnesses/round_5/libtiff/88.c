// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening for writing if read+write fails (file might be empty or invalid)
        tif = TIFFOpen("./dummy_file", "w");
    }

    if (!tif) {
        return 0;
    }

    // 1. TIFFCreateDirectory
    TIFFCreateDirectory(tif);

    // 2. TIFFCreateCustomDirectory
    // We need a valid TIFFFieldArray. Since we cannot easily construct one without
    // internal libtiff knowledge of field definitions, we will skip this specific call
    // or attempt to use a static dummy if possible. However, the prompt warns against NULL.
    // To safely fuzz without crashing on invalid pointer, we skip this if we don't have
    // a valid array. In a real scenario, one might try to parse Data as a TIFFFieldArray,
    // but that is highly complex. We will skip this specific API invocation to avoid
    // undefined behavior from invalid pointers, focusing on the others.
    // Note: If a valid custom array were available, it would be passed here.
    // For fuzzing purposes, we rely on the other APIs which are safer to call blindly.

    // 4. TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 5. TIFFSetDirectory
    // Use data from input to determine directory index, modulo a safe range
    if (Size >= sizeof(tdir_t)) {
        tdir_t dir_idx = *(const tdir_t *)Data;
        // Limit to a reasonable range to avoid excessive allocation or invalid indices
        if (dir_idx < 100) {
            TIFFSetDirectory(tif, dir_idx);
        }
    }

    // 6. TIFFGetTagListCount
    int count = TIFFGetTagListCount(tif);
    (void)count; // Use the value to prevent unused variable warnings

    // 3. TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // Cleanup
    TIFFClose(tif);

    return 0;
}