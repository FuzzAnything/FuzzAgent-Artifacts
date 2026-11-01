// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_tiff(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    // Write a minimal valid TIFF header (little endian, offset to first IFD at 8)
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(header, 1, 8, f);
    // Write dummy IFD (count 0, next 0)
    uint8_t ifd[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    fwrite(ifd, 1, 8, f);
    // Write remaining fuzz data
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    
    // Prepare environment: create a dummy file
    write_dummy_tiff(filename, Data, Size);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Fuzzing logic for target API functions

    // 1. TIFFFindField
    // Try various tag IDs and data types
    if (Size >= 4) {
        uint32_t tag_id = *(uint32_t *)Data;
        TIFFDataType dt = (Size >= 5) ? (TIFFDataType)Data[4] : TIFF_ANY;
        const TIFFField *field = TIFFFindField(tif, tag_id, dt);
        if (field) {
            // If found, explore further
            uint32_t tag = TIFFFieldTag(field);
            (void)tag; // Use variable to avoid unused warning
            TIFFDataType type = TIFFFieldDataType(field);
            (void)type;
            const char *name = TIFFFieldName(field);
            if (name) {
                // Safe to use name
                (void)name;
            }
        }
    }

    // 2. TIFFFieldWithTag
    if (Size >= 4) {
        uint32_t tag_id = *(uint32_t *)Data;
        const TIFFField *field = TIFFFieldWithTag(tif, tag_id);
        if (field) {
            uint32_t tag = TIFFFieldTag(field);
            (void)tag;
            TIFFDataType type = TIFFFieldDataType(field);
            (void)type;
            const char *name = TIFFFieldName(field);
            if (name) {
                (void)name;
            }
        }
    }

    // 3. TIFFFieldWithName
    // Construct a null-terminated string from input data
    if (Size > 0) {
        char name_buf[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(name_buf, Data, len);
        name_buf[len] = '\0';
        
        const TIFFField *field = TIFFFieldWithName(tif, name_buf);
        if (field) {
            uint32_t tag = TIFFFieldTag(field);
            (void)tag;
            TIFFDataType type = TIFFFieldDataType(field);
            (void)type;
            const char *name = TIFFFieldName(field);
            if (name) {
                (void)name;
            }
        }
    }

    // Cleanup
    TIFFClose(tif);
    
    return 0;
}