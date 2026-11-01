// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r+");
    if (!tif) {
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            return 0;
        }
    }

    TIFFFreeDirectory(tif);

    if (TIFFCreateDirectory(tif) != 0) {
        // Continue regardless
    }

    uint32_t width = 0;
    if (Size > 0) width = Data[0] + 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);

    uint32_t length = 0;
    if (Size > 1) length = Data[1] + 1;
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);

    uint16_t bps = 8;
    if (Size > 2) bps = Data[2] % 32 + 1;
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps);

    uint16_t compression = COMPRESSION_NONE;
    if (Size > 3) compression = (Data[3] % 5) + 1;
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    if (Size > 4) photometric = Data[4] % 5 + 1;
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);

    uint32_t rowsPerStrip = 0;
    if (Size > 5) rowsPerStrip = Data[5] + 1;
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsPerStrip);

    uint16_t planar = PLANARCONFIG_CONTIG;
    if (Size > 6) planar = (Data[6] % 2) + 1;
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar);

    uint16_t sampleFormat = SAMPLEFORMAT_UINT;
    if (Size > 7) sampleFormat = (Data[7] % 3) + 1;
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sampleFormat);

    TIFFDeferStrileArrayWriting(tif);

    TIFFWriteCheck(tif, 0, "fuzz_test");

    TIFFClose(tif);
    
    unlink(filename);

    return 0;
}