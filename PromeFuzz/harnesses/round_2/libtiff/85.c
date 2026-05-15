// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    uint32_t tile_count = TIFFIsTiled(tif) ? TIFFNumberOfTiles(tif) : 0;
    
    for (uint32_t i = 0; i < 10; i++) {
        uint32_t strile = i;
        
        if (strip_count > 0) {
            strile = i % strip_count;
            
            int pbErr = 0;
            uint64_t offset1 = TIFFGetStrileOffsetWithErr(tif, strile, &pbErr);
            
            uint64_t bytecount1 = TIFFGetStrileByteCount(tif, strile);
            
            pbErr = 0;
            uint64_t bytecount2 = TIFFGetStrileByteCountWithErr(tif, strile, &pbErr);
            
            uint32_t rows = (i % 10) + 1;
            uint64_t strip_size = TIFFVStripSize64(tif, rows);
            
            uint64_t raw_size = TIFFRawStripSize64(tif, strile);
            
            uint64_t offset2 = TIFFGetStrileOffset(tif, strile);
        }
        
        if (tile_count > 0) {
            uint32_t tile = i % tile_count;
            
            int pbErr = 0;
            uint64_t offset1 = TIFFGetStrileOffsetWithErr(tif, tile, &pbErr);
            
            uint64_t bytecount1 = TIFFGetStrileByteCount(tif, tile);
            
            pbErr = 0;
            uint64_t bytecount2 = TIFFGetStrileByteCountWithErr(tif, tile, &pbErr);
            
            uint64_t offset2 = TIFFGetStrileOffset(tif, tile);
        }
    }
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}