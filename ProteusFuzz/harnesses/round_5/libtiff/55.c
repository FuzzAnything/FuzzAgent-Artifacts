// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data to create a minimal header
    if (Size < 4) {
        return 0;
    }

    // Write input to a dummy file
    write_dummy_file(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Allocate buffers for operations
    uint8_t *inbuf = (uint8_t *)malloc(Size);
    uint8_t *outbuf = (uint8_t *)malloc(Size);
    uint32_t *rgbabuf = (uint32_t *)malloc(Size);
    
    if (!inbuf || !outbuf || !rgbabuf) {
        TIFFClose(tif);
        free(inbuf);
        free(outbuf);
        free(rgbabuf);
        return 0;
    }

    // Copy fuzz data to input buffer
    memcpy(inbuf, Data, Size);

    // Fuzz TIFFGetMode
    int mode = TIFFGetMode(tif);
    (void)mode; // Suppress unused variable warning

    // Fuzz TIFFCurrentStrip
    uint32_t strip = TIFFCurrentStrip(tif);
    (void)strip;

    // Fuzz TIFFReadScanline
    // Try reading a few scanlines with different parameters
    for (uint32_t row = 0; row < 10 && row < Size / 10; row++) {
        for (uint16_t sample = 0; sample < 4; sample++) {
            TIFFReadScanline(tif, outbuf, row, sample);
        }
    }

    // Fuzz TIFFReadFromUserBuffer
    // Try reading from user buffer with different strip/tile indices
    for (uint32_t strile = 0; strile < 10 && strile < Size / 10; strile++) {
        TIFFReadFromUserBuffer(tif, strile, inbuf, Size, outbuf, Size);
    }

    // Fuzz TIFFCheckTile
    for (uint32_t x = 0; x < 10; x++) {
        for (uint32_t y = 0; y < 10; y++) {
            for (uint32_t z = 0; z < 10; z++) {
                for (uint16_t s = 0; s < 4; s++) {
                    TIFFCheckTile(tif, x, y, z, s);
                }
            }
        }
    }

    // Fuzz TIFFReadRGBATile
    for (uint32_t col = 0; col < 10; col++) {
        for (uint32_t row = 0; row < 10; row++) {
            TIFFReadRGBATile(tif, col, row, rgbabuf);
        }
    }

    // Cleanup
    free(inbuf);
    free(outbuf);
    free(rgbabuf);
    TIFFClose(tif);

    return 0;
}