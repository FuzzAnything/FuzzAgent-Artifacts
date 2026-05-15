// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write input to dummy file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    // Open TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Extract parameters from fuzzer input
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    const char* name = NULL;
    
    if (Size >= sizeof(uint32_t)) {
        tag = *(const uint32_t*)Data;
    }
    
    if (Size >= sizeof(uint32_t) + 1) {
        dtype = (TIFFDataType)Data[sizeof(uint32_t)];
        // Ensure dtype is within valid range
        if (dtype > TIFF_IFD8) {
            dtype = TIFF_NOTYPE;
        }
    }
    
    if (Size >= sizeof(uint32_t) + 2) {
        name = (const char*)(Data + sizeof(uint32_t) + 1);
        // Ensure name is null-terminated within bounds
        size_t name_len = Size - (sizeof(uint32_t) + 1);
        if (name_len > 0 && name[name_len - 1] != '\0') {
            // Use a static buffer for safety
            static char safe_name[256];
            size_t copy_len = name_len < 255 ? name_len : 255;
            memcpy(safe_name, name, copy_len);
            safe_name[copy_len] = '\0';
            name = safe_name;
        }
    }

    // Test TIFFFieldWithTag
    const TIFFField* field1 = TIFFFieldWithTag(tif, tag);
    if (field1) {
        // Test TIFFFieldDataType
        TIFFDataType dt = TIFFFieldDataType(field1);
        (void)dt;
        
        // Test TIFFFieldTag
        uint32_t field_tag = TIFFFieldTag(field1);
        (void)field_tag;
    }

    // Test TIFFFindField
    const TIFFField* field2 = TIFFFindField(tif, tag, dtype);
    if (field2) {
        // Test TIFFFieldDataType on found field
        TIFFDataType dt2 = TIFFFieldDataType(field2);
        (void)dt2;
        
        // Test TIFFFieldTag on found field
        uint32_t field_tag2 = TIFFFieldTag(field2);
        (void)field_tag2;
    }

    // Test TIFFFieldWithName if we have a name
    if (name && name[0] != '\0') {
        const TIFFField* field3 = TIFFFieldWithName(tif, name);
        if (field3) {
            // Test TIFFFieldDataType on named field
            TIFFDataType dt3 = TIFFFieldDataType(field3);
            (void)dt3;
            
            // Test TIFFFieldTag on named field
            uint32_t field_tag3 = TIFFFieldTag(field3);
            (void)field_tag3;
        }
    }

    // Test TIFFDataWidth for all possible data types
    for (int i = TIFF_NOTYPE; i <= TIFF_IFD8; i++) {
        int width = TIFFDataWidth((TIFFDataType)i);
        (void)width;
    }

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}