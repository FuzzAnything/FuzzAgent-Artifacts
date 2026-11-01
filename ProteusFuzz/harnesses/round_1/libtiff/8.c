// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

static int safe_tiff_set_field_uint16(TIFF *tif, uint32_t tag, uint16_t val) {
    if (tif == NULL) return 0;
    // TIFFSetField expects specific types for specific tags. 
    // Passing a raw uint16 as a pointer or incorrect type causes crashes.
    // We will only set tags that accept a single uint16 value directly or use a safe wrapper.
    // However, TIFFVSetField is variadic. To avoid passing garbage pointers, 
    // we restrict usage to tags known to accept uint16 or use a dummy valid value.
    // A safer approach for fuzzing without knowing the tag type is to limit tags or 
    // ensure the argument matches the expected type for common tags.
    // Here we try to set the field. If the tag expects a pointer (e.g. TIFFTAG_IMAGEWIDTH expects uint32_t),
    // passing a uint16 might be interpreted as a pointer if the tag logic is flawed, 
    // but libtiff usually checks types. The crash suggests a NULL pointer dereference inside libtiff
    // likely because we passed a value that was interpreted as a pointer to NULL.
    // We will restrict the tag values to known safe ones or ensure we pass valid data.
    // Since we don't know the tag type from the fuzzer input, we will only set tags 
    // that are known to accept a single integer value if possible, or just skip if tag is 0.
    if (tag == 0) return 0;
    
    // To prevent passing a small integer as a pointer (which causes SEGV at 0x1),
    // we only invoke TIFFSetField if we are sure of the type or use a generic safe tag.
    // But the task requires calling TIFFSetField. 
    // The crash happens at tif_dir.c:846 where it does memcpy(tv->value, va_arg(ap, void *), ...).
    // This implies the tag expects a pointer, but we passed a uint16 (value 1) which is treated as a pointer.
    // We must ensure we don't pass small integers as pointers.
    // Strategy: Only set tags that are known to take simple integers, or pass a valid pointer.
    // Since we can't easily map all tags, we will filter tags to a small set of known safe tags 
    // or ensure the argument is a valid pointer if the tag requires it.
    // However, the simplest fix for the crash is to avoid passing small integers as the first vararg 
    // if the tag expects a pointer.
    // Let's try to set only tags that are definitely uint16 or uint32 without pointer indirection.
    // Common tags: TIFFTAG_IMAGEWIDTH (uint32), TIFFTAG_IMAGELENGTH (uint32), TIFFTAG_BITSPERSAMPLE (uint16).
    // We will map the fuzz input to these specific tags to ensure safety.
    
    // Map fuzz tag to a safe tag
    uint32_t safe_tag = tag;
    if (safe_tag == 0) safe_tag = TIFFTAG_IMAGEWIDTH;
    
    // If the original tag was small (like 1), it might be interpreted as a pointer.
    // We will force the tag to be one of the known integer-only tags.
    if (tag < 256) {
        // Use a known safe tag
        safe_tag = TIFFTAG_IMAGEWIDTH;
    }
    
    // We pass the value. If the tag expects a pointer, this will still crash.
    // We must ensure the tag we pick matches the argument type.
    // TIFFTAG_IMAGEWIDTH expects uint32_t. val is uint16. This is usually fine (promotion).
    // The crash was likely because tag=1 (or similar) was used, and libtiff thought it needed a pointer.
    // Let's restrict tags to a whitelist of integer-only tags.
    
    // Whitelist of tags that take simple integers (no pointer)
    if (safe_tag != TIFFTAG_IMAGEWIDTH && safe_tag != TIFFTAG_IMAGELENGTH && 
        safe_tag != TIFFTAG_BITSPERSAMPLE && safe_tag != TIFFTAG_COMPRESSION &&
        safe_tag != TIFFTAG_PHOTOMETRIC) {
        safe_tag = TIFFTAG_IMAGEWIDTH;
    }

    return TIFFSetField(tif, safe_tag, val);
}

static uint32_t get_tag(const uint8_t *d, size_t s, int idx) {
    if (idx * 4 + 4 > s) return 0;
    uint32_t val;
    memcpy(&val, d + idx * 4, sizeof(val));
    return val;
}

static uint16_t get_short(const uint8_t *d, size_t s, int idx) {
    if (idx * 2 + 2 > s) return 1;
    uint16_t val;
    memcpy(&val, d + idx * 2, sizeof(val));
    return val;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    const char *filename = "./dummy_file";
    
    // Write input data to dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    TIFF *tif = TIFFOpen(filename, "w");
    if (tif == NULL) return 0;

    // 1. TIFFSetField (20 times)
    for (int i = 0; i < 20; i++) {
        uint32_t tag = get_tag(Data, Size, i);
        uint16_t val = get_short(Data, Size, i + 20);
        safe_tiff_set_field_uint16(tif, tag, val);
    }

    // 2. TIFFFindField
    uint32_t find_tag = get_tag(Data, Size, 20);
    TIFFFindField(tif, find_tag, TIFF_ANY);

    // 3. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 4. TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // 5. TIFFSetField (15 times)
    for (int i = 0; i < 15; i++) {
        uint32_t tag = get_tag(Data, Size, 21 + i);
        uint16_t val = get_short(Data, Size, 40 + i);
        safe_tiff_set_field_uint16(tif, tag, val);
    }

    // 6. TIFFSetDirectory
    tdir_t dir_idx = get_short(Data, Size, 55);
    TIFFSetDirectory(tif, dir_idx);

    // 7. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 8. TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // 9. TIFFSetField
    uint32_t exif_tag = get_tag(Data, Size, 56);
    uint16_t exif_val = get_short(Data, Size, 57);
    safe_tiff_set_field_uint16(tif, exif_tag, exif_val);

    // 10. TIFFSetDirectory
    tdir_t dir_idx2 = get_short(Data, Size, 58);
    TIFFSetDirectory(tif, dir_idx2);

    // 11. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 12. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 13. TIFFOpen (for reading)
    tif = TIFFOpen(filename, "r");
    if (tif == NULL) return 0;

    // 14. TIFFSetDirectory
    tdir_t dir_idx3 = get_short(Data, Size, 59);
    TIFFSetDirectory(tif, dir_idx3);

    // 15. TIFFFindField
    uint32_t find_tag2 = get_tag(Data, Size, 60);
    TIFFFindField(tif, find_tag2, TIFF_ANY);

    // 16. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 17. TIFFClose (handles NULL gracefully)
    TIFFClose(tif);

    return 0;
}