// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

static TIFF* create_dummy_tiff(const char* mode) {
    TIFF* tif = TIFFOpen("./dummy_file", mode);
    if (!tif) {
        // If opening fails, try to create the file first
        int fd = open("./dummy_file", O_CREAT | O_RDWR, 0644);
        if (fd >= 0) {
            close(fd);
            tif = TIFFOpen("./dummy_file", mode);
        }
    }
    return tif;
}

static void setup_basic_tiff_fields(TIFF* tif) {
    if (!tif) return;
    
    // Set minimal required fields for a valid TIFF
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
    
    // Set tiling parameters if needed
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, 32);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, 32);
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);
    
    // Write data to dummy file
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    // Test TIFFWriteBufferSetup
    TIFF* tif_write = create_dummy_tiff("w");
    if (tif_write) {
        setup_basic_tiff_fields(tif_write);
        
        // Test with NULL buffer (let libtiff allocate)
        TIFFWriteBufferSetup(tif_write, NULL, -1);
        
        // Test with custom buffer
        void* buffer = malloc(4096);
        if (buffer) {
            TIFFWriteBufferSetup(tif_write, buffer, 4096);
            free(buffer);
        }
        
        // Test TIFFWriteRawStrip
        if (Size > 0) {
            uint32_t strip = 0;
            tmsize_t cc = Size > 1024 ? 1024 : Size;
            TIFFWriteRawStrip(tif_write, strip, (void*)Data, cc);
        }
        
        // Test TIFFWriteEncodedStrip
        if (Size > 0) {
            uint32_t strip = 0;
            tmsize_t cc = Size > 1024 ? 1024 : Size;
            TIFFWriteEncodedStrip(tif_write, strip, (void*)Data, cc);
        }
        
        // Test TIFFWriteEncodedTile
        if (Size > 0) {
            uint32_t tile = 0;
            tmsize_t cc = Size > 1024 ? 1024 : Size;
            TIFFWriteEncodedTile(tif_write, tile, (void*)Data, cc);
        }
        
        TIFFClose(tif_write);
    }
    
    // Test read functions
    TIFF* tif_read = create_dummy_tiff("r");
    if (tif_read) {
        // Test TIFFReadBufferSetup
        void* read_buffer = malloc(4096);
        if (read_buffer) {
            TIFFReadBufferSetup(tif_read, read_buffer, 4096);
            free(read_buffer);
        }
        
        // Test TIFFReadEncodedTile
        if (Size > 0) {
            uint32_t tile = 0;
            void* buf = malloc(1024);
            if (buf) {
                tmsize_t size = Size > 1024 ? 1024 : Size;
                TIFFReadEncodedTile(tif_read, tile, buf, size);
                free(buf);
            }
        }
        
        TIFFClose(tif_read);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}