// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

static TIFF* create_test_tiff(const uint8_t *data, size_t size) {
    write_dummy_file(data, size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        remove_dummy_file();
        return NULL;
    }
    
    // Set up minimal TIFF directory for testing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 10);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    return tif;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create test TIFF file
    TIFF *tif = create_test_tiff(Data, Size);
    if (!tif) return 0;
    
    // Test TIFFWriteScanline with various parameters
    uint32_t width = 100;
    uint8_t *buf = (uint8_t*)malloc(width);
    if (buf) {
        // Fill buffer with fuzzer data (cyclic)
        for (uint32_t i = 0; i < width; i++) {
            buf[i] = Data[i % Size];
        }
        
        // Try writing scanlines with different rows and samples
        for (uint32_t row = 0; row < 10; row++) {
            for (uint16_t sample = 0; sample < 2; sample++) {
                TIFFWriteScanline(tif, buf, row, sample);
            }
        }
        free(buf);
    }
    
    // Test TIFFFlushData
    TIFFFlushData(tif);
    
    // Test TIFFFlush
    TIFFFlush(tif);
    
    // Test TIFFGetCloseProc
    TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
    if (close_proc) {
        // We don't actually call it, just get the pointer
    }
    
    // Create a second TIFF handle to test TIFFCleanup separately
    // This avoids the use-after-free when calling TIFFClose after TIFFCleanup
    TIFF *tif2 = create_test_tiff(Data, Size);
    if (tif2) {
        // Test TIFFCleanup on the second handle
        TIFFCleanup(tif2);
        // Don't call TIFFClose after TIFFCleanup
    }
    
    // Test TIFFClose on the first handle (without TIFFCleanup)
    TIFFClose(tif);
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}