// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
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
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open for reading
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (tif_r) {
        uint64_t diroff = 0;
        toff_t offset = 0;
        tdir_t dir_idx = 0;
        const TIFFFieldArray *infoarray = NULL;

        // Fuzz TIFFReadEXIFDirectory
        TIFFReadEXIFDirectory(tif_r, offset);

        // Fuzz TIFFReadCustomDirectory
        TIFFReadCustomDirectory(tif_r, offset, infoarray);

        // Fuzz TIFFSetSubDirectory
        // Use data from input to generate offset, ensuring it's within reasonable bounds
        if (Size >= 8) {
            offset = *(uint64_t *)Data;
        } else if (Size > 0) {
            offset = Data[0];
        }
        TIFFSetSubDirectory(tif_r, offset);

        // Fuzz TIFFSetDirectory
        if (Size >= 4) {
            dir_idx = *(uint32_t *)Data;
        } else if (Size > 0) {
            dir_idx = Data[0];
        }
        TIFFSetDirectory(tif_r, dir_idx);

        // Fuzz TIFFLastDirectory
        TIFFLastDirectory(tif_r);

        TIFFClose(tif_r);
    }

    // Open for writing to test write functions
    TIFF *tif_w = TIFFOpen("./dummy_file", "w");
    if (tif_w) {
        uint64_t pdir_offset = 0;

        // Set some basic fields to allow writing a directory
        uint16_t width = 100;
        uint16_t height = 100;
        TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_w, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_w, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, 1);

        // Fuzz TIFFWriteCustomDirectory
        if (Size >= 8) {
            pdir_offset = *(uint64_t *)Data;
        } else if (Size > 0) {
            pdir_offset = Data[0];
        }
        TIFFWriteCustomDirectory(tif_w, &pdir_offset);

        // Fuzz TIFFSetSubDirectory in write mode
        if (Size >= 8) {
            uint64_t sub_off = *(uint64_t *)Data;
            TIFFSetSubDirectory(tif_w, sub_off);
        }

        // Fuzz TIFFSetDirectory in write mode
        if (Size >= 4) {
            tdir_t w_dir_idx = *(uint32_t *)Data;
            TIFFSetDirectory(tif_w, w_dir_idx);
        }

        // Fuzz TIFFLastDirectory in write mode
        TIFFLastDirectory(tif_w);

        TIFFClose(tif_w);
    }

    return 0;
}