// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    // First phase: Create and write a TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    // Set up basic image parameters using fuzzer data
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    uint16_t bitspersample = Data[4] % 16 + 1;
    uint16_t samplesperpixel = (Data[5] % 4) + 1;
    uint16_t photometric = Data[6] % 6;
    uint16_t planarconfig = Data[7] % 2;
    uint16_t compression = Data[8] % 7;
    
    // Call TIFFSetField 8 times as specified
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height > 0 ? height : 1);
    
    // Defer strile array writing
    TIFFDeferStrileArrayWriting(tif);
    
    // Write check
    TIFFWriteCheck(tif, 0, "test");
    
    // Write directory
    TIFFWriteDirectory(tif);
    
    // Set directory and force strile writing
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    
    // Set directory again and force strile writing
    TIFFSetDirectory(tif, 0);
    TIFFForceStrileArrayWriting(tif);
    
    // Set directory again for writing
    TIFFSetDirectory(tif, 0);
    
    // Prepare tile data
    uint32_t tile_size = 256;
    uint8_t *tile_data = (uint8_t *)malloc(tile_size);
    if (tile_data) {
        size_t copy_size = Size > tile_size ? tile_size : Size;
        memcpy(tile_data, Data, copy_size);
        
        // Write encoded tile
        TIFFWriteEncodedTile(tif, 0, tile_data, copy_size);
        
        free(tile_data);
    }
    
    // Prepare strip data
    uint32_t strip_size = 512;
    uint8_t *strip_data = (uint8_t *)malloc(strip_size);
    if (strip_data) {
        size_t copy_size = Size > strip_size ? strip_size : Size;
        memcpy(strip_data, Data, copy_size);
        
        // Write encoded strip
        TIFFWriteEncodedStrip(tif, 0, strip_data, copy_size);
        
        free(strip_data);
    }
    
    // Close and reopen for reading
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    // Read encoded tile
    uint8_t *read_tile_buf = (uint8_t *)malloc(1024);
    if (read_tile_buf) {
        TIFFReadEncodedTile(tif, 0, read_tile_buf, 1024);
        free(read_tile_buf);
    }
    
    // Close and reopen again
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    // Read encoded strip
    uint8_t *read_strip_buf = (uint8_t *)malloc(1024);
    if (read_strip_buf) {
        TIFFReadEncodedStrip(tif, 0, read_strip_buf, 1024);
        free(read_strip_buf);
    }
    
    // Final close operations
    TIFFClose(tif);
    TIFFClose(NULL);  // Additional close as specified in sequence
    
    cleanup_file();
    return 0;
}