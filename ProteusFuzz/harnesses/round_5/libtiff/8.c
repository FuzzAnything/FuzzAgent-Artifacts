// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
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
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    TIFF *tif1 = NULL;
    TIFF *tif2 = NULL;
    TIFF *tif3 = NULL;
    
    // Write input data to dummy file
    write_dummy_file(filename, Data, Size);

    // Sequence 1: TIFFOpen -> TIFFCheckpointDirectory -> TIFFSetField -> TIFFWriteDirectory -> TIFFSetDirectory -> TIFFCreateDirectory -> TIFFClose
    tif1 = TIFFOpen(filename, "w");
    if (tif1) {
        TIFFCheckpointDirectory(tif1);
        // Set a valid field (ImageWidth)
        TIFFSetField(tif1, TIFFTAG_IMAGEWIDTH, 100);
        TIFFWriteDirectory(tif1);
        TIFFSetDirectory(tif1, 0);
        TIFFCreateDirectory(tif1);
        TIFFClose(tif1);
        tif1 = NULL;
    }

    // Sequence 2: TIFFOpen -> TIFFSetField -> TIFFCheckpointDirectory -> TIFFSetField -> TIFFCheckpointDirectory -> TIFFWriteDirectory -> TIFFClose
    tif2 = TIFFOpen(filename, "a");
    if (tif2) {
        TIFFSetField(tif2, TIFFTAG_IMAGELENGTH, 100);
        TIFFCheckpointDirectory(tif2);
        TIFFSetField(tif2, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFCheckpointDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFClose(tif2);
        tif2 = NULL;
    }

    // Sequence 3: TIFFOpen -> TIFFSetDirectory -> TIFFSetDirectory -> TIFFClose
    tif3 = TIFFOpen(filename, "r");
    if (tif3) {
        TIFFSetDirectory(tif3, 0);
        TIFFSetDirectory(tif3, 1);
        TIFFClose(tif3);
        tif3 = NULL;
    }

    // Final cleanup (should be no-ops as handles are NULL, but included per sequence)
    TIFFClose(tif1);
    TIFFClose(tif2);
    TIFFClose(tif3);
    TIFFClose(tif3); // Extra close as per sequence

    return 0;
}