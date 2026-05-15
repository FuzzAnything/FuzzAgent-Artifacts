// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
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

static void test_field_functions(const TIFFField* field) {
    if (field == NULL) return;
    
    // Test all six API functions
    TIFFFieldIsAnonymous(field);
    TIFFFieldSetGetCountSize(field);
    TIFFFieldTag(field);
    TIFFFieldName(field);
    TIFFFieldPassCount(field);
    TIFFFieldReadCount(field);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Create a dummy TIFF file
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    // Write minimal TIFF header (8 bytes) plus some data
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(header, 1, sizeof(header), fp);
    if (Size > 0) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
    }
    fclose(fp);
    
    // Open the dummy TIFF file
    TIFF* tif = TIFFOpen(filename, "r");
    if (tif) {
        // Get directory information to access fields
        TIFFReadDirectory(tif);
        
        // Try to get fields using different methods
        // Test with known tags if available
        const TIFFField* field;
        
        // Try TIFFTAG_IMAGEWIDTH (256)
        field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_ANY);
        test_field_functions(field);
        
        // Try TIFFTAG_IMAGELENGTH (257)
        field = TIFFFindField(tif, TIFFTAG_IMAGELENGTH, TIFF_ANY);
        test_field_functions(field);
        
        // Try TIFFTAG_BITSPERSAMPLE (258)
        field = TIFFFindField(tif, TIFFTAG_BITSPERSAMPLE, TIFF_ANY);
        test_field_functions(field);
        
        // Try TIFFTAG_COMPRESSION (259)
        field = TIFFFindField(tif, TIFFTAG_COMPRESSION, TIFF_ANY);
        test_field_functions(field);
        
        // Also test with field name lookup
        field = TIFFFieldWithName(tif, "ImageWidth");
        test_field_functions(field);
        
        field = TIFFFieldWithName(tif, "ImageLength");
        test_field_functions(field);
        
        // Test with NULL pointer
        test_field_functions(NULL);
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Remove dummy file
    remove(filename);
    
    return 0;
}