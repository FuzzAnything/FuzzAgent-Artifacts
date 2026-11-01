// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";

    // First sequence
    TIFF *tif1 = TIFFOpen(filename, mode);
    if (tif1 != NULL) {
        TIFFCheckpointDirectory(tif1);
        TIFFSetField(tif1, TIFFTAG_IMAGEWIDTH, (uint32_t)(Data[0] % 100) + 1);
        TIFFSetField(tif1, TIFFTAG_IMAGELENGTH, (uint32_t)(Data[1] % 100) + 1);
        TIFFWriteDirectory(tif1);
        TIFFSetDirectory(tif1, 0);
        TIFFCreateDirectory(tif1);
        TIFFClose(tif1);
    }

    // Second sequence
    TIFF *tif2 = TIFFOpen(filename, mode);
    if (tif2 != NULL) {
        TIFFSetField(tif2, TIFFTAG_BITSPERSAMPLE, (uint16_t)(Data[2] % 16) + 1);
        TIFFCheckpointDirectory(tif2);
        TIFFSetField(tif2, TIFFTAG_COMPRESSION, (uint16_t)(Data[3] % 10) + 1);
        TIFFCheckpointDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFClose(tif2);
    }

    // Third sequence
    TIFF *tif3 = TIFFOpen(filename, "r");
    if (tif3 != NULL) {
        TIFFSetDirectory(tif3, 0);
        TIFFSetDirectory(tif3, 1);
        TIFFClose(tif3);
    }

    // Fourth sequence (potential NULL or invalid handle check)
    TIFF *tif4 = TIFFOpen(filename, mode);
    if (tif4 != NULL) {
        TIFFClose(tif4);
    }

    // Cleanup dummy file
    unlink(filename);

    return 0;
}