// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        uint16_t compression = 0;
        TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
        
        uint32_t rows_per_strip = 0;
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        uint32_t tile_count = TIFFNumberOfTiles(tif);
        
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        tmsize_t tile_size = TIFFTileSize(tif);
        tmsize_t strip_size = TIFFStripSize(tif);
        
        if (tile_count > 0 && tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                uint32_t tile_idx = 0;
                if (tile_count > 1 && Size > sizeof(uint32_t)) {
                    memcpy(&tile_idx, Data, sizeof(uint32_t));
                    tile_idx %= tile_count;
                }
                
                TIFFReadEncodedTile(tif, tile_idx, buf, tile_size);
                
                free(buf);
            }
        }
        
        if (!compression) {
            tmsize_t read_buf_size = scanline_size * 16;
            if (read_buf_size > 0) {
                void* read_buf = malloc(read_buf_size);
                if (read_buf) {
                    TIFFReadBufferSetup(tif, read_buf, read_buf_size);
                    free(read_buf);
                }
            }
        }
        
        TIFFClose(tif);
    }
    
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        uint32_t width = 256;
        uint32_t height = 256;
        uint16_t samples_per_pixel = 1;
        uint16_t bits_per_sample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planar_config = PLANARCONFIG_CONTIG;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
        
        size_t data_offset = 0;
        
        if (Size > sizeof(uint32_t)) {
            uint32_t tile_dim = 64;
            TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_dim);
            TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_dim);
            
            uint32_t tile_count = ((width + tile_dim - 1) / tile_dim) * 
                                  ((height + tile_dim - 1) / tile_dim);
            
            tmsize_t tile_buf_size = tile_dim * tile_dim;
            void* tile_buf = malloc(tile_buf_size);
            if (tile_buf) {
                size_t copy_size = tile_buf_size;
                if (Size - data_offset < copy_size) {
                    copy_size = Size - data_offset;
                }
                if (copy_size > 0) {
                    memcpy(tile_buf, Data + data_offset, copy_size);
                    data_offset += copy_size;
                }
                
                uint32_t tile_idx = 0;
                if (tile_count > 1 && Size - data_offset >= sizeof(uint32_t)) {
                    memcpy(&tile_idx, Data + data_offset, sizeof(uint32_t));
                    tile_idx %= tile_count;
                    data_offset += sizeof(uint32_t);
                }
                
                TIFFWriteEncodedTile(tif, tile_idx, tile_buf, tile_buf_size);
                
                tmsize_t write_buf_size = tile_buf_size * 2;
                void* write_buf = malloc(write_buf_size);
                if (write_buf) {
                    TIFFWriteBufferSetup(tif, write_buf, write_buf_size);
                    
                    if (Size - data_offset >= sizeof(uint32_t)) {
                        uint32_t write_tile_idx = 0;
                        memcpy(&write_tile_idx, Data + data_offset, sizeof(uint32_t));
                        write_tile_idx %= tile_count;
                        data_offset += sizeof(uint32_t);
                        
                        TIFFWriteEncodedTile(tif, write_tile_idx, tile_buf, tile_buf_size);
                    }
                    
                    free(write_buf);
                }
                
                free(tile_buf);
            }
        }
        
        if (Size - data_offset > sizeof(uint32_t)) {
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
            
            uint32_t strip_count = TIFFNumberOfStrips(tif);
            tmsize_t strip_buf_size = TIFFStripSize(tif);
            
            if (strip_buf_size > 0 && strip_count > 0) {
                void* strip_buf = malloc(strip_buf_size);
                if (strip_buf) {
                    size_t copy_size = strip_buf_size;
                    if (Size - data_offset < copy_size) {
                        copy_size = Size - data_offset;
                    }
                    if (copy_size > 0) {
                        memcpy(strip_buf, Data + data_offset, copy_size);
                        data_offset += copy_size;
                    }
                    
                    uint32_t strip_idx = 0;
                    if (strip_count > 1 && Size - data_offset >= sizeof(uint32_t)) {
                        memcpy(&strip_idx, Data + data_offset, sizeof(uint32_t));
                        strip_idx %= strip_count;
                        data_offset += sizeof(uint32_t);
                    }
                    
                    TIFFWriteEncodedStrip(tif, strip_idx, strip_buf, strip_buf_size);
                    
                    if (Size - data_offset >= sizeof(uint32_t)) {
                        uint32_t raw_strip_idx = 0;
                        memcpy(&raw_strip_idx, Data + data_offset, sizeof(uint32_t));
                        raw_strip_idx %= strip_count;
                        data_offset += sizeof(uint32_t);
                        
                        TIFFWriteRawStrip(tif, raw_strip_idx, strip_buf, strip_buf_size);
                    }
                    
                    free(strip_buf);
                }
            }
        }
        
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}