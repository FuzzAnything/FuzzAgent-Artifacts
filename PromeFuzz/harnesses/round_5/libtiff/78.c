// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write fuzzer data to dummy file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }
    
    // Open the dummy TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract test parameters from fuzzer input
    uint32_t tag = *(uint32_t*)Data;
    TIFFDataType dtype = (TIFFDataType)(Data[4] % (TIFF_IFD8 + 1)); // Valid enum range
    
    // Test TIFFFindField
    const TIFFField *field1 = TIFFFindField(tif, tag, dtype);
    
    // Test TIFFFieldWithTag
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFFieldDataType if field exists
    if (field1) {
        TIFFDataType dt = TIFFFieldDataType(field1);
        (void)dt; // Use to avoid unused variable warning
    }
    
    // Test TIFFDataWidth
    int width = TIFFDataWidth(dtype);
    (void)width;
    
    // Test TIFFFieldTag if field exists
    if (field2) {
        uint32_t tag_val = TIFFFieldTag(field2);
        (void)tag_val;
    }
    
    // Test TIFFFieldWithName - use part of input as field name
    if (Size > 8) {
        // Create null-terminated string from input
        size_t name_len = Size - 8 > 255 ? 255 : Size - 8;
        char *field_name = (char*)malloc(name_len + 1);
        if (field_name) {
            memcpy(field_name, Data + 8, name_len);
            field_name[name_len] = '\0';
            const TIFFField *field3 = TIFFFieldWithName(tif, field_name);
            (void)field3;
            free(field_name);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}