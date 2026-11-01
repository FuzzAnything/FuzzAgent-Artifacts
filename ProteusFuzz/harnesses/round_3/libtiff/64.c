// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void set_dummy_error_handler(TIFF *tif) {
    if (tif != NULL) {
        TIFFSetErrorHandler(NULL);
        TIFFSetWarningHandler(NULL);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to the dummy file
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return 0;
    }
    if (fwrite(Data, 1, Size, f) != Size) {
        fclose(f);
        return 0;
    }
    fclose(f);

    // Open the file for writing to test directory creation functions
    TIFF *tif = TIFFOpen(filename, "w8");
    if (tif == NULL) {
        return 0;
    }

    set_dummy_error_handler(tif);

    // Basic setup to make the TIFF valid for directory operations
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);

    // Fuzzing logic for target API functions

    // 1. TIFFCreateGPSDirectory
    // Initialize GPS directory
    (void)TIFFCreateGPSDirectory(tif);

    // 2. TIFFMergeFieldInfo
    // Create a minimal custom field info array to merge.
    // TIFFFieldInfo structure fields:
    // uint32_t field_tag
    // const char *field_name
    // short field_readcount
    // short field_writecount
    // TIFFDataType field_type
    // ... other fields
    // We define the array dynamically to avoid initializer issues with pointers.
    static const char *custom_tag_name = "CustomTag";
    TIFFFieldInfo custom_fields[1];
    custom_fields[0].field_tag = 0xFFFF;
    custom_fields[0].field_name = custom_tag_name;
    custom_fields[0].field_readcount = 1;
    custom_fields[0].field_writecount = 1;
    custom_fields[0].field_type = TIFF_SHORT;
    // Initialize remaining fields to 0
    memset((char*)&custom_fields[0] + sizeof(custom_fields[0].field_type), 0, 
           sizeof(TIFFFieldInfo) - sizeof(uint32_t) - sizeof(const char*) - sizeof(short) - sizeof(short) - sizeof(TIFFDataType));
    
    (void)TIFFMergeFieldInfo(tif, custom_fields, 1);

    // 5. TIFFCreateEXIFDirectory
    // Initialize EXIF directory
    (void)TIFFCreateEXIFDirectory(tif);

    // 6. TIFFWriteDirectory
    // Write the current directory (which now contains GPS/EXIF tags potentially)
    (void)TIFFWriteDirectory(tif);

    // 4. TIFFFreeDirectory
    // Free the directory resources
    TIFFFreeDirectory(tif);

    // Cleanup
    TIFFClose(tif);

    // Clean up the dummy file
    remove(filename);

    return 0;
}