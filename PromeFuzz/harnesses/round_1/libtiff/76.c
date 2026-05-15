// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static int setup_basic_tiff(TIFF* tif) {
    // Set some basic required fields to make the TIFF valid
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
    
    return 1;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a dummy file with fuzzer data
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to file
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Open TIFF file in write mode
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        // Try to open in read mode if write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Set up basic TIFF structure if we opened in write mode
    if (TIFFGetMode(tif) == O_RDWR) {
        setup_basic_tiff(tif);
    }
    
    // Use fuzzer data to decide which functions to call and in what order
    // Use first few bytes as control bytes
    size_t control_bytes = Size > 10 ? 10 : Size;
    
    for (size_t i = 0; i < control_bytes; i++) {
        uint8_t op = Data[i] % 6;
        
        switch (op) {
            case 0:
                if (TIFFGetMode(tif) == O_RDWR) {
                    TIFFWriteDirectory(tif);
                    // After writing, set up basic structure for next directory
                    setup_basic_tiff(tif);
                }
                break;
            case 1:
                if (TIFFGetMode(tif) == O_RDWR) {
                    TIFFRewriteDirectory(tif);
                }
                break;
            case 2:
                if (TIFFGetMode(tif) == O_RDWR) {
                    TIFFCheckpointDirectory(tif);
                }
                break;
            case 3:
                if (TIFFGetMode(tif) == O_RDWR) {
                    TIFFForceStrileArrayWriting(tif);
                }
                break;
            case 4:
                if (TIFFGetMode(tif) == O_RDWR) {
                    TIFFDeferStrileArrayWriting(tif);
                }
                break;
            case 5:
                if (TIFFGetMode(tif) == O_RDWR) {
                    TIFFSetupStrips(tif);
                }
                break;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}