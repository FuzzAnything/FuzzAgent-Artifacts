// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void dummy_cleanup(TIFF *tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

static const TIFFField *safe_find_field(TIFF *tif, uint32_t tag, TIFFDataType type) {
    if (!tif) return NULL;
    return TIFFFindField(tif, tag, type);
}

static const TIFFField *safe_find_field_by_name(TIFF *tif, const char *name) {
    if (!tif || !name) return NULL;
    return TIFFFieldWithName(tif, name);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "rb");
    if (!tif) {
        // Try opening as write mode if read fails, to test different code paths
        tif = TIFFOpen(filename, "wb");
        if (tif) {
            // Write minimal header to make it a valid TIFF for some operations
            uint16_t dummy = 0;
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            // Close and reopen for reading to ensure directory is written
            TIFFClose(tif);
            tif = TIFFOpen(filename, "rb");
        }
    }

    if (!tif) {
        return 0;
    }

    // Extract some values from input for fuzzing parameters
    uint32_t tag_id = 0;
    if (Size >= 4) {
        tag_id = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    TIFFDataType data_type = TIFF_BYTE;
    if (Size >= 5) {
        uint8_t type_val = Data[4] % 16; // Limit to reasonable TIFF data types
        data_type = (TIFFDataType)type_val;
    }

    // Fuzz TIFFFieldWithName
    if (Size >= 10) {
        char name_buf[256];
        size_t name_len = (Size > 256) ? 255 : Size - 10;
        memcpy(name_buf, Data + 10, name_len);
        name_buf[name_len] = '\0';
        
        const TIFFField *field_by_name = safe_find_field_by_name(tif, name_buf);
        if (field_by_name) {
            // If found, test other functions on this field
            const char *name = TIFFFieldName(field_by_name);
            if (name) {
                // Use name to avoid unused variable warning
                volatile const char *v = name;
                (void)v;
            }
            uint32_t tag = TIFFFieldTag(field_by_name);
            (void)tag;
            int pass_count = TIFFFieldPassCount(field_by_name);
            (void)pass_count;
            TIFFDataType dtype = TIFFFieldDataType(field_by_name);
            (void)dtype;
        }
    }

    // Fuzz TIFFFindField
    const TIFFField *field_by_tag = safe_find_field(tif, tag_id, data_type);
    if (field_by_tag) {
        const char *name = TIFFFieldName(field_by_tag);
        if (name) {
            volatile const char *v = name;
            (void)v;
        }
        uint32_t tag = TIFFFieldTag(field_by_tag);
        (void)tag;
        int pass_count = TIFFFieldPassCount(field_by_tag);
        (void)pass_count;
        TIFFDataType dtype = TIFFFieldDataType(field_by_tag);
        (void)dtype;
    }

    // Test with TIFF_ANY type
    const TIFFField *field_any = safe_find_field(tif, tag_id, TIFF_ANY);
    if (field_any) {
        const char *name = TIFFFieldName(field_any);
        if (name) {
            volatile const char *v = name;
            (void)v;
        }
        uint32_t tag = TIFFFieldTag(field_any);
        (void)tag;
        int pass_count = TIFFFieldPassCount(field_any);
        (void)pass_count;
        TIFFDataType dtype = TIFFFieldDataType(field_any);
        (void)dtype;
    }

    // Test with common known tags
    uint32_t common_tags[] = {
        TIFFTAG_IMAGEWIDTH, TIFFTAG_IMAGELENGTH, TIFFTAG_BITSPERSAMPLE,
        TIFFTAG_COMPRESSION, TIFFTAG_PHOTOMETRIC, TIFFTAG_ROWSPERSTRIP,
        TIFFTAG_SAMPLESPERPIXEL, TIFFTAG_PLANARCONFIG, TIFFTAG_STRIPBYTECOUNTS,
        TIFFTAG_STRIPOFFSETS, TIFFTAG_XRESOLUTION, TIFFTAG_YRESOLUTION,
        TIFFTAG_RESOLUTIONUNIT, TIFFTAG_SOFTWARE, TIFFTAG_DATETIME
    };
    size_t num_common_tags = sizeof(common_tags) / sizeof(common_tags[0]);
    for (size_t i = 0; i < num_common_tags; i++) {
        const TIFFField *f = safe_find_field(tif, common_tags[i], TIFF_ANY);
        if (f) {
            const char *n = TIFFFieldName(f);
            if (n) { volatile const char *v = n; (void)v; }
            uint32_t t = TIFFFieldTag(f);
            (void)t;
            int pc = TIFFFieldPassCount(f);
            (void)pc;
            TIFFDataType dt = TIFFFieldDataType(f);
            (void)dt;
        }
    }

    // Cleanup
    dummy_cleanup(tif);
    
    // Remove dummy file
    remove(filename);

    return 0;
}