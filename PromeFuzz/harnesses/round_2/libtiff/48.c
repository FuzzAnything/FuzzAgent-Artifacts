// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
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
#include <fcntl.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Write fuzz data to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Use fuzz data to generate parameters
    uint32_t tag = 0;
    TIFFDataType data_type = TIFF_NOTYPE;
    const char* field_name = NULL;
    
    // Extract tag from first 4 bytes
    if (Size >= 4) {
        memcpy(&tag, Data, 4);
    }
    
    // Extract data type from next byte (modulo to valid range)
    if (Size >= 5) {
        data_type = (TIFFDataType)(Data[4] % 14); // 14 is number of TIFFDataType values
    }
    
    // Extract field name from remaining bytes (if any)
    if (Size > 5) {
        // Create a null-terminated string for field name
        size_t name_len = Size - 5;
        char* name_buf = malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, Data + 5, name_len);
            name_buf[name_len] = '\0';
            field_name = name_buf;
        }
    }
    
    // Test TIFFFieldWithTag
    const TIFFField* field1 = TIFFFieldWithTag(tif, tag);
    if (field1) {
        // Test related functions on valid field
        TIFFFieldDataType(field1);
        TIFFFieldTag(field1);
        TIFFFieldName(field1);
    }
    
    // Test TIFFFieldWithName if we have a field name
    if (field_name) {
        const TIFFField* field2 = TIFFFieldWithName(tif, field_name);
        if (field2) {
            TIFFFieldDataType(field2);
            TIFFFieldTag(field2);
            TIFFFieldName(field2);
        }
    }
    
    // Test TIFFFindField
    const TIFFField* field3 = TIFFFindField(tif, tag, data_type);
    if (field3) {
        TIFFFieldDataType(field3);
        TIFFFieldTag(field3);
        TIFFFieldName(field3);
    }
    
    // Test with various tag values from fuzz data
    if (Size >= 8) {
        uint32_t tag2 = 0;
        memcpy(&tag2, Data + 4, 4);
        const TIFFField* field4 = TIFFFieldWithTag(tif, tag2);
        if (field4) {
            TIFFFieldDataType(field4);
        }
    }
    
    // Test with different data types
    if (Size >= 6) {
        TIFFDataType data_type2 = (TIFFDataType)(Data[5] % 14);
        const TIFFField* field5 = TIFFFindField(tif, tag, data_type2);
        if (field5) {
            TIFFFieldTag(field5);
        }
    }
    
    // Cleanup
    if (field_name && Size > 5) {
        free((void*)field_name);
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}