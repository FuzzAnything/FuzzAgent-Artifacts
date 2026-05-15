// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "libtiff/tiffio.h"

static void fuzz_tiff_field_functions(const TIFFField *field) {
    if (field == NULL) {
        return;
    }
    
    // Call all target API functions with the field
    int pass_count = TIFFFieldPassCount(field);
    int is_anonymous = TIFFFieldIsAnonymous(field);
    int read_count = TIFFFieldReadCount(field);
    int set_get_size = TIFFFieldSetGetSize(field);
    int set_get_count_size = TIFFFieldSetGetCountSize(field);
    int write_count = TIFFFieldWriteCount(field);
    
    // Use the results to avoid dead code elimination
    (void)pass_count;
    (void)is_anonymous;
    (void)read_count;
    (void)set_get_size;
    (void)set_get_count_size;
    (void)write_count;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Create a dummy TIFF file with minimal valid header
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    
    // Write minimal TIFF header (little-endian)
    uint8_t tiff_header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, magic number 42
        0x08, 0x00, 0x00, 0x00,  // Offset to first IFD: 8 bytes
        0x00, 0x00,              // Number of directory entries: 0
        0x00, 0x00, 0x00, 0x00   // Offset to next IFD: 0 (end)
    };
    fwrite(tiff_header, 1, sizeof(tiff_header), fp);
    fclose(fp);
    
    // Open the dummy TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }
    
    // Try to get TIFFField objects using different methods
    // First, try to get fields by known tags
    uint32_t tags_to_try[] = {
        TIFFTAG_IMAGEWIDTH,      // 256
        TIFFTAG_IMAGELENGTH,     // 257
        TIFFTAG_BITSPERSAMPLE,   // 258
        TIFFTAG_COMPRESSION,     // 259
        TIFFTAG_PHOTOMETRIC,     // 262
        TIFFTAG_STRIPOFFSETS,    // 273
        TIFFTAG_SAMPLESPERPIXEL, // 277
        TIFFTAG_ROWSPERSTRIP,    // 278
        TIFFTAG_STRIPBYTECOUNTS, // 279
        TIFFTAG_XRESOLUTION,     // 282
        TIFFTAG_YRESOLUTION,     // 283
        TIFFTAG_PLANARCONFIG,    // 284
        TIFFTAG_RESOLUTIONUNIT,  // 296
    };
    
    // Use fuzzer data to select which tags to try
    size_t num_tags = sizeof(tags_to_try) / sizeof(tags_to_try[0]);
    
    // Try multiple fields to explore different code paths
    for (size_t i = 0; i < 3 && i < num_tags && i * 4 < Size; i++) {
        // Use fuzzer data to select tag index
        uint32_t tag_index = 0;
        if (i * 4 + 4 <= Size) {
            memcpy(&tag_index, Data + i * 4, 4);
        }
        tag_index = tag_index % num_tags;
        
        const TIFFField *field = TIFFFieldWithTag(tif, tags_to_try[tag_index]);
        fuzz_tiff_field_functions(field);
    }
    
    // Also try to find fields by name if we have enough data
    if (Size > 0) {
        // Create a null-terminated string from fuzzer data for field name
        char field_name[256];
        size_t name_len = Size < 255 ? Size : 255;
        memcpy(field_name, Data, name_len);
        field_name[name_len] = '\0';
        
        const TIFFField *field_by_name = TIFFFieldWithName(tif, field_name);
        fuzz_tiff_field_functions(field_by_name);
    }
    
    // Try to get fields using TIFFFindField
    if (Size > 0) {
        // Use first byte as tag to search for
        uint32_t search_tag = Data[0];
        if (Size > 4) {
            memcpy(&search_tag, Data, 4);
        }
        
        const TIFFField *found_field = TIFFFindField(tif, search_tag, TIFF_ANY);
        fuzz_tiff_field_functions(found_field);
    }
    
    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}