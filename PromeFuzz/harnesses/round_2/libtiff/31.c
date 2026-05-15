// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

static TIFF* open_tiff_with_data(const uint8_t *data, size_t size, const char* mode) {
    if (write_dummy_file(data, size) != 0) {
        return NULL;
    }
    
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    return TIFFOpen("./dummy_file", mode);
}

static void setup_basic_tiff_tags(TIFF* tif) {
    if (!tif) return;
    
    // Set minimal required tags for a valid TIFF
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (uint32_t)1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine mode and operation sequence
    uint8_t mode_selector = Data[0];
    const char* mode = (mode_selector & 1) ? "w" : "a";
    
    TIFF* tif = open_tiff_with_data(Data, Size, mode);
    if (!tif) return 0;
    
    // Setup basic tags for write modes
    if (strchr(mode, 'w') || strchr(mode, 'a')) {
        setup_basic_tiff_tags(tif);
    }
    
    // Use remaining bytes to determine which operations to perform
    size_t op_index = 1;
    while (op_index < Size && op_index < 32) {
        uint8_t op = Data[op_index] % 6;
        
        switch (op) {
            case 0:
                TIFFRewriteDirectory(tif);
                break;
            case 1:
                TIFFDeferStrileArrayWriting(tif);
                break;
            case 2:
                TIFFForceStrileArrayWriting(tif);
                break;
            case 3:
                TIFFFlush(tif);
                break;
            case 4:
                TIFFCheckpointDirectory(tif);
                break;
            case 5:
                TIFFSetupStrips(tif);
                break;
        }
        
        // Try to write some dummy data to trigger strip setup
        if (strchr(mode, 'w') || strchr(mode, 'a')) {
            unsigned char dummy_pixel = 0;
            if (TIFFWriteEncodedStrip(tif, 0, &dummy_pixel, 1) == 1) {
                // Data written successfully
            }
        }
        
        op_index++;
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}