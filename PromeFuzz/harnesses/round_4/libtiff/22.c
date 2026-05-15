// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Silence libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file in read/write mode
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only mode if r+ fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            // Try creating new file if reading fails
            tif = TIFFOpen("./dummy_file", "w");
            if (!tif) {
                return 0;
            }
        }
    }
    
    // Create a directory if needed (for TIFFWriteDirectory)
    if (TIFFCurrentDirectory(tif) == 0) {
        // Set some minimal required tags for a valid directory
        uint32 width = 1;
        uint32 height = 1;
        uint16 samplesperpixel = 1;
        uint16 bitspersample = 8;
        uint16 photometric = PHOTOMETRIC_MINISBLACK;
        uint16 planarconfig = PLANARCONFIG_CONTIG;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
    }
    
    // Call functions in specified order
    // 1. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 2. TIFFSetDirectory (use first byte of data as directory index)
    tdir_t dir_index = 0;
    if (Size > 0) {
        dir_index = Data[0] % 10; // Limit to reasonable range
    }
    TIFFSetDirectory(tif, dir_index);
    
    // 3. TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset; // Use offset to avoid unused variable warning
    
    // 4. TIFFSetDirectory (use second byte if available)
    if (Size > 1) {
        dir_index = Data[1] % 10;
    } else {
        dir_index = 0;
    }
    TIFFSetDirectory(tif, dir_index);
    
    // 5. TIFFForceStrileArrayWriting
    // Only call if we're in write mode and have a directory
    if (TIFFIsTiled(tif) || TIFFIsUpSampled(tif)) {
        // Try to set up conditions for strile array writing
        // Set some strile-related fields
        uint32 tilewidth = 16;
        uint32 tilelength = 16;
        if (TIFFIsTiled(tif)) {
            TIFFSetField(tif, TIFFTAG_TILEWIDTH, tilewidth);
            TIFFSetField(tif, TIFFTAG_TILELENGTH, tilelength);
        }
    }
    TIFFForceStrileArrayWriting(tif);
    
    // 6. TIFFFlushData
    TIFFFlushData(tif);
    
    // Cleanup
    TIFFClose(tif);
    
    return 0;
}