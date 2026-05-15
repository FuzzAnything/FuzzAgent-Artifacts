// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "libtiff/tiffio.h"

static int dummy_file_initialized = 0;

static void initialize_dummy_file(void) {
    if (dummy_file_initialized) return;
    
    // Create a minimal valid TIFF file for testing
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write TIFF header (little-endian)
    uint8_t header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, magic number 42
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00,              // End of IFD
    };
    fwrite(header, 1, sizeof(header), f);
    fclose(f);
    dummy_file_initialized = 1;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    initialize_dummy_file();
    
    // Open dummy TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;
    
    // Use fuzz data to generate parameters
    uint32_t tag = 0;
    if (Size >= sizeof(uint32_t)) {
        memcpy(&tag, Data, sizeof(uint32_t));
    } else {
        tag = Data[0];
    }
    
    // Generate data type from fuzz input
    TIFFDataType data_type = (TIFFDataType)(Data[0] % 13); // 13 is max enum value
    
    // Generate field name from fuzz input
    char field_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(field_name, Data, name_len);
    field_name[name_len] = '\0';
    
    // Test TIFFFieldWithName
    const TIFFField* field1 = TIFFFieldWithName(tif, field_name);
    if (field1) {
        // Test TIFFFieldDataType on the returned field
        TIFFDataType dt = TIFFFieldDataType(field1);
        (void)dt;
        
        // Test TIFFFieldTag on the returned field
        uint32_t tag_val = TIFFFieldTag(field1);
        (void)tag_val;
        
        // Test TIFFFieldName on the returned field
        const char* name = TIFFFieldName(field1);
        (void)name;
    }
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    if (field2) {
        // Test related functions on this field
        TIFFDataType dt2 = TIFFFieldDataType(field2);
        (void)dt2;
        
        uint32_t tag_val2 = TIFFFieldTag(field2);
        (void)tag_val2;
        
        const char* name2 = TIFFFieldName(field2);
        (void)name2;
    }
    
    // Test TIFFFindField
    const TIFFField* field3 = TIFFFindField(tif, tag, data_type);
    if (field3) {
        // Test related functions on this field
        TIFFDataType dt3 = TIFFFieldDataType(field3);
        (void)dt3;
        
        uint32_t tag_val3 = TIFFFieldTag(field3);
        (void)tag_val3;
        
        const char* name3 = TIFFFieldName(field3);
        (void)name3;
    }
    
    // Try different combinations to explore more states
    if (Size > 1) {
        // Use different tag from the input
        uint32_t tag2 = 0;
        if (Size >= 2 * sizeof(uint32_t)) {
            memcpy(&tag2, Data + sizeof(uint32_t), sizeof(uint32_t));
        } else {
            tag2 = Data[1];
        }
        
        // Test with different data type
        TIFFDataType data_type2 = (TIFFDataType)((Data[0] + Data[Size-1]) % 13);
        
        const TIFFField* field4 = TIFFFindField(tif, tag2, data_type2);
        if (field4) {
            TIFFFieldDataType(field4);
            TIFFFieldTag(field4);
            TIFFFieldName(field4);
        }
        
        // Test TIFFFieldWithTag with different tag
        const TIFFField* field5 = TIFFFieldWithTag(tif, tag2);
        if (field5) {
            TIFFFieldDataType(field5);
            TIFFFieldTag(field5);
            TIFFFieldName(field5);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}