// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tiffio.h"

static void
write_dummy_file(const uint8_t *Data, size_t Size)
{
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        /* Write a minimal TIFF header to ensure TIFFOpen succeeds */
        uint16_t magic = 0x4949; /* Little-endian */
        uint16_t version = 42;
        uint32_t ifd_offset = 8;
        
        fwrite(&magic, sizeof(uint16_t), 1, f);
        fwrite(&version, sizeof(uint16_t), 1, f);
        fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
        
        /* Write the fuzzed data after the header */
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        
        fclose(f);
    }
}

int
LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size)
{
    /* Ensure we have some data to work with, or create a minimal valid file */
    if (Size == 0) {
        uint8_t dummy = 0;
        write_dummy_file(&dummy, 1);
    } else {
        write_dummy_file(Data, Size);
    }

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Fuzz TIFFFieldWithName using input data as potential tag names */
    /* We try to interpret bytes as a string, handling null terminators */
    for (size_t i = 0; i < Size; i++) {
        if (Data[i] == 0) {
            /* Found a null terminator, try to use the substring as a name */
            char name_buf[256];
            size_t len = i;
            if (len >= sizeof(name_buf)) len = sizeof(name_buf) - 1;
            memcpy(name_buf, Data, len);
            name_buf[len] = '\0';
            
            const TIFFField *field = TIFFFieldWithName(tif, name_buf);
            if (field != NULL) {
                uint32_t tag = TIFFFieldTag(field);
                (void)tag; /* Use tag to prevent unused variable warning */
                
                const char *name = TIFFFieldName(field);
                if (name != NULL) {
                    /* Just access the name to exercise the path */
                    (void)name;
                }
                
                TIFFDataType dtype = TIFFFieldDataType(field);
                (void)dtype;
            }
        }
    }

    /* Fuzz TIFFFindField and TIFFFieldWithTag using input data as tag IDs */
    for (size_t i = 0; i < Size; i++) {
        uint32_t tag_id = Data[i];
        
        /* Try with specific data types if we have enough data */
        TIFFDataType type = TIFF_ANY;
        if (i + 1 < Size) {
            type = (TIFFDataType)Data[i+1];
        }
        
        const TIFFField *field1 = TIFFFindField(tif, tag_id, type);
        if (field1 != NULL) {
            uint32_t t1 = TIFFFieldTag(field1);
            (void)t1;
            const char *n1 = TIFFFieldName(field1);
            if (n1 != NULL) (void)n1;
            TIFFDataType d1 = TIFFFieldDataType(field1);
            (void)d1;
        }
        
        const TIFFField *field2 = TIFFFieldWithTag(tif, tag_id);
        if (field2 != NULL) {
            uint32_t t2 = TIFFFieldTag(field2);
            (void)t2;
            const char *n2 = TIFFFieldName(field2);
            if (n2 != NULL) (void)n2;
            TIFFDataType d2 = TIFFFieldDataType(field2);
            (void)d2;
        }
    }

    /* Cleanup */
    TIFFClose(tif);
    
    /* Remove the dummy file */
    remove("./dummy_file");

    return 0;
}