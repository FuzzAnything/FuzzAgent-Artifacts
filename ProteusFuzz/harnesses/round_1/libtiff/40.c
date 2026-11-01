// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
#include <tiffio.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to a temporary file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Test Read Mode
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif) {
        uint32_t strip_idx = 0;
        uint32_t tile_idx = 0;
        tmsize_t size_param = 0;

        if (Size >= 4) {
            strip_idx = *(const uint32_t *)(Data);
            tile_idx = *(const uint32_t *)(Data);
            size_param = *(const uint32_t *)(Data + 4);
        }

        // Limit buffer size to avoid excessive allocation
        if (size_param > 1024 * 1024) {
            size_param = 1024 * 1024;
        }

        void *buf = malloc(size_param);
        if (buf) {
            // Fuzz TIFFStripSize
            tmsize_t strip_size = TIFFStripSize(tif);
            (void)strip_size;

            // Fuzz TIFFRawStripSize
            tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_idx);
            (void)raw_strip_size;

            // Fuzz TIFFReadRawStrip
            tmsize_t read_raw = TIFFReadRawStrip(tif, strip_idx, buf, size_param);
            (void)read_raw;

            // Fuzz TIFFReadEncodedStrip
            tmsize_t read_encoded = TIFFReadEncodedStrip(tif, strip_idx, buf, size_param);
            (void)read_encoded;

            // Fuzz TIFFReadRawTile
            tmsize_t read_tile = TIFFReadRawTile(tif, tile_idx, buf, size_param);
            (void)read_tile;

            free(buf);
        }
        TIFFClose(tif);
    }

    // Test Write Mode
    // Prepare a minimal valid TIFF header for writing
    fp = fopen(filename, "wb");
    if (fp) {
        // Minimal TIFF header (Little Endian)
        // II (49 49), Version (2a 00), Offset to first IFD (08 00 00 00)
        uint8_t header[] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, fp);
        fclose(fp);

        tif = TIFFOpen(filename, "w");
        if (tif) {
            // Set basic tags
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);

            tmsize_t size_param = 0;
            if (Size >= 8) {
                size_param = *(const uint32_t *)(Data + 4);
            }
            if (size_param > 1024 * 1024) {
                size_param = 1024 * 1024;
            }

            void *buf = malloc(size_param);
            if (buf) {
                memset(buf, 0, size_param);
                // Fuzz TIFFWriteRawStrip
                tmsize_t written = TIFFWriteRawStrip(tif, 0, buf, size_param);
                (void)written;
                free(buf);
            }
            TIFFClose(tif);
        }
    }

    remove(filename);
    return 0;
}