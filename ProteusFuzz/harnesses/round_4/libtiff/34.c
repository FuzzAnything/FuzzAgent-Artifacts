// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            uint16_t header[4];
            header[0] = 0x002A; // Little-endian
            header[1] = 42;     // Magic number
            header[2] = 8;      // First IFD offset
            header[3] = 0;      // Padding
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Test reading operations
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Test TIFFLastDirectory
        (void)TIFFLastDirectory(tif_read);

        // Test TIFFReadGPSDirectory with various offsets
        toff_t offset = 0;
        if (Size >= sizeof(toff_t)) {
            memcpy(&offset, Data, sizeof(toff_t));
        }
        (void)TIFFReadGPSDirectory(tif_read, offset);

        // Test TIFFReadEXIFDirectory
        (void)TIFFReadEXIFDirectory(tif_read, offset);

        // Test TIFFReadCustomDirectory
        const TIFFFieldArray *custom_array = NULL;
        (void)TIFFReadCustomDirectory(tif_read, offset, custom_array);

        // Iterate directories to trigger more states
        while (!TIFFLastDirectory(tif_read)) {
            if (!TIFFReadDirectory(tif_read)) {
                break;
            }
            (void)TIFFLastDirectory(tif_read);
        }

        TIFFClose(tif_read);
    }

    // Test writing operations
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Test TIFFCreateEXIFDirectory
        (void)TIFFCreateEXIFDirectory(tif_write);

        // Test TIFFCreateGPSDirectory
        (void)TIFFCreateGPSDirectory(tif_write);

        // Try to write a minimal directory to ensure valid state for subsequent calls
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 1);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, 1);

        // Write a dummy strip
        uint8_t *buf = (uint8_t *)malloc(1);
        if (buf) {
            buf[0] = 0;
            TIFFWriteScanline(tif_write, buf, 0, 0);
            free(buf);
        }

        // Re-test creation functions after writing
        (void)TIFFCreateEXIFDirectory(tif_write);
        (void)TIFFCreateGPSDirectory(tif_write);

        TIFFClose(tif_write);
    }

    return 0;
}