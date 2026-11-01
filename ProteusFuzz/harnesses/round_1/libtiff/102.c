// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to a dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Attempt to open the file for reading
    TIFF *tif_read = TIFFOpen(filename, "rb");
    if (tif_read) {
        // Fuzz TIFFGetStrileOffset
        uint32_t strile_idx = 0;
        if (Size > 0) strile_idx = Data[0] % 1000;
        uint64_t offset = TIFFGetStrileOffset(tif_read, strile_idx);
        (void)offset;

        // Fuzz TIFFGetStrileOffsetWithErr
        int pbErr = 0;
        offset = TIFFGetStrileOffsetWithErr(tif_read, strile_idx, &pbErr);
        (void)offset;

        // Fuzz TIFFReadEncodedTile
        // Allocate a buffer for reading
        tmsize_t tile_size = TIFFTileSize(tif_read);
        if (tile_size > 0 && tile_size < (1024 * 1024)) {
            uint8_t *tile_buf = (uint8_t *)malloc(tile_size);
            if (tile_buf) {
                tmsize_t result = TIFFReadEncodedTile(tif_read, strile_idx, tile_buf, tile_size);
                (void)result;
                free(tile_buf);
            }
        }
        
        // Try reading with -1 size as per docs for auto-sizing if supported
        if (tile_size > 0 && tile_size < (1024 * 1024)) {
             uint8_t *tile_buf2 = (uint8_t *)malloc(tile_size);
             if (tile_buf2) {
                 tmsize_t result2 = TIFFReadEncodedTile(tif_read, strile_idx, tile_buf2, -1);
                 (void)result2;
                 free(tile_buf2);
             }
        }

        cleanup_tiff(tif_read);
    }

    // Attempt to open the file for writing (may fail if file is read-only or corrupt)
    TIFF *tif_write = TIFFOpen(filename, "wb");
    if (tif_write) {
        // Set some basic tags to make the file valid for writing
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, 10);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, 10);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        // Fuzz TIFFWriteEncodedStrip
        uint32_t strip_idx = 0;
        if (Size > 1) strip_idx = Data[1] % 100;
        tmsize_t data_len = (Size > 2) ? (Data[2] % (Size - 2)) : 0;
        if (data_len > 0 && data_len < Size) {
            tmsize_t result = TIFFWriteEncodedStrip(tif_write, strip_idx, (void *)(Data + 2), data_len);
            (void)result;
        }

        // Fuzz TIFFWriteEncodedTile
        uint32_t tile_idx = 0;
        if (Size > 3) tile_idx = Data[3] % 100;
        tmsize_t tile_data_len = (Size > 4) ? (Data[4] % (Size - 4)) : 0;
        if (tile_data_len > 0 && tile_data_len < Size) {
            tmsize_t result = TIFFWriteEncodedTile(tif_write, tile_idx, (void *)(Data + 4), tile_data_len);
            (void)result;
        }

        // Fuzz TIFFWriteTile
        uint32_t x = (Size > 5) ? Data[5] % 10 : 0;
        uint32_t y = (Size > 6) ? Data[6] % 10 : 0;
        uint32_t z = (Size > 7) ? Data[7] % 10 : 0;
        uint16_t s = (Size > 8) ? Data[8] % 10 : 0;
        
        // Prepare a small buffer for tile writing
        uint8_t tile_write_buf[256];
        memset(tile_write_buf, 0, sizeof(tile_write_buf));
        tmsize_t tile_result = TIFFWriteTile(tif_write, tile_write_buf, x, y, z, s);
        (void)tile_result;

        cleanup_tiff(tif_write);
    }

    return 0;
}