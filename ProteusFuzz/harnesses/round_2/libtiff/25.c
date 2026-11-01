// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    /* Basic setup to make the TIFF object valid for some operations */
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    /* Fuzz TIFFSetupStrips */
    TIFFSetupStrips(tif);

    /* Fuzz TIFFWriteCheck */
    TIFFWriteCheck(tif, 0, "Test");

    /* Fuzz TIFFSetMode */
    int old_mode = TIFFSetMode(tif, O_RDONLY);
    (void)old_mode;
    TIFFSetMode(tif, O_RDWR);

    /* Fuzz TIFFDeferStrileArrayWriting */
    TIFFDeferStrileArrayWriting(tif);

    /* Fuzz TIFFForceStrileArrayWriting */
    TIFFForceStrileArrayWriting(tif);

    /* Fuzz TIFFFlush */
    TIFFFlush(tif);

    TIFFClose(tif);

    /* Try opening in read mode to test read-only paths */
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFSetupStrips(tif);
        TIFFWriteCheck(tif, 0, "Test");
        TIFFSetMode(tif, O_RDWR);
        TIFFDeferStrileArrayWriting(tif);
        TIFFForceStrileArrayWriting(tif);
        TIFFFlush(tif);
        TIFFClose(tif);
    }

    return 0;
}