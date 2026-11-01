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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            // Write minimal TIFF header to ensure file is not empty
            uint8_t minimal_header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(minimal_header, 1, sizeof(minimal_header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. TIFFFileName
    const char *filename = TIFFFileName(tif);
    (void)filename; // Use the value to avoid unused variable warning

    // 2. TIFFFieldWithTag (First call)
    uint32_t tag1 = (Size > 0) ? Data[0] : 0;
    const TIFFField *field1 = TIFFFieldWithTag(tif, tag1);
    (void)field1;

    // 3. TIFFFieldWithTag (Second call)
    uint32_t tag2 = (Size > 1) ? ((uint32_t)Data[1] << 8) | Data[0] : 0;
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag2);
    (void)field2;

    // 4. TIFFFieldWithTag (Third call)
    uint32_t tag3 = (Size > 2) ? ((uint32_t)Data[2] << 16) | ((uint32_t)Data[1] << 8) | Data[0] : 0;
    const TIFFField *field3 = TIFFFieldWithTag(tif, tag3);
    (void)field3;

    // 5. TIFFFieldWithTag (Fourth call)
    uint32_t tag4 = (Size > 3) ? ((uint32_t)Data[3] << 24) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[1] << 8) | Data[0] : 0;
    const TIFFField *field4 = TIFFFieldWithTag(tif, tag4);
    (void)field4;

    // 6. TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    // 7. TIFFFieldWithTag (Fifth call)
    uint32_t tag5 = (Size > 4) ? ((uint32_t)Data[4] << 24) | ((uint32_t)Data[3] << 16) | ((uint32_t)Data[2] << 8) | Data[1] : 0;
    const TIFFField *field5 = TIFFFieldWithTag(tif, tag5);
    (void)field5;

    // 8. _TIFFmalloc
    tmsize_t alloc_size = 0;
    if (Size > 5) {
        alloc_size = (tmsize_t)Data[5];
    }
    // Ensure we don't allocate 0 or extremely large sizes that might crash
    if (alloc_size == 0) alloc_size = 1;
    if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;

    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr) {
        // Write some data to the allocated buffer to ensure it's valid
        memset(ptr, 0x41, alloc_size);
    }

    // 9. _TIFFfree
    if (ptr) {
        _TIFFfree(ptr);
    }

    TIFFClose(tif);
    return 0;
}