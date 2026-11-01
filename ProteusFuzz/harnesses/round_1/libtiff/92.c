// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

// Helper function to safely call TIFFVGetField with a specific argument
// Must be variadic to allow va_start
static int safe_tiffvgetfield_uint32(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return ret;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Ensure at least one directory is read to populate fields
    TIFFReadDirectory(tif);

    // Fuzz TIFFVGetField
    uint32_t test_tag = TIFFTAG_IMAGEWIDTH;
    if (Size >= 4) {
        test_tag = *(const uint32_t *)(Data);
        if (test_tag > 0xFFFF) test_tag = 0xFFFF;
    }

    uint32_t dummy_val = 0;
    // Use the helper to correctly handle va_list
    // Pass the dummy_val as the variadic argument
    safe_tiffvgetfield_uint32(tif, test_tag, &dummy_val);

    // Fuzz TIFFGetTagListEntry
    int tag_index = 0;
    if (Size >= 8) {
        tag_index = *(const int *)(Data + 4);
        if (tag_index < 0) tag_index = 0;
        if (tag_index > 1000) tag_index = 1000;
    }
    uint32_t tag_id = TIFFGetTagListEntry(tif, tag_index);
    (void)tag_id;

    // Fuzz TIFFMergeFieldInfo
    // Define a dummy custom field
    static const TIFFFieldInfo custom_fields[] = {
        {0x0000, 0, 0, TIFF_SHORT, 0, 0, 0, 0, "CustomTag", NULL},
        {0x0001, 1, 1, TIFF_LONG, 0, 0, 0, 0, "CustomTag2", NULL},
    };
    int merge_count = 2;
    if (Size >= 12) {
        merge_count = *(const uint32_t *)(Data + 8);
        if (merge_count > 10) merge_count = 10;
    }
    if (merge_count > 2) merge_count = 2;

    int merge_result = TIFFMergeFieldInfo(tif, custom_fields, merge_count);
    (void)merge_result;

    // Fuzz TIFFFindField
    uint32_t find_tag = 0;
    if (Size >= 16) {
        find_tag = *(const uint32_t *)(Data + 12);
        if (find_tag > 0xFFFF) find_tag = 0xFFFF;
    }
    TIFFDataType find_type = TIFF_ANY;
    if (Size >= 20) {
        find_type = (TIFFDataType)(*(const uint32_t *)(Data + 16) % 16);
    }
    const TIFFField *field = TIFFFindField(tif, find_tag, find_type);
    (void)field;

    // Fuzz TIFFCreateEXIFDirectory
    int exif_result = TIFFCreateEXIFDirectory(tif);
    (void)exif_result;

    // Fuzz TIFFReadCustomDirectory
    toff_t diroff = 0;
    if (Size >= 24) {
        diroff = *(const toff_t *)(Data + 20);
    }
    
    // We cannot initialize TIFFField directly as it is an incomplete type in tiffio.h.
    // However, we can try to pass NULL or a pointer to a valid array if we had access.
    // Since we can't construct a valid TIFFFieldArray without internal knowledge,
    // we will skip this specific call or pass a dummy pointer if the API allows.
    // The API expects const TIFFFieldArray *. We cannot safely construct one.
    // We will omit this call to avoid compilation errors, or rely on the fact that
    // passing NULL might trigger a check (though the signature is const pointer).
    // To be safe and compile, we will not call TIFFReadCustomDirectory with a constructed array.
    // Instead, we can try to read a custom directory if the library provides a default or
    // if we can find a way to get a valid array. Since we can't, we skip this specific fuzzing
    // for the custom array construction part, but we can still try to call it with a null pointer
    // if the function handles it, but the signature is const *.
    // Let's assume we cannot safely construct the array, so we skip this specific invocation
    // to ensure compilation.
    
    // Alternative: Try to use the internal field array if accessible, but it's not.
    // We will just skip this line to ensure the driver compiles.
    // int custom_read_result = TIFFReadCustomDirectory(tif, diroff, NULL); // Likely invalid
    
    TIFFClose(tif);

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}