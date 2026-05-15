// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
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

static void dummy_cleanup(void) {
    unlink("./dummy_file");
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        dummy_cleanup();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint16_t samplesperpixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    uint16_t bitspersample = 8;
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    
    uint32_t tile_width = 0, tile_height = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    
    uint32_t rowsperstrip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    uint32_t tile_count = TIFFNumberOfTiles(tif);
    
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    tmsize_t strip_size = TIFFStripSize(tif);
    tmsize_t tile_size = TIFFTileSize(tif);
    
    uint32_t strip_index = 0;
    uint32_t tile_index = 0;
    
    if (strip_count > 0) {
        strip_index = Data[0] % strip_count;
    }
    
    if (tile_count > 0) {
        tile_index = Data[1] % tile_count;
    }
    
    void* buffer = NULL;
    tmsize_t buf_size = 0;
    
    if (Size > 200) {
        buf_size = (Data[2] % 1024) + 1;
        buffer = malloc(buf_size);
        if (buffer) {
            size_t copy_size = (Size - 100) > buf_size ? buf_size : (Size - 100);
            memcpy(buffer, Data + 100, copy_size);
        }
    }
    
    int setup_result = 0;
    tmsize_t read_result = 0;
    tmsize_t write_result = 0;
    
    if (buffer && buf_size > 0) {
        setup_result = TIFFWriteBufferSetup(tif, buffer, buf_size);
        
        setup_result = TIFFReadBufferSetup(tif, buffer, buf_size);
    } else {
        setup_result = TIFFWriteBufferSetup(tif, NULL, -1);
        
        setup_result = TIFFReadBufferSetup(tif, NULL, -1);
    }
    
    if (strip_count > 0 && strip_size > 0) {
        void* strip_buffer = malloc(strip_size);
        if (strip_buffer) {
            if (Size > 300) {
                size_t copy_size = (Size - 300) > strip_size ? strip_size : (Size - 300);
                memcpy(strip_buffer, Data + 300, copy_size);
            }
            
            write_result = TIFFWriteRawStrip(tif, strip_index, strip_buffer, 
                                           strip_size);
            
            write_result = TIFFWriteEncodedStrip(tif, strip_index, strip_buffer,
                                               strip_size);
            
            free(strip_buffer);
        }
    }
    
    if (tile_count > 0 && tile_size > 0) {
        void* tile_buffer = malloc(tile_size);
        if (tile_buffer) {
            if (Size > 400) {
                size_t copy_size = (Size - 400) > tile_size ? tile_size : (Size - 400);
                memcpy(tile_buffer, Data + 400, copy_size);
            }
            
            write_result = TIFFWriteEncodedTile(tif, tile_index, tile_buffer,
                                              tile_size);
            
            read_result = TIFFReadEncodedTile(tif, tile_index, tile_buffer,
                                            tile_size);
            
            free(tile_buffer);
        }
    }
    
    if (buffer) {
        free(buffer);
    }
    
    TIFFClose(tif);
    dummy_cleanup();
    
    return 0;
}