// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as new file if input wasn't valid TIFF
        tif = TIFFOpen("./dummy_file", "w+");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Prepare variables for TIFFGetField
    uint32_t width = 0, height = 0;
    uint16_t bits_per_sample = 0, samples_per_pixel = 0;
    uint16_t photometric = 0, compression = 0;
    uint32_t rows_per_strip = 0;
    uint16_t planar_config = 0;
    float x_resolution = 0.0, y_resolution = 0.0;
    uint16_t resolution_unit = 0;
    
    // Call functions in specified order
    // 1. TIFFGetField
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &x_resolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &y_resolution);
    TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &resolution_unit);
    
    // 2. TIFFSetSubDirectory
    uint64_t subdir_offset = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&subdir_offset, Data, sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, subdir_offset);
    
    // 3. TIFFReadDirectory
    TIFFReadDirectory(tif);
    
    // 4. TIFFSetDirectory
    tdir_t dir_num = 0;
    if (Size >= sizeof(tdir_t) + sizeof(uint64_t)) {
        memcpy(&dir_num, Data + sizeof(uint64_t), sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_num);
    
    // 5. TIFFSetSubDirectory
    uint64_t subdir_offset2 = 0;
    if (Size >= 2 * sizeof(uint64_t) + sizeof(tdir_t)) {
        memcpy(&subdir_offset2, Data + sizeof(uint64_t) + sizeof(tdir_t), sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, subdir_offset2);
    
    // 6. TIFFReadDirectory
    TIFFReadDirectory(tif);
    
    // 7. TIFFSetDirectory
    tdir_t dir_num2 = 0;
    if (Size >= 2 * sizeof(uint64_t) + 2 * sizeof(tdir_t)) {
        memcpy(&dir_num2, Data + 2 * sizeof(uint64_t) + sizeof(tdir_t), sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_num2);
    
    // 8. TIFFReadDirectory
    TIFFReadDirectory(tif);
    
    // 9. TIFFCurrentDirectory
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    
    // 10. TIFFSetSubDirectory
    uint64_t subdir_offset3 = 0;
    if (Size >= 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t)) {
        memcpy(&subdir_offset3, Data + 2 * sizeof(uint64_t) + 2 * sizeof(tdir_t), sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, subdir_offset3);
    
    // 11. TIFFNumberOfDirectories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    // 12. TIFFCreateDirectory
    TIFFCreateDirectory(tif);
    
    // 13. TIFFSetField
    uint32_t new_width = 100;
    uint32_t new_height = 100;
    uint16_t new_bits = 8;
    uint16_t new_samples = 1;
    uint16_t new_photo = PHOTOMETRIC_MINISBLACK;
    uint16_t new_compression = COMPRESSION_NONE;
    uint32_t new_rows = 1;
    uint16_t new_planar = PLANARCONFIG_CONTIG;
    float new_xres = 72.0;
    float new_yres = 72.0;
    uint16_t new_resunit = RESUNIT_INCH;
    
    if (Size >= 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + sizeof(uint32_t)) {
        memcpy(&new_width, Data + 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t), sizeof(uint32_t));
        new_width = new_width % 1000 + 1; // Keep reasonable size
    }
    if (Size >= 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + 2 * sizeof(uint32_t)) {
        memcpy(&new_height, Data + 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + sizeof(uint32_t), sizeof(uint32_t));
        new_height = new_height % 1000 + 1;
    }
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, new_width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, new_height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, new_bits);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, new_samples);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, new_photo);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, new_compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, new_rows);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, new_planar);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, new_xres);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, new_yres);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, new_resunit);
    
    // 14. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 15. TIFFSetField (set different values)
    uint32_t new_width2 = 200;
    uint32_t new_height2 = 200;
    if (Size >= 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + 3 * sizeof(uint32_t)) {
        memcpy(&new_width2, Data + 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + 2 * sizeof(uint32_t), sizeof(uint32_t));
        new_width2 = new_width2 % 1000 + 1;
    }
    if (Size >= 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + 4 * sizeof(uint32_t)) {
        memcpy(&new_height2, Data + 3 * sizeof(uint64_t) + 2 * sizeof(tdir_t) + 3 * sizeof(uint32_t), sizeof(uint32_t));
        new_height2 = new_height2 % 1000 + 1;
    }
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, new_width2);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, new_height2);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)16);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)3);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    
    // 16. TIFFCheckpointDirectory
    TIFFCheckpointDirectory(tif);
    
    // 17. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 18. TIFFCheckpointDirectory
    TIFFCheckpointDirectory(tif);
    
    // 19. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 20. TIFFClose
    TIFFClose(tif);
    
    // 21. TIFFClose (on NULL to test error path)
    TIFFClose(NULL);
    
    // Clean up
    unlink("./dummy_file");
    
    // Use variables to avoid compiler warnings
    (void)width;
    (void)height;
    (void)bits_per_sample;
    (void)samples_per_pixel;
    (void)photometric;
    (void)compression;
    (void)rows_per_strip;
    (void)planar_config;
    (void)x_resolution;
    (void)y_resolution;
    (void)resolution_unit;
    (void)current_dir;
    (void)num_dirs;
    
    return 0;
}