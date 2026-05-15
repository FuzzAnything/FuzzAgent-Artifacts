// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldIsAnonymous at tif_dirinfo.c:879:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_tiff_field_functions(const TIFFField *field) {
    if (field == NULL) {
        // Only call functions that are documented to handle NULL input
        TIFFFieldSetGetSize(NULL);
        TIFFFieldSetGetCountSize(NULL);
        // Do not call TIFFFieldIsAnonymous, TIFFFieldWriteCount, 
        // TIFFFieldPassCount, or TIFFFieldReadCount with NULL
        return;
    }
    
    // Call all target functions with the field
    TIFFFieldSetGetSize(field);
    TIFFFieldIsAnonymous(field);
    TIFFFieldSetGetCountSize(field);
    TIFFFieldWriteCount(field);
    TIFFFieldPassCount(field);
    TIFFFieldReadCount(field);
}

static const TIFFField* get_field_from_input(const uint8_t *data, size_t size) {
    // Use the fuzzer input to try different ways to get TIFFField pointers
    
    // Method 1: Try to interpret input as tag number
    if (size >= sizeof(uint32_t)) {
        uint32_t tag;
        memcpy(&tag, data, sizeof(uint32_t));
        // Try to find field by tag (requires TIFF handle)
        // We'll create a dummy TIFF for this
        TIFF* tif = TIFFOpen("./dummy_file", "w");
        if (tif) {
            const TIFFField* field = TIFFFindField(tif, tag, TIFF_ANY);
            TIFFClose(tif);
            if (field) {
                return field;
            }
        }
    }
    
    // Method 2: Try to interpret input as field name
    if (size > 0) {
        // Create a null-terminated string from input (limited to 100 chars)
        char name[101];
        size_t name_len = size < 100 ? size : 100;
        memcpy(name, data, name_len);
        name[name_len] = '\0';
        
        TIFF* tif = TIFFOpen("./dummy_file", "w");
        if (tif) {
            const TIFFField* field = TIFFFieldWithName(tif, name);
            TIFFClose(tif);
            if (field) {
                return field;
            }
        }
    }
    
    return NULL;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Create a dummy TIFF file for library functions that need it
    FILE* dummy = fopen("./dummy_file", "wb");
    if (dummy) {
        // Write minimal valid TIFF header (8 bytes)
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, sizeof(header), dummy);
        fclose(dummy);
    }
    
    // Get TIFFField pointer from fuzzer input
    const TIFFField* field = get_field_from_input(Data, Size);
    
    // Fuzz the target functions
    fuzz_tiff_field_functions(field);
    
    // Also test with NULL for functions that handle it
    if (Size == 0) {
        // Only call functions that are documented to handle NULL
        TIFFFieldSetGetSize(NULL);
        TIFFFieldSetGetCountSize(NULL);
    }
    
    // Try to get more diverse field pointers by opening a TIFF and using known tags
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Try a set of common TIFF tags to get field pointers
        uint32_t common_tags[] = {
            256,  // ImageWidth
            257,  // ImageLength
            258,  // BitsPerSample
            259,  // Compression
            262,  // PhotometricInterpretation
            273,  // StripOffsets
            277,  // SamplesPerPixel
            278,  // RowsPerStrip
            279,  // StripByteCounts
            282,  // XResolution
            283,  // YResolution
            296,  // ResolutionUnit
            317,  // Predictor
            320,  // ColorMap
            338,  // ExtraSamples
        };
        
        // Use fuzzer input to select which tags to try
        for (size_t i = 0; i < sizeof(common_tags)/sizeof(common_tags[0]); i++) {
            if (Size == 0 || (Data[0] % sizeof(common_tags)/sizeof(common_tags[0])) == i) {
                const TIFFField* common_field = TIFFFindField(tif, common_tags[i], TIFF_ANY);
                if (common_field) {
                    fuzz_tiff_field_functions(common_field);
                }
            }
        }
        
        // Always test a few specific tags for coverage
        const TIFFField* width_field = TIFFFindField(tif, 256, TIFF_ANY);  // ImageWidth
        if (width_field) {
            fuzz_tiff_field_functions(width_field);
        }
        
        const TIFFField* compression_field = TIFFFindField(tif, 259, TIFF_ANY);  // Compression
        if (compression_field) {
            fuzz_tiff_field_functions(compression_field);
        }
        
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}