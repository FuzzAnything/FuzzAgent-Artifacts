// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

// Helper function to safely call TIFFVGetField with variable arguments
// The function must have a variadic parameter to use va_start correctly
static int safe_tiff_vgetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return ret;
}

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0 && Data != NULL) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid-ish header if input is empty to avoid immediate crash on open
            uint16_t header[] = {0x002A, 0x0000, 0x0000, 0x0008};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Ensure we have a directory to work with, otherwise many functions will fail gracefully or do nothing
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz TIFFVGetField
    uint32_t tag = 0;
    if (Size >= 4) {
        tag = *(uint32_t *)Data;
    }
    
    // Use the helper function to handle va_list correctly
    // Pass a dummy argument to satisfy the variadic requirement of the helper
    int vget_result = safe_tiff_vgetfield(tif, tag, NULL);
    (void)vget_result;

    // Fuzz TIFFGetTagListEntry
    int tag_index = 0;
    if (Size >= 8) {
        tag_index = *(int *)(Data + 4);
    }
    // Clamp index to reasonable range
    if (tag_index < 0) tag_index = 0;
    if (tag_index > 1000) tag_index = 1000;
    
    uint32_t result_tag = TIFFGetTagListEntry(tif, tag_index);
    (void)result_tag;

    // Fuzz TIFFMergeFieldInfo
    // We cannot construct a valid TIFFFieldInfo array easily due to incomplete type in headers.
    // We skip this specific call to avoid compilation errors or undefined behavior.
    
    // Fuzz TIFFFindField
    uint32_t find_tag = 0;
    if (Size >= 12) {
        find_tag = *(uint32_t *)(Data + 8);
    }
    TIFFDataType find_type = TIFF_ANY;
    if (Size >= 16) {
        find_type = (TIFFDataType)(*(uint32_t *)(Data + 12));
    }
    const TIFFField *field = TIFFFindField(tif, find_tag, find_type);
    (void)field;

    // Fuzz TIFFCreateEXIFDirectory
    int exif_result = TIFFCreateEXIFDirectory(tif);
    (void)exif_result;

    // Fuzz TIFFReadCustomDirectory
    // We cannot construct a valid TIFFFieldArray due to incomplete type.
    // We skip this specific call.
    
    TIFFClose(tif);
    return 0;
}