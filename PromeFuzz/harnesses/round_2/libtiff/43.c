// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

static void fuzz_tiff_field(const TIFFField *field) {
    if (field == NULL) {
        return;
    }
    
    // Call all target API functions
    TIFFFieldPassCount(field);
    TIFFFieldWriteCount(field);
    TIFFFieldSetGetCountSize(field);
    TIFFFieldReadCount(field);
    TIFFFieldSetGetSize(field);
    TIFFFieldName(field);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Create a dummy TIFF file to get valid TIFFField structures
    const char* filename = "./dummy_file.tiff";
    
    // Write minimal TIFF data to file
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write minimal TIFF header (8 bytes: II, 42, IFD offset 8)
    uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(header, 1, sizeof(header), fp);
    
    // Write IFD with 0 entries (2 bytes for count, 4 bytes for next IFD offset)
    uint8_t ifd[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    fwrite(ifd, 1, sizeof(ifd), fp);
    
    fclose(fp);
    
    // Open the TIFF file to get access to field functions
    TIFF* tif = TIFFOpen(filename, "r");
    if (tif) {
        // Use fuzzer data to generate tag and data type values
        uint32_t tag = 0;
        TIFFDataType dtype = TIFF_NOTYPE;
        
        // Process the input data in chunks to test multiple fields
        size_t i = 0;
        while (i + 5 <= Size && i < 100) { // Limit iterations to avoid timeout
            // Extract tag (4 bytes) and data type (1 byte) from fuzzer input
            if (i + 4 <= Size) {
                memcpy(&tag, Data + i, 4);
                i += 4;
            }
            
            if (i < Size) {
                // Use the next byte as data type (cast to TIFFDataType)
                dtype = (TIFFDataType)Data[i];
                i += 1;
            }
            
            // Try to find a field with this tag and data type
            const TIFFField* field = TIFFFindField(tif, tag, dtype);
            
            // Test with the found field
            fuzz_tiff_field(field);
        }
        
        // Also test with NULL pointer
        fuzz_tiff_field(NULL);
        
        TIFFClose(tif);
    }
    
    // Clean up
    remove(filename);
    
    return 0;
}