// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write minimal valid header if input is empty to avoid immediate open failure
            // Basic TIFF header (little endian)
            uint16_t magic = 0x002A; // II
            uint16_t offset = 8;
            uint16_t dir_count = 0;
            fwrite(&magic, sizeof(uint16_t), 1, fp);
            fwrite(&offset, sizeof(uint16_t), 1, fp);
            fwrite(&dir_count, sizeof(uint16_t), 1, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    int ret = 0;

    if (Size < 2) {
        return 0;
    }

    // Prepare environment: create a dummy file
    setup_dummy_file(Data, Size);

    // Open the file for writing to allow directory manipulation
    // We use "w+" to ensure write capability for the checks required
    tif = TIFFOpen("./dummy_file", "w+");
    if (tif == NULL) {
        return 0;
    }

    // 1. TIFFFreeDirectory
    // Safe to call on a newly opened file to ensure clean state
    TIFFFreeDirectory(tif);

    // 2. TIFFCreateDirectory
    // Initialize a new directory
    ret = TIFFCreateDirectory(tif);
    if (ret != 0) {
        // Should always return 0, but handle if behavior changes
    }

    // 3-10. TIFFSetField (8 calls)
    // Set various fields to explore different code paths.
    // We use safe, standard tags. If the fuzzer provides data, we might use it,
    // but for stability in fuzzing, we use fixed valid tags first.
    
    // ImageWidth (Tag 256)
    uint32_t width = (Size > 0) ? (Data[0] + 1) : 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);

    // ImageLength (Tag 257)
    uint32_t length = (Size > 1) ? (Data[1] + 1) : 1;
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);

    // BitsPerSample (Tag 258)
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);

    // Compression (Tag 259)
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

    // PhotometricInterpretation (Tag 262)
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    // RowsPerStrip (Tag 278)
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, length);

    // PlanarConfiguration (Tag 284)
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // SampleFormat (Tag 339)
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);

    // 11. TIFFDeferStrileArrayWriting
    // Defers writing strip/tile arrays
    ret = TIFFDeferStrileArrayWriting(tif);

    // 12. TIFFWriteCheck
    // Validates write readiness. 
    // We pass 0 for the row/strip index and a dummy function name.
    ret = TIFFWriteCheck(tif, 0, "fuzz_test");

    // Cleanup
    TIFFClose(tif);
    
    // Remove the dummy file
    remove("./dummy_file");

    return 0;
}