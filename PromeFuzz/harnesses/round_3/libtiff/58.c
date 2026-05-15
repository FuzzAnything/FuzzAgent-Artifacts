// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Test TIFFDataWidth with various data types
    // Use first byte to select data type
    if (Size > 0) {
        uint8_t type_selector = Data[0] % 17; // There are 17 TIFFDataType values
        TIFFDataType dt = (TIFFDataType)type_selector;
        int width = TIFFDataWidth(dt);
        (void)width; // Use result to avoid unused variable warning
    }
    
    // Create a dummy TIFF file for testing functions that require TIFF*
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    
    // Write minimal TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00, // Little-endian, magic number
        0x08, 0x00, 0x00, 0x00, // IFD offset
        0x00, 0x00              // End of IFD
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write some of the fuzzer data as TIFF directory entries
    if (Size > 1) {
        size_t to_write = Size - 1;
        if (to_write > 1024) to_write = 1024; // Limit size
        fwrite(Data + 1, 1, to_write, fp);
    }
    
    fclose(fp);
    
    // Open the dummy TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Test TIFFFindField with various tags and types
        if (Size > 5) {
            uint32_t tag;
            memcpy(&tag, Data + 1, sizeof(uint32_t));
            uint8_t type_selector = Data[5] % 17;
            TIFFDataType dt = (TIFFDataType)type_selector;
            const TIFFField* field = TIFFFindField(tif, tag, dt);
            (void)field;
        }
        
        // Test TIFFFieldWithName with a string from fuzzer input
        if (Size > 10) {
            // Create null-terminated string from fuzzer data
            size_t name_len = Size - 10;
            if (name_len > 100) name_len = 100;
            char* field_name = (char*)malloc(name_len + 1);
            if (field_name) {
                memcpy(field_name, Data + 10, name_len);
                field_name[name_len] = '\0';
                const TIFFField* field = TIFFFieldWithName(tif, field_name);
                (void)field;
                free(field_name);
            }
        }
        
        // Test TIFFFieldWithTag with various tags
        if (Size > 14) {
            uint32_t tag;
            memcpy(&tag, Data + 14, sizeof(uint32_t));
            const TIFFField* field = TIFFFieldWithTag(tif, tag);
            if (field) {
                // Test TIFFFieldDataType and TIFFFieldTag on the found field
                TIFFDataType field_dt = TIFFFieldDataType(field);
                uint32_t field_tag = TIFFFieldTag(field);
                (void)field_dt;
                (void)field_tag;
            }
        }
        
        // Test with known TIFF tags
        const TIFFField* field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
        if (field) {
            TIFFDataType dt = TIFFFieldDataType(field);
            uint32_t tag = TIFFFieldTag(field);
            (void)dt;
            (void)tag;
        }
        
        // Test with another known tag
        field = TIFFFieldWithName(tif, "ImageLength");
        if (field) {
            TIFFDataType dt = TIFFFieldDataType(field);
            uint32_t tag = TIFFFieldTag(field);
            (void)dt;
            (void)tag;
        }
        
        TIFFClose(tif);
    }
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}