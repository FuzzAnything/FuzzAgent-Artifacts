// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create dummy file for TIFF operations
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to file
    if (Data != NULL && Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Allocate TIFF options
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        unlink(filename);
        return 0;
    }
    
    // Set memory allocation limit
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 1024 * 1024); // 1MB limit
    
    // Open TIFF file with options
    TIFF* tif = TIFFOpenExt(filename, "w", opts);
    
    // Free options
    TIFFOpenOptionsFree(opts);
    
    if (tif != NULL) {
        // First TIFFClose as per order
        TIFFClose(tif);
        
        // Re-open file for writing operations
        tif = TIFFOpen(filename, "w");
        if (tif != NULL) {
            // Set some sample fields
            uint32_t width = 256;
            uint32_t height = 256;
            uint16_t samples_per_pixel = 3;
            uint16_t bits_per_sample = 8;
            uint16_t photometric = PHOTOMETRIC_RGB;
            uint16_t planar_config = PLANARCONFIG_CONTIG;
            
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
            
            // Write directory
            TIFFWriteDirectory(tif);
            
            // Final TIFFClose
            TIFFClose(tif);
        }
    }
    
    // Clean up dummy file
    unlink(filename);
    
    return 0;
}