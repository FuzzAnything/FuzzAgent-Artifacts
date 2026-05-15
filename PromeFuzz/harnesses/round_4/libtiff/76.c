// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
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
#include <stdio.h>

static const uint8_t minimal_tiff[] = {
    0x49, 0x49, 0x2A, 0x00,  // Little-endian TIFF header
    0x08, 0x00, 0x00, 0x00,  // First IFD offset
    0x00, 0x00,              // Number of directory entries (0)
    0x00, 0x00, 0x00, 0x00   // Next IFD offset (0)
};

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Create dummy TIFF file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write minimal valid TIFF header
    fwrite(minimal_tiff, 1, sizeof(minimal_tiff), fp);
    
    // Write fuzzer data as additional content
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzzer input
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_BYTE;
    char name_buf[256] = {0};
    
    if (Size >= 4) {
        tag = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    }
    
    if (Size >= 5) {
        dtype = (TIFFDataType)(Data[4] % (TIFF_IFD8 + 1)); // Modulo to valid enum range
    }
    
    if (Size >= 6) {
        size_t name_len = Size - 5;
        if (name_len > 255) name_len = 255;
        memcpy(name_buf, Data + 5, name_len);
        name_buf[name_len] = '\0';
    } else {
        strcpy(name_buf, "Artist"); // Default field name
    }
    
    // Test TIFFFindField
    const TIFFField *field1 = TIFFFindField(tif, tag, dtype);
    
    // Test TIFFFieldWithTag
    const TIFFField *field2 = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFFieldTag and TIFFFieldDataType on found fields
    if (field1) {
        uint32_t tag1 = TIFFFieldTag(field1);
        TIFFDataType dtype1 = TIFFFieldDataType(field1);
        int width1 = TIFFDataWidth(dtype1);
        (void)tag1; (void)dtype1; (void)width1; // Avoid unused warnings
    }
    
    if (field2) {
        uint32_t tag2 = TIFFFieldTag(field2);
        TIFFDataType dtype2 = TIFFFieldDataType(field2);
        int width2 = TIFFDataWidth(dtype2);
        (void)tag2; (void)dtype2; (void)width2; // Avoid unused warnings
    }
    
    // Test TIFFDataWidth with various data types
    for (int i = 0; i <= TIFF_IFD8; i++) {
        int width = TIFFDataWidth((TIFFDataType)i);
        (void)width;
    }
    
    // Test TIFFFieldWithName
    const TIFFField *field3 = TIFFFieldWithName(tif, name_buf);
    if (field3) {
        uint32_t tag3 = TIFFFieldTag(field3);
        TIFFDataType dtype3 = TIFFFieldDataType(field3);
        (void)tag3; (void)dtype3;
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}