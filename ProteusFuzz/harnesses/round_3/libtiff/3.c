// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare the environment: Create a dummy file for writing
    const char *filename = "./dummy_file";
    
    // Write the fuzzing input to the dummy file
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Open the TIFF file for writing
    TIFF *tif = TIFFOpen(filename, "w8");
    if (!tif) {
        return 0;
    }

    // Invoke the target functions in the specified order
    
    // 1. TIFFFreeDirectory
    // Note: Called on a newly opened file. While typically used to clean up,
    // calling it here ensures we test the cleanup path on a fresh handle.
    TIFFFreeDirectory(tif);

    // 2. TIFFCreateDirectory
    // Initializes a new directory.
    if (!TIFFCreateDirectory(tif)) {
        // TIFFCreateDirectory usually returns 1 on success, 0 on failure.
        // If it fails, we cannot proceed with SetField safely.
        TIFFClose(tif);
        return 0;
    }

    // 3-10. TIFFSetField (8 calls)
    // We attempt to set various fields. We use data from the input buffer
    // to drive the values, ensuring diverse program states.
    // We must ensure we don't pass invalid pointers or sizes.
    
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    uint32_t val3 = 0;
    uint32_t val4 = 0;
    uint32_t val5 = 0;
    uint32_t val6 = 0;
    uint32_t val7 = 0;
    uint32_t val8 = 0;

    if (Size >= 8) {
        val1 = Data[0];
        val2 = Data[1];
        val3 = Data[2];
        val4 = Data[3];
        val5 = Data[4];
        val6 = Data[5];
        val7 = Data[6];
        val8 = Data[7];
    }

    // Set ImageWidth (TAG_IMAGEWIDTH)
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, val1 ? val1 : 1);
    
    // Set ImageLength (TAG_IMAGELENGTH)
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, val2 ? val2 : 1);
    
    // Set BitsPerSample (TAG_BITSPERSAMPLE) - usually an array, but try single
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, val3 ? val3 : 1);
    
    // Set Compression (TAG_COMPRESSION)
    TIFFSetField(tif, TIFFTAG_COMPRESSION, val4 ? val4 : COMPRESSION_NONE);
    
    // Set PhotometricInterpretation (TAG_PHOTOMETRIC)
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, val5 ? val5 : PHOTOMETRIC_MINISBLACK);
    
    // Set RowsPerStrip (TAG_ROWSPERSTRIP)
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, val6 ? val6 : 1);
    
    // Set PlanarConfig (TAG_PLANARCONFIG)
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, val7 ? val7 : PLANARCONFIG_CONTIG);
    
    // Set SampleFormat (TAG_SAMPLEFORMAT)
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, val8 ? val8 : SAMPLEFORMAT_UINT);

    // 11. TIFFDeferStrileArrayWriting
    // Defers writing strip/tile arrays.
    TIFFDeferStrileArrayWriting(tif);

    // 12. TIFFWriteCheck
    // Validates write readiness.
    // Arguments: tif, orientation (int), function name (const char *)
    // We use a dummy orientation and function name.
    TIFFWriteCheck(tif, 0, "fuzz_test");

    // Cleanup
    TIFFClose(tif);

    // Remove the dummy file
    unlink(filename);

    return 0;
}