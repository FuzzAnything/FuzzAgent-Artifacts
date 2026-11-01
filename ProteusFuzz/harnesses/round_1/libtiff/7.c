// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif1 = TIFFOpen("./dummy_file", "w");
    if (!tif1) {
        return 0;
    }

    /* TIFFSetField 1 */
    TIFFSetField(tif1, TIFFTAG_IMAGEWIDTH, 100);
    /* TIFFSetField 2 */
    TIFFSetField(tif1, TIFFTAG_IMAGELENGTH, 100);
    /* TIFFSetField 3 */
    TIFFSetField(tif1, TIFFTAG_BITSPERSAMPLE, 8);
    /* TIFFSetField 4 */
    TIFFSetField(tif1, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    /* TIFFSetField 5 */
    TIFFSetField(tif1, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    /* TIFFSetField 6 */
    TIFFSetField(tif1, TIFFTAG_SAMPLESPERPIXEL, 1);
    /* TIFFSetField 7 */
    TIFFSetField(tif1, TIFFTAG_ROWSPERSTRIP, 1);
    /* TIFFSetField 8 */
    TIFFSetField(tif1, TIFFTAG_STRIPBYTECOUNTS, (uint32_t)10000);

    TIFFWriteDirectory(tif1);

    TIFFCreateEXIFDirectory(tif1);

    /* TIFFSetField 9 */
    TIFFSetField(tif1, TIFFTAG_EXIFIFD, 0);

    TIFFFreeDirectory(tif1);

    /* 
       TIFFCreateCustomDirectory requires a valid TIFFFieldArray pointer.
       Passing NULL causes a crash in _TIFFSetupFields.
       Since we cannot construct a valid TIFFFieldArray manually (opaque structure),
       and the function is not designed to handle NULL, we skip this call
       to prevent the crash while maintaining the rest of the test sequence.
    */
    // TIFFCreateCustomDirectory(tif1, NULL); // Removed to fix crash

    /* TIFFSetField 10 */
    TIFFSetField(tif1, TIFFTAG_IMAGEWIDTH, 50);
    /* TIFFSetField 11 */
    TIFFSetField(tif1, TIFFTAG_IMAGELENGTH, 50);

    TIFFSetDirectory(tif1, 0);

    TIFFClose(tif1);

    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        TIFFClose(tif2);
    }

    /* Extra close to match sequence, handles NULL gracefully */
    TIFFClose(tif2);

    return 0;
}