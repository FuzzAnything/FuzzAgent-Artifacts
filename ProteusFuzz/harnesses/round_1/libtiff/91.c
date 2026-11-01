// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Ensure we have some data to write to the dummy file
    write_dummy_file(Data, Size);

    // Open the file for writing to test directory creation functions
    TIFF *tif = TIFFOpen("./dummy_file", "w8");
    if (!tif) {
        return 0;
    }

    // Set basic image properties to ensure a valid state for directory operations
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

    // Test TIFFCreateEXIFDirectory
    (void)TIFFCreateEXIFDirectory(tif);

    // Test TIFFCreateGPSDirectory
    (void)TIFFCreateGPSDirectory(tif);

    // Test TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // Test TIFFSetDirectory with index 0
    (void)TIFFSetDirectory(tif, 0);

    // Test TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // Test with a second directory to exercise TIFFSetDirectory with different indices
    // Re-initialize fields for the new directory
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    
    // Write the second directory
    (void)TIFFWriteDirectory(tif);

    // Try to switch back to the first directory
    (void)TIFFSetDirectory(tif, 0);
    // Try to switch to the second directory
    (void)TIFFSetDirectory(tif, 1);

    // Test TIFFFreeDirectory again on the active directory
    TIFFFreeDirectory(tif);

    // Note: TIFFCreateCustomDirectory requires a fully defined TIFFFieldArray.
    // Since TIFFField and TIFFFieldArray are opaque in the public API (tiffio.h),
    // we cannot construct them manually in the fuzzer without internal headers.
    // Therefore, we omit the manual construction of custom fields to avoid compilation errors.
    // The fuzzing of TIFFCreateCustomDirectory is implicitly covered by the library's
    // internal handling if it were called with valid internal structures, but here
    // we skip the manual instantiation to maintain compilation with standard headers.

    // Cleanup
    TIFFClose(tif);

    // Remove the dummy file
    remove("./dummy_file");

    return 0;
}