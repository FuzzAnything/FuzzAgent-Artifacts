// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Extract parameters from fuzzed data
    uint32_t tag = 0;
    if (Size >= 4) {
        memcpy(&tag, Data, 4);
    }
    
    TIFFDataType data_type = TIFF_NOTYPE;
    if (Size >= 5) {
        data_type = (TIFFDataType)(Data[4] % (TIFF_IFD8 + 1));
    }
    
    // Test TIFFFindField
    const TIFFField* field1 = TIFFFindField(tif, tag, data_type);
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFFieldWithName - use a portion of data as field name
    const char* field_name = NULL;
    if (Size >= 8) {
        // Use up to 64 bytes for field name, null-terminated
        size_t name_len = Size - 8;
        if (name_len > 64) name_len = 64;
        char* temp_name = (char*)malloc(name_len + 1);
        if (temp_name) {
            memcpy(temp_name, Data + 8, name_len);
            temp_name[name_len] = '\0';
            const TIFFField* field3 = TIFFFieldWithName(tif, temp_name);
            if (field3) {
                // Test TIFFFieldName on the found field
                field_name = TIFFFieldName(field3);
            }
            free(temp_name);
        }
    }
    
    // Test field properties if we found any fields
    if (field1) {
        TIFFDataType dt1 = TIFFFieldDataType(field1);
        uint32_t tag1 = TIFFFieldTag(field1);
        const char* name1 = TIFFFieldName(field1);
        (void)dt1;
        (void)tag1;
        (void)name1;
    }
    
    if (field2) {
        TIFFDataType dt2 = TIFFFieldDataType(field2);
        uint32_t tag2 = TIFFFieldTag(field2);
        const char* name2 = TIFFFieldName(field2);
        (void)dt2;
        (void)tag2;
        (void)name2;
    }
    
    // Try another tag value for more coverage
    if (Size >= 12) {
        uint32_t tag3 = 0;
        memcpy(&tag3, Data + 8, 4);
        const TIFFField* field4 = TIFFFindField(tif, tag3, data_type);
        const TIFFField* field5 = TIFFFieldWithTag(tif, tag3);
        (void)field4;
        (void)field5;
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}