// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
// TIFFFieldWithName at tif_dirinfo.c:856:18 in tiffio.h
// TIFFFieldTag at tif_dirinfo.c:867:10 in tiffio.h
// TIFFFieldName at tif_dirinfo.c:869:13 in tiffio.h
// TIFFFieldDataType at tif_dirinfo.c:871:14 in tiffio.h
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
#include <unistd.h>

static void create_dummy_tiff(const char* filename) {
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) return;
    
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, width));
    
    unsigned char pixel = 0;
    for (uint32_t row = 0; row < height; row++) {
        if (TIFFWriteScanline(tif, &pixel, row, 0) < 0) {
            break;
        }
    }
    
    TIFFClose(tif);
}

static void test_field_functions(TIFF* tif, uint32_t tag, TIFFDataType type, const char* name) {
    if (!tif) return;
    
    const TIFFField* field1 = TIFFFindField(tif, tag, type);
    if (field1) {
        uint32_t field_tag = TIFFFieldTag(field1);
        const char* field_name = TIFFFieldName(field1);
        TIFFDataType field_type = TIFFFieldDataType(field1);
        (void)field_tag;
        (void)field_name;
        (void)field_type;
    }
    
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    if (field2) {
        uint32_t field_tag = TIFFFieldTag(field2);
        const char* field_name = TIFFFieldName(field2);
        TIFFDataType field_type = TIFFFieldDataType(field2);
        (void)field_tag;
        (void)field_name;
        (void)field_type;
    }
    
    const TIFFField* field3 = TIFFFieldWithName(tif, name);
    if (field3) {
        uint32_t field_tag = TIFFFieldTag(field3);
        const char* field_name = TIFFFieldName(field3);
        TIFFDataType field_type = TIFFFieldDataType(field3);
        (void)field_tag;
        (void)field_name;
        (void)field_type;
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    const char* dummy_filename = "./dummy_file";
    create_dummy_tiff(dummy_filename);
    
    TIFF* tif = TIFFOpen(dummy_filename, "r");
    if (!tif) {
        unlink(dummy_filename);
        return 0;
    }
    
    uint32_t tag = *(uint32_t*)Data;
    TIFFDataType type = (TIFFDataType)(Data[4] % (TIFF_IFD8 + 1));
    
    char* name = NULL;
    if (Size > 8) {
        size_t name_len = Size - 8;
        if (name_len > 100) name_len = 100;
        name = (char*)malloc(name_len + 1);
        if (name) {
            memcpy(name, Data + 8, name_len);
            name[name_len] = '\0';
        }
    }
    
    if (!name) {
        name = (char*)"ImageWidth";
    }
    
    test_field_functions(tif, tag, type, name);
    
    if (name && name != "ImageWidth") {
        free(name);
    }
    
    TIFFClose(tif);
    unlink(dummy_filename);
    
    return 0;
}