// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silently ignore warnings and errors during fuzzing
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Set error handlers to ignore warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // First TIFFClose (on NULL pointer - should be safe)
    TIFFClose(NULL);
    
    // Open a TIFF file for reading
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // TIFFReadFromUserBuffer
        uint32_t strile = 0;
        tmsize_t buffer_size = 1024;
        void *inbuf = malloc(buffer_size);
        void *outbuf = malloc(buffer_size);
        
        if (inbuf && outbuf) {
            // Initialize input buffer with some data
            memset(inbuf, 0xAA, buffer_size);
            TIFFReadFromUserBuffer(tif, strile, inbuf, buffer_size, outbuf, buffer_size);
        }
        
        free(inbuf);
        free(outbuf);
        
        // Second TIFFClose
        TIFFClose(tif);
    }
    
    // Re-open for next operations
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // TIFFReadEncodedStrip
        uint32_t strip = 0;
        tmsize_t read_size = 4096;
        void *read_buf = malloc(read_size);
        
        if (read_buf) {
            TIFFReadEncodedStrip(tif, strip, read_buf, read_size);
            free(read_buf);
        }
        
        // Third TIFFClose
        TIFFClose(tif);
    }
    
    // Re-open for final operations
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // TIFFGetStrileOffsetWithErr
        uint32_t strile = 0;
        int err1 = 0;
        TIFFGetStrileOffsetWithErr(tif, strile, &err1);
        
        // TIFFGetStrileByteCountWithErr
        int err2 = 0;
        TIFFGetStrileByteCountWithErr(tif, strile, &err2);
        
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}