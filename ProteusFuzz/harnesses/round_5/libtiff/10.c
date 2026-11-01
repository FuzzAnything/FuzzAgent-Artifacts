// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

static uint32_t get_u32(const uint8_t *Data, size_t Size, size_t *pos) {
    if (*pos + 4 > Size) return 0;
    uint32_t v = 0;
    memcpy(&v, Data + *pos, 4);
    *pos += 4;
    return v;
}

static uint16_t get_u16(const uint8_t *Data, size_t Size, size_t *pos) {
    if (*pos + 2 > Size) return 0;
    uint16_t v = 0;
    memcpy(&v, Data + *pos, 2);
    *pos += 2;
    return v;
}

static uint8_t get_u8(const uint8_t *Data, size_t Size, size_t *pos) {
    if (*pos >= Size) return 0;
    uint8_t v = Data[*pos];
    *pos++;
    return v;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    uint32_t tag = 0;
    uint32_t val32 = 0;
    uint16_t val16 = 0;
    uint8_t val8 = 0;
    float valf = 0.0f;
    double vald = 0.0;
    const char *str = "test";
    tdir_t dir_idx = 0;

    size_t pos = 0;

    // 1. TIFFSetField (20 times)
    for (int i = 0; i < 20; i++) {
        tag = get_u32(Data, Size, &pos);
        val32 = get_u32(Data, Size, &pos);
        // Try setting a common tag like ImageWidth or ImageLength with fuzzed values
        if (tag == 0) tag = TIFFTAG_IMAGEWIDTH;
        if (val32 == 0) val32 = 1;
        TIFFSetField(tif, tag, val32);
    }

    // 2. TIFFFindField
    tag = get_u32(Data, Size, &pos);
    const TIFFField *field = TIFFFindField(tif, tag, TIFF_ANY);
    (void)field;

    // 3. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 4. TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // 5. TIFFSetField (20 times for GPS)
    for (int i = 0; i < 20; i++) {
        tag = get_u32(Data, Size, &pos);
        val32 = get_u32(Data, Size, &pos);
        // Use TIFFTAG_GPSLATITUDE if tag is 0, but ensure the constant is defined
        // If not defined in this build, use a generic valid tag or skip
        if (tag == 0) {
            // Fallback to a known tag if GPS tags are not available in this build
            tag = TIFFTAG_IMAGEWIDTH; 
        }
        if (val32 == 0) val32 = 1;
        TIFFSetField(tif, tag, val32);
    }

    // 6. TIFFSetDirectory
    dir_idx = get_u16(Data, Size, &pos);
    TIFFSetDirectory(tif, dir_idx);

    // 7. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 8. TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 9. TIFFSetField (1 time for EXIF)
    tag = get_u32(Data, Size, &pos);
    val32 = get_u32(Data, Size, &pos);
    if (tag == 0) {
        // Fallback to a known tag if EXIF tags are not available
        tag = TIFFTAG_IMAGEWIDTH;
    }
    if (val32 == 0) val32 = 1;
    TIFFSetField(tif, tag, val32);

    // 10. TIFFSetDirectory
    dir_idx = get_u16(Data, Size, &pos);
    TIFFSetDirectory(tif, dir_idx);

    // 11. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 12. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 13. TIFFOpen (read mode)
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 14. TIFFSetDirectory
    dir_idx = get_u16(Data, Size, &pos);
    TIFFSetDirectory(tif, dir_idx);

    // 15. TIFFFindField
    tag = get_u32(Data, Size, &pos);
    field = TIFFFindField(tif, tag, TIFF_ANY);
    (void)field;

    // 16. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 17. TIFFClose (double close safety check)
    TIFFClose(tif);

    return 0;
}