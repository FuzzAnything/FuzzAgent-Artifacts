// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
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
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write at least a minimal TIFF header (8 bytes) if input is too small
        if (Size < 8) {
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00}; // Little endian, offset 8
            fwrite(header, 1, 8, f);
        }
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment
    // Create a dummy file from input data
    write_dummy_file(Data, Size);

    // Open the file in read mode
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Try to read the directory to populate internal field structures
    // This is necessary for TIFFFindField and related functions to work correctly
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Step 2 & 3: Explore program states by invoking target functions diversely

    // Fuzz TIFFFieldWithTag with various tag IDs derived from input
    for (size_t i = 0; i < Size && i < 1000; i++) {
        uint32_t tag_id = 0;
        if (i + 3 < Size) {
            tag_id = Data[i] | (Data[i+1] << 8) | (Data[i+2] << 16) | (Data[i+3] << 24);
        } else {
            tag_id = Data[i];
        }

        const TIFFField *field = TIFFFieldWithTag(tif, tag_id);
        
        if (field != NULL) {
            // If field is found, call other functions that require a valid pointer
            const char *name = TIFFFieldName(field);
            if (name != NULL) {
                // Use name to prevent unused variable warning
                (void)strlen(name);
            }
            
            TIFFDataType type = TIFFFieldDataType(field);
            (void)type;
            
            uint32_t tag = TIFFFieldTag(field);
            (void)tag;
        }
    }

    // Fuzz TIFFFindField with various tag IDs and data types
    for (size_t i = 0; i < Size && i < 1000; i++) {
        uint32_t tag_id = 0;
        if (i + 3 < Size) {
            tag_id = Data[i] | (Data[i+1] << 8) | (Data[i+2] << 16) | (Data[i+3] << 24);
        } else {
            tag_id = Data[i];
        }
        
        // Cycle through a few common data types
        TIFFDataType dtype = (TIFFDataType)(i % 15);
        
        const TIFFField *field = TIFFFindField(tif, tag_id, dtype);
        
        if (field != NULL) {
            const char *name = TIFFFieldName(field);
            if (name != NULL) {
                (void)strlen(name);
            }
            TIFFDataType type = TIFFFieldDataType(field);
            (void)type;
            uint32_t tag = TIFFFieldTag(field);
            (void)tag;
        }
    }

    // Fuzz TIFFFieldWithName using input data as potential field names
    // We construct a buffer to ensure null-termination
    char name_buf[256];
    size_t max_len = Size < 255 ? Size : 255;
    memcpy(name_buf, Data, max_len);
    name_buf[max_len] = '\0';

    const TIFFField *field_name = TIFFFieldWithName(tif, name_buf);
    if (field_name != NULL) {
        const char *name = TIFFFieldName(field_name);
        if (name != NULL) {
            (void)strlen(name);
        }
        TIFFDataType type = TIFFFieldDataType(field_name);
        (void)type;
        uint32_t tag = TIFFFieldTag(field_name);
        (void)tag;
    }

    // Step 2: Cleanup
    TIFFClose(tif);
    
    // Clean up the dummy file
    remove("./dummy_file");

    return 0;
}