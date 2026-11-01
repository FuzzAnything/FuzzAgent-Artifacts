// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // Little-endian, TIFF version 42, IFD offset 8
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Provide minimal data for file creation if input is empty
        const uint8_t dummy_header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        write_dummy_file(dummy_header, 8);
    } else {
        write_dummy_file(Data, Size);
    }

    // Open the dummy file for reading (to test read-related APIs like TIFFLastDirectory, TIFFSetDirectory)
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Test TIFFLastDirectory
        TIFFLastDirectory(tif_read);

        // Test TIFFSetDirectory with various indices
        tdir_t dir_idx = 0;
        // Try a few iterations to explore state
        for (int i = 0; i < 5; i++) {
            TIFFSetDirectory(tif_read, dir_idx);
            dir_idx++;
        }
        // Try a large index
        TIFFSetDirectory(tif_read, 0xFFFFFFFFU);

        TIFFClose(tif_read);
    }

    // Open the dummy file for writing (to test write-related APIs)
    // Note: If the file is not a valid TIFF, TIFFOpen might fail or behave unexpectedly, which is fine for fuzzing.
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Test TIFFCreateEXIFDirectory
        TIFFCreateEXIFDirectory(tif_write);

        // Test TIFFCreateGPSDirectory
        TIFFCreateGPSDirectory(tif_write);

        // Test TIFFCreateCustomDirectory
        // Avoid passing NULL as it causes a segfault inside libtiff when dereferencing the array.
        // We cannot easily construct a valid TIFFFieldArray here without internal headers,
        // so we skip this specific call with NULL or invalid data to prevent crashes.
        // Instead, we rely on the other directory creation functions which use internal arrays.
        
        // Test TIFFRewriteDirectory
        // This requires a valid directory to exist. If the file was just opened for writing,
        // the first directory might not be written yet, but the call is safe to attempt.
        TIFFRewriteDirectory(tif_write);

        // Attempt to set a directory before writing one (edge case)
        TIFFSetDirectory(tif_write, 0);
        TIFFRewriteDirectory(tif_write);

        TIFFClose(tif_write);
    }

    // Clean up the dummy file
    remove("./dummy_file");

    return 0;
}