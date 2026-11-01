// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            // Write a minimal valid TIFF header if no data provided
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

// Helper function to handle va_list operations to avoid fixed-args error
// We add a dummy variadic parameter to allow va_start to work correctly
static int fuzz_vgetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return result;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open in read mode first for read operations
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. TIFFMergeFieldInfo
    static const char *custom_name1 = "CustomTag";
    static const char *custom_name2 = "CustomTag2";
    
    // Define custom fields
    // Structure: {tag, readcount, writecount, type, anonymous, bit, oktochange, passcount, name}
    static const TIFFFieldInfo custom_fields[] = {
        {32768, 1, 1, TIFF_SHORT, 0, 0, 0, 0, (char*)custom_name1},
        {32769, 1, 1, TIFF_LONG, 0, 0, 0, 0, (char*)custom_name2}
    };
    uint32_t count = 2;
    
    TIFFMergeFieldInfo(tif, custom_fields, count);

    // 2. TIFFReadCustomDirectory
    toff_t diroff = 0;
    if (Size >= 8) {
        diroff = *(uint64_t *)(Data);
        diroff %= (Size > 1000 ? 1000 : Size); 
    }
    
    // Pass NULL for infoarray to test error handling as we cannot construct TIFFFieldArray easily
    TIFFReadCustomDirectory(tif, diroff, NULL);

    // 3. TIFFGetTagListEntry
    int tag_index = 0;
    if (Size > 0) {
        tag_index = Data[0] % 10;
    }
    uint32_t tag_id = TIFFGetTagListEntry(tif, tag_index);
    (void)tag_id;

    // Close and reopen in write mode for write operations
    TIFFClose(tif);
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        // 4. TIFFCreateEXIFDirectory
        int ret_exif = TIFFCreateEXIFDirectory(tif);
        (void)ret_exif;
        
        // 5. TIFFVGetField
        uint32_t test_tag = 0;
        if (Size > 1) {
            test_tag = Data[1];
        }
        
        // Call the helper function which correctly handles va_start
        int result = fuzz_vgetfield(tif, test_tag);
        (void)result;

        // 6. TIFFFindField
        uint32_t search_tag = 0;
        if (Size > 2) {
            search_tag = Data[2];
        }
        const TIFFField *field = TIFFFindField(tif, search_tag, TIFF_ANY);
        (void)field;

        TIFFClose(tif);
    }

    return 0;
}