// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress error messages during fuzzing
}

static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Allocate and configure TIFFOpenOptions
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Use input data to configure options in various ways
    if (Size > 0) {
        // Use first byte to determine configuration strategy
        uint8_t config_byte = Data[0];
        
        // Set max single memory allocation (use bytes 1-4 if available)
        if (Size >= 5) {
            tmsize_t max_single = 0;
            // Combine bytes 1-4 into a tmsize_t value
            for (int i = 0; i < 4 && (i + 1) < Size; i++) {
                max_single = (max_single << 8) | Data[i + 1];
            }
            TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);
        }
        
        // Set max cumulated memory allocation (use bytes 5-8 if available)
        if (Size >= 9) {
            tmsize_t max_cumulated = 0;
            // Combine bytes 5-8 into a tmsize_t value
            for (int i = 0; i < 4 && (i + 5) < Size; i++) {
                max_cumulated = (max_cumulated << 8) | Data[i + 5];
            }
            TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);
        }
        
        // Set warning about unknown tags (use byte 9 if available)
        if (Size >= 10) {
            int warn_unknown = Data[9] & 1; // Use LSB only
            TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_unknown);
        }
        
        // Try different combinations based on config_byte
        if (config_byte & 0x01) {
            // Try with very small memory limits
            TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 1024);
            TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 4096);
        }
        
        if (config_byte & 0x02) {
            // Try with moderate memory limits
            TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 65536);
            TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 262144);
        }
        
        if (config_byte & 0x04) {
            // Enable unknown tag warnings
            TIFFOpenOptionsSetWarnAboutUnknownTags(opts, 1);
        } else if (config_byte & 0x08) {
            // Disable unknown tag warnings
            TIFFOpenOptionsSetWarnAboutUnknownTags(opts, 0);
        }
    }
    
    // Try to open the TIFF file with extended options
    TIFF *tif = TIFFOpenExt("./dummy_file", "r", opts);
    
    if (tif) {
        // If opened successfully, try to read some basic information
        // to exercise more code paths
        uint32 width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        // Try to read directory information
        uint16 dir_count = 0;
        TIFFGetField(tif, TIFFTAG_SUBFILETYPE, &dir_count);
        
        // Close the TIFF file
        TIFFClose(tif);
    }
    
    // Free the options
    TIFFOpenOptionsFree(opts);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}