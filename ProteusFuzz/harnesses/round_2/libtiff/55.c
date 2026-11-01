// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    /* Attempt to read the first directory to initialize state */
    (void)TIFFReadDirectory(tif);

    /* Fuzz TIFFSetDirectory with various indices derived from input */
    if (Size >= 4) {
        uint32_t dir_idx = *(const uint32_t *)Data;
        /* Limit index to a reasonable range to avoid excessive loops or crashes */
        if (dir_idx < 1000) {
            (void)TIFFSetDirectory(tif, (tdir_t)dir_idx);
        }
    }

    /* Fuzz TIFFSetSubDirectory with various offsets */
    if (Size >= 8) {
        uint64_t sub_off = *(const uint64_t *)Data;
        (void)TIFFSetSubDirectory(tif, sub_off);
    }

    /* Fuzz TIFFReadGPSDirectory with various offsets */
    if (Size >= 8) {
        toff_t gps_off = *(const toff_t *)Data;
        (void)TIFFReadGPSDirectory(tif, gps_off);
    }

    /* Fuzz TIFFReadEXIFDirectory with various offsets */
    if (Size >= 8) {
        toff_t exif_off = *(const toff_t *)Data;
        (void)TIFFReadEXIFDirectory(tif, exif_off);
    }

    /* Fuzz TIFFLastDirectory */
    (void)TIFFLastDirectory(tif);

    /* Fuzz TIFFReadDirectory again to explore state changes */
    (void)TIFFReadDirectory(tif);

    TIFFClose(tif);
    return 0;
}