// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "libtiff/tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static TIFF* create_test_tiff(const uint8_t *data, size_t size) {
    write_dummy_file(data, size);
    return TIFFOpen("./dummy_file", "r");
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    TIFF* tif = create_test_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzz data
    size_t offset = 0;
    
    // Get tag for TIFFFindField/TIFFFieldWithTag
    uint32_t tag = 0;
    if (Size - offset >= sizeof(uint32_t)) {
        memcpy(&tag, Data + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    } else {
        tag = Data[0];
    }
    
    // Get data type for TIFFFindField
    TIFFDataType dtype = TIFF_NOTYPE;
    if (Size - offset >= sizeof(TIFFDataType)) {
        memcpy(&dtype, Data + offset, sizeof(TIFFDataType));
        offset += sizeof(TIFFDataType);
    } else if (Size > 1) {
        dtype = (TIFFDataType)(Data[1] % 20); // Limit to valid enum range
    }
    
    // Get field name for TIFFFieldWithName
    char field_name[256] = {0};
    size_t name_len = Size - offset;
    if (name_len > 0) {
        if (name_len > 255) name_len = 255;
        memcpy(field_name, Data + offset, name_len);
        field_name[name_len] = '\0';
    } else {
        strcpy(field_name, "ImageWidth");
    }
    
    // Test TIFFFindField
    const TIFFField* field1 = TIFFFindField(tif, tag, dtype);
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFFieldName on found fields
    if (field1) {
        const char* name1 = TIFFFieldName(field1);
        (void)name1; // Use to avoid unused variable warning
    }
    
    if (field2) {
        const char* name2 = TIFFFieldName(field2);
        (void)name2;
    }
    
    // Test TIFFFieldDataType on found fields
    if (field1) {
        TIFFDataType dt1 = TIFFFieldDataType(field1);
        (void)dt1;
    }
    
    if (field2) {
        TIFFDataType dt2 = TIFFFieldDataType(field2);
        (void)dt2;
    }
    
    // Test TIFFFieldTag on found fields
    if (field1) {
        uint32_t tag1 = TIFFFieldTag(field1);
        (void)tag1;
    }
    
    if (field2) {
        uint32_t tag2 = TIFFFieldTag(field2);
        (void)tag2;
    }
    
    // Test TIFFFieldWithName
    const TIFFField* field3 = TIFFFieldWithName(tif, field_name);
    if (field3) {
        const char* name3 = TIFFFieldName(field3);
        TIFFDataType dt3 = TIFFFieldDataType(field3);
        uint32_t tag3 = TIFFFieldTag(field3);
        (void)name3;
        (void)dt3;
        (void)tag3;
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}