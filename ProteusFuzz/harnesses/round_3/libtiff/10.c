// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
        }
        fclose(f);
    }
}

static void safe_set_field(TIFF *t, const uint8_t *d, size_t *idx, size_t max) {
    // Need at least 3 bytes: 2 for tag, 1 for value
    if (*idx + 3 > max) return;
    
    uint32_t tag = ((uint32_t)d[*idx] << 8) | (uint32_t)d[*idx + 1];
    (*idx) += 2;
    
    int val = d[*idx];
    (*idx) += 1;
    
    TIFFSetField(t, tag, val);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (tif == NULL) {
        return 0;
    }

    size_t idx = 0;

    // 1. TIFFSetField (x20)
    for (int i = 0; i < 20; i++) {
        safe_set_field(tif, Data, &idx, Size);
    }

    // 2. TIFFFindField
    if (idx + 2 <= Size) {
        uint32_t tag = ((uint32_t)Data[idx] << 8) | (uint32_t)Data[idx + 1];
        idx += 2;
        TIFFFindField(tif, tag, TIFF_ANY);
    }

    // 3. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 4. TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // 5. TIFFSetField (x15)
    for (int i = 0; i < 15; i++) {
        safe_set_field(tif, Data, &idx, Size);
    }

    // 6. TIFFSetDirectory
    if (idx < Size) {
        tdir_t dir_idx = Data[idx];
        idx++;
        TIFFSetDirectory(tif, dir_idx);
    }

    // 7. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 8. TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 9. TIFFSetField
    safe_set_field(tif, Data, &idx, Size);

    // 10. TIFFSetDirectory
    if (idx < Size) {
        tdir_t dir_idx = Data[idx];
        idx++;
        TIFFSetDirectory(tif, dir_idx);
    }

    // 11. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 12. TIFFClose
    TIFFClose(tif);

    // 13. TIFFOpen (Read mode)
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2 == NULL) {
        return 0;
    }

    // 14. TIFFSetDirectory
    if (idx < Size) {
        tdir_t dir_idx = Data[idx];
        idx++;
        TIFFSetDirectory(tif2, dir_idx);
    }

    // 15. TIFFFindField
    if (idx + 2 <= Size) {
        uint32_t tag = ((uint32_t)Data[idx] << 8) | (uint32_t)Data[idx + 1];
        idx += 2;
        TIFFFindField(tif2, tag, TIFF_ANY);
    }

    // 16. TIFFClose (first close)
    TIFFClose(tif2);

    // 17. TIFFClose (second close - redundant but per task list)
    TIFFClose(tif2);

    return 0;
}