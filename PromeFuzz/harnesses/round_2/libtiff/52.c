// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
// TIFFDataWidth at tif_dirinfo.c:637:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Use fuzzer data to generate parameters
    uint32_t tag = 0;
    if (Size >= 4) {
        memcpy(&tag, Data, 4);
    }
    
    TIFFDataType dtype = TIFF_NOTYPE;
    if (Size >= 5) {
        dtype = (TIFFDataType)(Data[4] % 14); // 14 is max TIFFDataType value
    }
    
    // Generate field name from fuzzer data
    char field_name[256] = {0};
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        memcpy(field_name, Data, name_len);
        field_name[name_len] = '\0';
    }
    
    // Test TIFFFieldWithName
    const TIFFField* field1 = TIFFFieldWithName(tif, field_name);
    if (field1) {
        // Test TIFFFieldDataType
        TIFFDataType dt = TIFFFieldDataType(field1);
        (void)dt;
        
        // Test TIFFFieldIsAnonymous
        int is_anon = TIFFFieldIsAnonymous(field1);
        (void)is_anon;
    }
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    if (field2) {
        TIFFDataType dt2 = TIFFFieldDataType(field2);
        (void)dt2;
        
        int is_anon2 = TIFFFieldIsAnonymous(field2);
        (void)is_anon2;
    }
    
    // Test TIFFFindField
    const TIFFField* field3 = TIFFFindField(tif, tag, dtype);
    if (field3) {
        TIFFDataType dt3 = TIFFFieldDataType(field3);
        (void)dt3;
        
        int is_anon3 = TIFFFieldIsAnonymous(field3);
        (void)is_anon3;
    }
    
    // Test TIFFDataWidth
    int width = TIFFDataWidth(dtype);
    (void)width;
    
    // Try with different data types
    for (int i = 0; i < 14; i++) {
        TIFFDataType test_dtype = (TIFFDataType)i;
        int test_width = TIFFDataWidth(test_dtype);
        (void)test_width;
    }
    
    // Try with different tags
    if (Size >= 8) {
        uint32_t tag2 = 0;
        memcpy(&tag2, Data + 4, 4);
        const TIFFField* field4 = TIFFFieldWithTag(tif, tag2);
        if (field4) {
            TIFFDataType dt4 = TIFFFieldDataType(field4);
            (void)dt4;
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}