// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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

static uint32_t get_uint32(const uint8_t *d, size_t s, size_t idx) {
    if (idx + 4 > s) return 0;
    uint32_t val = 0;
    memcpy(&val, d + idx, 4);
    return val;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }

    // Write input data to the file to ensure it's not empty
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    // Open the TIFF file for writing
    TIFF *tif = TIFFOpen(filename, "w8");
    if (tif == NULL) {
        unlink(filename);
        return 0;
    }

    // 1. TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // 2. TIFFCreateDirectory
    if (TIFFCreateDirectory(tif) == 0) {
        // Always returns 0 on success
    }

    // 3-10. TIFFSetField (8 calls)
    uint32_t val1 = get_uint32(Data, Size, 0);
    uint32_t val2 = get_uint32(Data, Size, 4);
    uint32_t val3 = get_uint32(Data, Size, 8);
    uint32_t val4 = get_uint32(Data, Size, 12);
    uint32_t val5 = get_uint32(Data, Size, 16);
    uint32_t val6 = get_uint32(Data, Size, 20);
    uint32_t val7 = get_uint32(Data, Size, 24);
    uint32_t val8 = get_uint32(Data, Size, 28);

    // Set ImageWidth (Tag 256) - required for write check
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, val1 ? val1 : 1);
    
    // Set ImageLength (Tag 257) - required for write check
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, val2 ? val2 : 1);
    
    // Set BitsPerSample (Tag 258)
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, val3 ? (val3 > 32 ? 32 : val3) : 8);
    
    // Set Compression (Tag 259)
    TIFFSetField(tif, TIFFTAG_COMPRESSION, val4 ? (val4 > 32 ? 32 : val4) : COMPRESSION_NONE);
    
    // Set PhotometricInterpretation (Tag 262)
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, val5 ? (val5 > 6 ? 6 : val5) : PHOTOMETRIC_MINISBLACK);
    
    // Set FillOrder (Tag 266)
    TIFFSetField(tif, TIFFTAG_FILLORDER, val6 ? (val6 > 2 ? 2 : val6) : FILLORDER_MSB2LSB);
    
    // Set SamplesPerPixel (Tag 277)
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, val7 ? (val7 > 4 ? 4 : val7) : 1);
    
    // Set PlanarConfig (Tag 284)
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, val8 ? (val8 > 2 ? 2 : val8) : PLANARCONFIG_CONTIG);

    // 11. TIFFDeferStrileArrayWriting
    TIFFDeferStrileArrayWriting(tif);

    // 12. TIFFWriteCheck
    TIFFWriteCheck(tif, 0, "LLVMFuzzerTestOneInput");

    // Cleanup
    TIFFClose(tif);
    
    // Remove the dummy file
    unlink(filename);

    return 0;
}