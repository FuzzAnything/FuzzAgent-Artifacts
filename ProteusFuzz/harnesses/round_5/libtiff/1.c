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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    const char *filename = TIFFFileName(tif);
    (void)filename;

    uint32_t tag1 = Data[0];
    const TIFFField *field1 = TIFFFieldWithTag(tif, tag1);
    (void)field1;

    uint32_t tag2 = (Size > 1) ? Data[1] : 0;
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag2);
    (void)field2;

    uint32_t tag3 = (Size > 2) ? Data[2] : 0;
    const TIFFField *field3 = TIFFFieldWithTag(tif, tag3);
    (void)field3;

    uint32_t tag4 = (Size > 3) ? Data[3] : 0;
    const TIFFField *field4 = TIFFFieldWithTag(tif, tag4);
    (void)field4;

    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    uint32_t tag5 = (Size > 4) ? Data[4] : 0;
    const TIFFField *field5 = TIFFFieldWithTag(tif, tag5);
    (void)field5;

    tmsize_t alloc_size = 0;
    if (Size > 5) {
        alloc_size = (tmsize_t)Data[5];
    }
    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr) {
        _TIFFfree(ptr);
    }

    TIFFClose(tif);
    return 0;
}