// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tiffio.h"

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFFreeDirectory(tif);
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write the fuzzed data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file in read mode first to test reading capabilities
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif) {
        // Test TIFFSetDirectory with various indices
        tdir_t dir_index = 0;
        if (Size > 0) {
            dir_index = Data[0];
        }
        TIFFSetDirectory(tif, dir_index);

        // Test TIFFPrintDirectory
        FILE *out_fp = fopen(filename, "a");
        if (out_fp) {
            long flags = 0;
            if (Size > 1) {
                flags = Data[1];
            }
            TIFFPrintDirectory(tif, out_fp, flags);
            fclose(out_fp);
        }

        // Close read handle
        TIFFClose(tif);
    }

    // Open the file in write mode to test writing capabilities
    // We use "w" to truncate and create a new file, or "a" to append
    // For fuzzing, "w" is safer to start fresh
    tif = TIFFOpen(filename, "w");
    if (tif) {
        // Set some basic tags to make the directory valid
        uint16_t width = 1;
        uint16_t height = 1;
        uint16_t bitspersample = 8;
        uint16_t samplesperpixel = 1;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t compression = COMPRESSION_NONE;
        
        if (Size > 2) {
            width = Data[2];
        }
        if (Size > 3) {
            height = Data[3];
        }
        if (Size > 4) {
            bitspersample = Data[4];
        }
        if (Size > 5) {
            samplesperpixel = Data[5];
        }
        if (Size > 6) {
            photometric = Data[6];
        }
        if (Size > 7) {
            compression = Data[7];
        }

        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

        // Test TIFFCreateEXIFDirectory
        TIFFCreateEXIFDirectory(tif);

        // Test TIFFWriteDirectory
        TIFFWriteDirectory(tif);

        // Test TIFFSetDirectory again (should work for index 0 now)
        TIFFSetDirectory(tif, 0);

        // Test TIFFRewriteDirectory
        TIFFRewriteDirectory(tif);

        // Test TIFFPrintDirectory on write mode
        FILE *out_fp = fopen(filename, "a");
        if (out_fp) {
            long flags = 0;
            if (Size > 8) {
                flags = Data[8];
            }
            TIFFPrintDirectory(tif, out_fp, flags);
            fclose(out_fp);
        }

        // Cleanup
        cleanup_tiff(tif);
    }

    // Test with read mode again on the potentially modified file
    tif = TIFFOpen(filename, "r");
    if (tif) {
        // Try to set directory to 0
        TIFFSetDirectory(tif, 0);
        
        // Try to print directory
        FILE *out_fp = fopen(filename, "a");
        if (out_fp) {
            TIFFPrintDirectory(tif, out_fp, 0);
            fclose(out_fp);
        }
        
        // Try to free directory
        TIFFFreeDirectory(tif);
        
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(filename);

    return 0;
}