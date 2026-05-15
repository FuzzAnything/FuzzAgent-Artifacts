// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static const TIFFField* get_tiff_field_from_data(const uint8_t* data, size_t size) {
    if (size < 4) {
        return NULL;
    }
    
    uint32_t tag;
    memcpy(&tag, data, sizeof(tag));
    
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return NULL;
    }
    
    fwrite(data + 4, 1, size - 4, dummy_file);
    fclose(dummy_file);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return NULL;
    }
    
    const TIFFField* field = TIFFFieldWithTag(tif, tag);
    
    TIFFClose(tif);
    remove("./dummy_file");
    
    return field;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    const TIFFField* field = get_tiff_field_from_data(Data, Size);
    if (!field) {
        return 0;
    }
    
    const char* field_name = TIFFFieldName(field);
    int write_count = TIFFFieldWriteCount(field);
    int read_count = TIFFFieldReadCount(field);
    int pass_count = TIFFFieldPassCount(field);
    int set_get_size = TIFFFieldSetGetSize(field);
    int set_get_count_size = TIFFFieldSetGetCountSize(field);
    
    volatile int dummy = 0;
    dummy += (field_name != NULL) ? 1 : 0;
    dummy += write_count;
    dummy += read_count;
    dummy += pass_count;
    dummy += set_get_size;
    dummy += set_get_count_size;
    
    return 0;
}