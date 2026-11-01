// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty to ensure open succeeds
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Fuzzing logic for target API functions

    // 1. TIFFFieldWithTag
    // Try various tag IDs derived from input data
    uint32_t tag_id = 0;
    if (Size >= 4) {
        tag_id = *(const uint32_t *)(Data);
    } else if (Size > 0) {
        tag_id = Data[0];
    }

    const TIFFField *field_by_tag = TIFFFieldWithTag(tif, tag_id);
    if (field_by_tag) {
        // 2. TIFFFieldName (requires valid pointer)
        const char *name = TIFFFieldName(field_by_tag);
        if (name) {
            // Just consume the pointer to avoid unused variable warnings
            (void)name;
        }

        // 3. TIFFFieldTag
        uint32_t retrieved_tag = TIFFFieldTag(field_by_tag);
        (void)retrieved_tag;

        // 4. TIFFFieldDataType
        TIFFDataType dtype = TIFFFieldDataType(field_by_tag);
        (void)dtype;
    }

    // 5. TIFFFieldWithName
    // Use a portion of input data as a string name (null-terminate safely)
    if (Size > 1) {
        char *name_buf = (char *)malloc(Size + 1);
        if (name_buf) {
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
            
            const TIFFField *field_by_name = TIFFFieldWithName(tif, name_buf);
            if (field_by_name) {
                const char *name = TIFFFieldName(field_by_name);
                (void)name;
                uint32_t retrieved_tag = TIFFFieldTag(field_by_name);
                (void)retrieved_tag;
                TIFFDataType dtype = TIFFFieldDataType(field_by_name);
                (void)dtype;
            }
            free(name_buf);
        }
    }

    // 6. TIFFFindField
    // Try finding with specific type (TIFF_ANY) and derived tag
    TIFFDataType type = TIFF_ANY;
    if (Size >= 4) {
        // Use next 4 bytes to potentially select a type, clamp to valid range if needed
        uint32_t raw_type = *(const uint32_t *)(Data + 4);
        // Map arbitrary uint32 to a few known types for diversity, or keep as TIFF_ANY
        if (raw_type < 16) {
            type = (TIFFDataType)raw_type;
        }
    }
    
    const TIFFField *field_found = TIFFFindField(tif, tag_id, type);
    if (field_found) {
        const char *name = TIFFFieldName(field_found);
        (void)name;
        uint32_t retrieved_tag = TIFFFieldTag(field_found);
        (void)retrieved_tag;
        TIFFDataType dtype = TIFFFieldDataType(field_found);
        (void)dtype;
    }

    // Cleanup
    TIFFClose(tif);
    
    return 0;
}