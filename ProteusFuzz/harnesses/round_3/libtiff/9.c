// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0 && data != NULL) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 11) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (tif == NULL) {
        return 0;
    }

    uint32_t tag1 = Data[0] % 65536;
    uint32_t tag2 = Data[1] % 65536;
    uint32_t tag3 = Data[2] % 65536;
    uint32_t tag4 = Data[3] % 65536;
    uint32_t tag5 = Data[4] % 65536;
    uint32_t tag6 = Data[5] % 65536;
    uint32_t tag7 = Data[6] % 65536;

    TIFFSetField(tif, tag1, 1);
    TIFFSetField(tif, tag2, 2);
    TIFFSetField(tif, tag3, 3);
    TIFFSetField(tif, tag4, 4);
    TIFFSetField(tif, tag5, 5);
    TIFFSetField(tif, tag6, 6);
    TIFFSetField(tif, tag7, 7);

    TIFFWriteDirectory(tif);

    TIFFCreateEXIFDirectory(tif);

    uint32_t tag8 = Data[7] % 65536;
    TIFFSetField(tif, tag8, 8);

    TIFFFreeDirectory(tif);

    // Avoid passing NULL to TIFFCreateCustomDirectory to prevent SEGV
    // Instead, skip this call or ensure a valid array is passed.
    // Since creating a valid custom array is complex and not required for basic fuzzing,
    // we skip the call to avoid the crash.
    // TIFFCreateCustomDirectory(tif, custom_array); 

    uint32_t tag9 = Data[8] % 65536;
    uint32_t tag10 = Data[9] % 65536;
    TIFFSetField(tif, tag9, 9);
    TIFFSetField(tif, tag10, 10);

    tdir_t dir_index = Data[10] % 100;
    TIFFSetDirectory(tif, dir_index);

    TIFFClose(tif);

    tif = TIFFOpen("./dummy_file", "r");
    if (tif != NULL) {
        TIFFClose(tif);
    }

    TIFFClose(tif);

    return 0;
}