// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        size_t written = fwrite(data, 1, size, f);
        if (written < size) {
            // Pad with zeros if input is too small to form a valid header
            size_t remaining = size - written;
            uint8_t pad[1024];
            memset(pad, 0, sizeof(pad));
            while (remaining > 0) {
                size_t to_write = (remaining > sizeof(pad)) ? sizeof(pad) : remaining;
                fwrite(pad, 1, to_write, f);
                remaining -= to_write;
            }
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data to write a minimal file
    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "rb");
    if (!tif) {
        return 0;
    }

    // Extract fuzzing parameters from input data if available
    uint64_t sub_dir_offset = 0;
    toff_t custom_dir_offset = 0;
    tdir_t dir_index = 0;
    toff_t exif_gps_offset = 0;

    if (Size >= sizeof(uint64_t)) {
        sub_dir_offset = *(const uint64_t *)(Data);
    }
    if (Size >= sizeof(uint64_t) * 2) {
        custom_dir_offset = *(const toff_t *)(Data + sizeof(uint64_t));
    }
    if (Size >= sizeof(uint64_t) * 3) {
        dir_index = *(const tdir_t *)(Data + sizeof(uint64_t) * 2);
    }
    if (Size >= sizeof(uint64_t) * 4) {
        exif_gps_offset = *(const toff_t *)(Data + sizeof(uint64_t) * 3);
    }

    // 1. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, sub_dir_offset);

    // 2. TIFFReadDirectory (standard IFD reading)
    TIFFReadDirectory(tif);

    // 3. TIFFSetDirectory (select specific directory)
    TIFFSetDirectory(tif, dir_index);

    // 4. TIFFReadEXIFDirectory
    TIFFReadEXIFDirectory(tif, exif_gps_offset);

    // 5. TIFFReadGPSDirectory
    TIFFReadGPSDirectory(tif, exif_gps_offset);

    // 6. TIFFReadCustomDirectory
    // We use the standard field array as a fallback or NULL if not accessible directly.
    // Note: _TIFFGetFields() is internal, so we rely on the fact that passing NULL
    // or a valid pointer is what we test. Here we pass NULL to test error handling
    // or use a static dummy if we could access internal globals. 
    // Since we cannot safely access internal globals without risking ABI breakage,
    // we pass NULL to trigger the error path or use the standard behavior if the 
    // implementation handles NULL gracefully (often it doesn't, which is good for fuzzing).
    // However, the signature requires a const TIFFFieldArray*. 
    // We will attempt to call it with NULL to test robustness, or skip if strictly required.
    // To be safe and trigger code paths, we pass NULL.
    TIFFReadCustomDirectory(tif, custom_dir_offset, NULL);

    // Cleanup
    TIFFClose(tif);

    return 0;
}