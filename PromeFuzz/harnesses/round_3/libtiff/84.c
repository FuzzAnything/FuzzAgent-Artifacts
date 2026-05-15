// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint16_t samplesperpixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    uint16_t bitspersample = 1;
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    
    uint32_t rowsperstrip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    if (rowsperstrip == 0) rowsperstrip = height;
    
    uint32_t strile = Data[0] % 100;
    int pbErr = 0;
    uint64_t byte_count = TIFFGetStrileByteCountWithErr(tif, strile, &pbErr);
    
    uint32_t strip = Data[1] % 100;
    tmsize_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0) {
        void *buf = _TIFFmalloc(strip_size);
        if (buf) {
            TIFFReadEncodedStrip(tif, strip, buf, strip_size);
            _TIFFfree(buf);
        }
    }
    
    uint32_t x = Data[2] % (width > 0 ? width : 100);
    uint32_t y = Data[3] % (height > 0 ? height : 100);
    uint32_t z = Data[4] % 10;
    uint16_t s = Data[5] % (samplesperpixel > 0 ? samplesperpixel : 1);
    uint32_t tile = TIFFComputeTile(tif, x, y, z, s);
    
    uint32_t current_strip = TIFFCurrentStrip(tif);
    
    uint32_t row = Data[6] % (height > 0 ? height : 100);
    uint16_t sample = Data[7] % (samplesperpixel > 0 ? samplesperpixel : 1);
    uint32_t computed_strip = TIFFComputeStrip(tif, row, sample);
    
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}