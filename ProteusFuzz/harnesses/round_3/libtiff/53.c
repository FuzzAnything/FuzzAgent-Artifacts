// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void setup_dummy_tiff_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    
    // Write a minimal valid TIFF header (little-endian)
    // Magic number (II), Byte order, Offset to first IFD
    uint8_t header[8];
    header[0] = 'I';
    header[1] = 'I';
    header[2] = 0x2A;
    header[3] = 0x00;
    header[4] = 0x08; // Offset to first IFD (8 bytes)
    header[5] = 0x00;
    header[6] = 0x00;
    header[7] = 0x00;
    
    write(fd, header, 8);
    
    // Write a minimal IFD (0 entries, next IFD offset 0)
    uint16_t ifd_entries = 0;
    uint32_t next_ifd = 0;
    write(fd, &ifd_entries, sizeof(ifd_entries));
    write(fd, &next_ifd, sizeof(next_ifd));
    
    // Append the fuzzing data as raw image data
    if (size > 0) {
        write(fd, data, size);
    }
    
    close(fd);
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create a dummy file with the input data
    setup_dummy_tiff_file(filename, Data, Size);

    // Try opening for reading
    TIFF *tif_read = TIFFOpen(filename, "r");
    if (tif_read) {
        uint32_t tile_idx = 0;
        uint32_t nrows = 0;
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        
        // Extract some values from input to drive fuzzing logic
        if (Size >= 4) {
            tile_idx = *(const uint32_t *)(Data);
            nrows = *(const uint32_t *)(Data + 4);
            if (Size >= 12) {
                x = *(const uint32_t *)(Data + 8);
                y = *(const uint32_t *)(Data + 12);
                z = *(const uint32_t *)(Data + 16);
                if (Size >= 20) {
                    s = *(const uint16_t *)(Data + 20);
                }
            }
        }

        // Fuzz TIFFTileSize
        tmsize_t tile_size = TIFFTileSize(tif_read);
        if (tile_size > 0 && tile_size < (1024 * 1024)) {
            void *buf = malloc(tile_size);
            if (buf) {
                // Fuzz TIFFReadRawTile
                TIFFReadRawTile(tif_read, tile_idx, buf, tile_size);
                free(buf);
            }
        }

        // Fuzz TIFFVTileSize
        tmsize_t vtile_size = TIFFVTileSize(tif_read, nrows);
        if (vtile_size > 0 && vtile_size < (1024 * 1024)) {
            void *vbuf = malloc(vtile_size);
            if (vbuf) {
                // Attempt to read raw tile with vtile_size
                TIFFReadRawTile(tif_read, tile_idx, vbuf, vtile_size);
                free(vbuf);
            }
        }

        // Fuzz TIFFTileRowSize
        tmsize_t row_size = TIFFTileRowSize(tif_read);
        if (row_size > 0 && row_size < (1024 * 1024)) {
            void *rowbuf = malloc(row_size);
            if (rowbuf) {
                // Attempt to read raw tile with row_size
                TIFFReadRawTile(tif_read, tile_idx, rowbuf, row_size);
                free(rowbuf);
            }
        }

        TIFFClose(tif_read);
    }

    // Try opening for writing
    TIFF *tif_write = TIFFOpen(filename, "w");
    if (tif_write) {
        uint32_t tile_idx = 0;
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        
        if (Size >= 4) {
            tile_idx = *(const uint32_t *)(Data);
            x = *(const uint32_t *)(Data + 4);
            y = *(const uint32_t *)(Data + 8);
            z = *(const uint32_t *)(Data + 12);
            if (Size >= 16) {
                s = *(const uint16_t *)(Data + 16);
            }
        }

        // Allocate a buffer for writing
        size_t write_size = Size < 1024 ? Size : 1024;
        void *write_buf = malloc(write_size);
        if (write_buf) {
            memcpy(write_buf, Data, write_size);
            
            // Fuzz TIFFWriteTile
            TIFFWriteTile(tif_write, write_buf, x, y, z, s);
            
            // Fuzz TIFFWriteRawTile
            TIFFWriteRawTile(tif_write, tile_idx, write_buf, write_size);
            
            free(write_buf);
        }

        TIFFClose(tif_write);
    }

    // Cleanup
    unlink(filename);

    return 0;
}