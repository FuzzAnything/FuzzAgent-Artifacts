// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_tiff(TIFF *tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

// Helper function to test va_list usage safely
// Must be a variadic function to use va_start correctly
static int test_va_list(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return ret;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    }

    // Define custom field info for TIFFMergeFieldInfo
    static const TIFFFieldInfo custom_fields[] = {
        { 32768, 1, 1, TIFF_SHORT, 0, 1, 1, "CustomTag", 0 },
        { 32769, 1, 1, TIFF_LONG, 0, 1, 1, "CustomTag2", 0 }
    };
    uint32_t n_custom = sizeof(custom_fields) / sizeof(custom_fields[0]);

    // 1. TIFFMergeFieldInfo
    TIFFMergeFieldInfo(tif, custom_fields, n_custom);

    // 2. TIFFGetTagListEntry - try various indices
    for (int i = -1; i < (int)n_custom + 2; i++) {
        TIFFGetTagListEntry(tif, i);
    }

    // 3. TIFFFindField - try various tags and types
    uint32_t tags_to_try[] = { 0, 256, 257, 32768, 32769, 0xFFFF, 0xFFFFFFFF };
    TIFFDataType types_to_try[] = { TIFF_NOTYPE, TIFF_BYTE, TIFF_SHORT, TIFF_LONG, TIFF_ANY };
    
    for (size_t i = 0; i < sizeof(tags_to_try)/sizeof(tags_to_try[0]); i++) {
        for (size_t j = 0; j < sizeof(types_to_try)/sizeof(types_to_try[0]); j++) {
            const TIFFField *field = TIFFFindField(tif, tags_to_try[i], types_to_try[j]);
            (void)field;
        }
    }

    // 4. TIFFVGetField - use helper function to avoid va_start error
    // Try with a known tag (ImageWidth)
    test_va_list(tif, 256);
    // Try with an invalid tag
    test_va_list(tif, 0xFFFF);

    // 5. TIFFReadCustomDirectory
    // Skipping direct construction of TIFFFieldArray as it is an opaque/complex internal type
    // and cannot be safely initialized without internal headers.
    
    // 6. TIFFCreateEXIFDirectory
    if (tif) {
        TIFFCreateEXIFDirectory(tif);
    }

    cleanup_tiff(tif);
    return 0;
}