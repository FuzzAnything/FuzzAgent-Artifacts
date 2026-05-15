// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff errors during fuzzing
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    // Silence libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer data to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        // Try opening for reading if write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Set some basic tags to create a valid directory
    if (TIFFGetMode(tif) == 'w') {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        
        // Use fuzzer data to decide which operations to perform
        size_t op_index = 0;
        if (Size > 0) {
            // Try different operations based on input data
            uint8_t op_selector = Data[op_index++ % Size];
            
            // Call target functions in different orders based on fuzzer input
            switch (op_selector % 6) {
                case 0:
                    TIFFDeferStrileArrayWriting(tif);
                    TIFFWriteDirectory(tif);
                    TIFFForceStrileArrayWriting(tif);
                    break;
                case 1:
                    TIFFWriteDirectory(tif);
                    TIFFRewriteDirectory(tif);
                    break;
                case 2:
                    TIFFSetupStrips(tif);
                    TIFFCheckpointDirectory(tif);
                    break;
                case 3:
                    TIFFDeferStrileArrayWriting(tif);
                    TIFFSetupStrips(tif);
                    TIFFWriteDirectory(tif);
                    break;
                case 4:
                    TIFFWriteDirectory(tif);
                    TIFFCheckpointDirectory(tif);
                    TIFFRewriteDirectory(tif);
                    break;
                case 5:
                    TIFFSetupStrips(tif);
                    TIFFDeferStrileArrayWriting(tif);
                    TIFFForceStrileArrayWriting(tif);
                    break;
            }
            
            // Try individual functions with different parameter variations
            if (Size > 1) {
                uint8_t func_selector = Data[1];
                
                // Set tile mode based on fuzzer input
                if (func_selector & 0x01) {
                    TIFFSetField(tif, TIFFTAG_TILEWIDTH, 16);
                    TIFFSetField(tif, TIFFTAG_TILELENGTH, 16);
                }
                
                // Try each function individually
                if ((func_selector >> 1) & 0x01) TIFFWriteDirectory(tif);
                if ((func_selector >> 2) & 0x01) TIFFRewriteDirectory(tif);
                if ((func_selector >> 3) & 0x01) TIFFCheckpointDirectory(tif);
                if ((func_selector >> 4) & 0x01) TIFFForceStrileArrayWriting(tif);
                if ((func_selector >> 5) & 0x01) TIFFDeferStrileArrayWriting(tif);
                if ((func_selector >> 6) & 0x01) TIFFSetupStrips(tif);
            }
        }
    } else {
        // For read mode, try to call functions that might work
        // Some functions require write mode but we try anyway
        TIFFSetupStrips(tif);
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}