// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal dummy TIFF header if input is empty to ensure file exists
            uint16_t dummy_header[] = {42, 0, 0, 8}; // Little-endian TIFF magic and offset
            fwrite(dummy_header, sizeof(dummy_header), 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. TIFFFileName
    const char *filename = TIFFFileName(tif);
    (void)filename; // Use the result to avoid unused variable warnings

    // 2. TIFFFieldWithTag (First call)
    const TIFFField *field1 = TIFFFieldWithTag(tif, 256); // ImageWidth

    // 3. TIFFFieldWithTag (Second call)
    const TIFFField *field2 = TIFFFieldWithTag(tif, 257); // ImageLength

    // 4. TIFFFieldWithTag (Third call)
    const TIFFField *field3 = TIFFFieldWithTag(tif, 258); // BitsPerSample

    // 5. TIFFFieldWithTag (Fourth call)
    const TIFFField *field4 = TIFFFieldWithTag(tif, 259); // Compression

    // 6. TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    // 7. TIFFFieldWithTag (Fifth call)
    const TIFFField *field5 = TIFFFieldWithTag(tif, 284); // PhotometricInterpretation
    (void)field1; (void)field2; (void)field3; (void)field4; (void)field5;

    // 8. _TIFFmalloc
    // Use a safe size derived from input to avoid huge allocations, capped at 1MB
    size_t alloc_size = 0;
    if (Size > 0) {
        alloc_size = (Size % 1024) + 1;
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    } else {
        alloc_size = 64;
    }
    void *ptr = _TIFFmalloc((tmsize_t)alloc_size);
    if (ptr) {
        // Optional: Write fuzz data to the allocated buffer to exercise memory
        memset(ptr, 0xAB, alloc_size);
    }

    // 9. _TIFFfree
    if (ptr) {
        _TIFFfree(ptr);
    }

    TIFFClose(tif);
    return 0;
}