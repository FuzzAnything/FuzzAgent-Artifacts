// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include <unistd.h>

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Get image dimensions for parameter generation
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    uint16_t samplesperpixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    // 1. Test TIFFCurrentStrip
    uint32_t current_strip = TIFFCurrentStrip(tif);
    (void)current_strip; // Use result to avoid unused variable warning
    
    // 2. Test TIFFComputeTile with various parameters
    if (width > 0 && height > 0 && samplesperpixel > 0) {
        // Use fuzzer data to generate coordinates if possible
        uint32_t x = (Size > 0) ? Data[0] % width : 0;
        uint32_t y = (Size > 1) ? Data[1] % height : 0;
        uint32_t z = 0; // Usually 0 for 2D images
        uint16_t s = (Size > 2) ? (Data[2] % samplesperpixel) : 0;
        
        uint32_t tile = TIFFComputeTile(tif, x, y, z, s);
        (void)tile;
    }
    
    // 3. Test TIFFGetStrileByteCountWithErr
    int err = 0;
    uint32_t strile = 0;
    if (Size > 3) {
        strile = Data[3];
    }
    uint64_t bytecount = TIFFGetStrileByteCountWithErr(tif, strile, &err);
    (void)bytecount;
    
    // 4. Test TIFFComputeStrip
    if (height > 0 && samplesperpixel > 0) {
        uint32_t row = (Size > 4) ? (Data[4] % height) : 0;
        uint16_t sample = (Size > 5) ? (Data[5] % samplesperpixel) : 0;
        uint32_t strip = TIFFComputeStrip(tif, row, sample);
        (void)strip;
    }
    
    // 5. Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;
    
    // 6. Test TIFFReadEncodedStrip
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    if (strip_count > 0) {
        uint32_t strip_to_read = 0;
        if (Size > 6) {
            strip_to_read = Data[6] % strip_count;
        }
        
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0) {
            void *buf = malloc(strip_size);
            if (buf) {
                tmsize_t bytes_read = TIFFReadEncodedStrip(tif, strip_to_read, buf, strip_size);
                (void)bytes_read;
                free(buf);
            }
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}