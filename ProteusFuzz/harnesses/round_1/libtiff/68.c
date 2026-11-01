// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    if (Size > 0) {
        write(fd, Data, Size);
    } else {
        // Write a minimal valid TIFF header if input is empty
        uint16_t header[] = {42, 0, 0, 8, 0, 0, 0, 0}; 
        write(fd, header, sizeof(header));
    }
    close(fd);
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

static TIFFYCbCrToRGB *init_ycbcr_buffer(void) {
    // Manually calculate size to avoid using internal TIFFroundup_32
    // sizeof(TIFFYCbCrToRGB) is typically 16 or 24 bytes depending on arch
    // We need to align it to sizeof(long)
    size_t base_size = sizeof(TIFFYCbCrToRGB);
    size_t align = sizeof(long);
    size_t aligned_base = (base_size + align - 1) & ~(align - 1);
    
    size_t size = aligned_base +
                  4 * 256 * sizeof(TIFFRGBValue) +
                  2 * 256 * sizeof(int) +
                  3 * 256 * sizeof(int32_t);
    
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)malloc(size);
    if (!ycbcr) return NULL;
    
    memset(ycbcr, 0, size);
    return ycbcr;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Prepare environment
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "rb");
    if (!tif) {
        cleanup_file();
        return 0;
    }

    // 1. Fuzz TIFFReadRGBAImage
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) && TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        if (width > 0 && height > 0 && width < 4096 && height < 4096) {
            uint32_t *rgba_buf = (uint32_t *)malloc(width * height * sizeof(uint32_t));
            if (rgba_buf) {
                TIFFReadRGBAImage(tif, width, height, rgba_buf, 0);
                free(rgba_buf);
            }
        }
    }

    // 2. Fuzz TIFFReadRGBATileExt
    uint32_t tile_width = 0, tile_length = 0;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) && TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length)) {
        if (tile_width > 0 && tile_length > 0 && tile_width < 4096 && tile_length < 4096) {
            uint32_t *tile_buf = (uint32_t *)malloc(tile_width * tile_length * sizeof(uint32_t));
            if (tile_buf) {
                // Try reading a tile at (0,0)
                TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 0);
                free(tile_buf);
            }
        }
    }

    // 3. Fuzz TIFFYCbCrtoRGB
    TIFFYCbCrToRGB *ycbcr = init_ycbcr_buffer();
    if (ycbcr) {
        uint32_t r, g, b;
        // Use input data to generate Y, Cb, Cr values
        int32_t y = (int32_t)Data[0];
        int32_t cb = (int32_t)Data[1];
        int32_t cr = (int32_t)Data[2];
        
        TIFFYCbCrtoRGB(ycbcr, y, cb, cr, &r, &g, &b);
        free(ycbcr);
    }

    // 4. Fuzz TIFFReadRGBAStrip
    // TIFFTAG_STRIPCOUNT is not a standard tag, we use TIFFTAG_ROWSPERSTRIP and calculate or just try strip 0
    uint32_t rows_per_strip = 0;
    if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
        if (rows_per_strip > 0 && width > 0) {
            uint32_t *strip_buf = (uint32_t *)malloc(width * rows_per_strip * sizeof(uint32_t));
            if (strip_buf) {
                // Read the first strip (index 0)
                TIFFReadRGBAStrip(tif, 0, strip_buf);
                free(strip_buf);
            }
        }
    }

    // 5. Fuzz TIFFReadRGBATile
    if (tile_width > 0 && tile_length > 0) {
        uint32_t *tile_buf2 = (uint32_t *)malloc(tile_width * tile_length * sizeof(uint32_t));
        if (tile_buf2) {
            TIFFReadRGBATile(tif, 0, 0, tile_buf2);
            free(tile_buf2);
        }
    }

    // 6. Fuzz TIFFRGBAImageBegin
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    char err[1024];
    if (TIFFRGBAImageBegin(&img, tif, 0, err)) {
        // If begin succeeded, try to get a row using TIFFRGBAImageGet
        if (img.width > 0 && img.height > 0) {
            uint32_t *row_buf = (uint32_t *)malloc(img.width * sizeof(uint32_t));
            if (row_buf) {
                // TIFFRGBAImageGet signature: (TIFFRGBAImage *, uint32_t *, uint32_t, uint32_t)
                // Arguments: image, buffer, row_index, count (usually 1 for a single row)
                TIFFRGBAImageGet(&img, row_buf, 0, 1);
                free(row_buf);
            }
        }
        
        // Cleanup RGBAImage resources
        TIFFRGBAImageEnd(&img);
    }

    // Cleanup
    TIFFClose(tif);
    cleanup_file();

    return 0;
}