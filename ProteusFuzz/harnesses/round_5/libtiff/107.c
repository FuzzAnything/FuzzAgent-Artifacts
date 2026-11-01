// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) {
        return;
    }
    if (Size > 0 && Data != NULL) {
        fwrite(Data, 1, Size, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w+");
    if (tif == NULL) {
        return 0;
    }

    // Set basic properties to allow writing a directory
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

    // Fuzz TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // Fuzz TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // Fuzz TIFFCreateCustomDirectory
    // We cannot easily construct a valid TIFFFieldArray from fuzz input without complex logic,
    // so we skip this specific API or rely on the internal EXIF/GPS arrays if accessible.
    // Since the prompt says "Do not pass NULL", and we don't have a valid custom array from fuzz data,
    // we skip direct invocation with fuzzed data to avoid crashes, 
    // but the other functions exercise the directory creation logic.

    // Fuzz TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // Fuzz TIFFSetDirectory
    // Try setting directory 0 (current) or 1 (if it exists)
    TIFFSetDirectory(tif, 0);
    TIFFSetDirectory(tif, 1);

    // Fuzz TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    TIFFClose(tif);

    return 0;
}