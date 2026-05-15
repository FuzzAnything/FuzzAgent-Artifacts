// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    // Extract parameters from fuzzer input
    uint32_t tag = *(uint32_t *)Data;
    TIFFDataType dtype = (TIFFDataType)(Data[0] % 14); // 0-13 for valid TIFFDataType values
    
    // For field name - use a portion of input as string
    size_t name_len = Size > 8 ? (Size - 8) % 256 : 0;
    char *field_name = NULL;
    if (name_len > 0) {
        field_name = (char *)malloc(name_len + 1);
        if (field_name) {
            memcpy(field_name, Data + 8, name_len);
            field_name[name_len] = '\0';
        }
    }
    
    // Test TIFFFindField
    const TIFFField *field1 = TIFFFindField(tif, tag, dtype);
    
    // Test TIFFFieldWithName
    const TIFFField *field2 = NULL;
    if (field_name) {
        field2 = TIFFFieldWithName(tif, field_name);
    }
    
    // Test TIFFFieldWithTag
    const TIFFField *field3 = TIFFFieldWithTag(tif, tag);
    
    // Test field accessor functions
    if (field1) {
        TIFFDataType dt1 = TIFFFieldDataType(field1);
        uint32_t tag1 = TIFFFieldTag(field1);
        const char *name1 = TIFFFieldName(field1);
        (void)dt1;
        (void)tag1;
        (void)name1;
    }
    
    if (field2) {
        TIFFDataType dt2 = TIFFFieldDataType(field2);
        uint32_t tag2 = TIFFFieldTag(field2);
        const char *name2 = TIFFFieldName(field2);
        (void)dt2;
        (void)tag2;
        (void)name2;
    }
    
    if (field3) {
        TIFFDataType dt3 = TIFFFieldDataType(field3);
        uint32_t tag3 = TIFFFieldTag(field3);
        const char *name3 = TIFFFieldName(field3);
        (void)dt3;
        (void)tag3;
        (void)name3;
    }
    
    // Cleanup
    if (field_name) free(field_name);
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}