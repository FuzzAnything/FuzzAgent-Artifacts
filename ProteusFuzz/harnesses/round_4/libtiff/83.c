// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment
    setup_dummy_file(Data, Size);

    // Open the file for reading and writing
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (tif == NULL) {
        // Try opening as read-only if r+ fails (e.g., file might be too small for write mode)
        tif = TIFFOpen("./dummy_file", "r");
        if (tif == NULL) {
            return 0;
        }
    }

    // Fuzzing logic to explore diverse states

    // 1. Try to set a directory (index derived from input data)
    tdir_t dir_index = (Size > 0) ? (tdir_t)(Data[0] % 100) : 0;
    TIFFSetDirectory(tif, dir_index);

    // 2. Try to print directory to stdout (flags derived from input)
    long print_flags = (Size > 1) ? (long)(Data[1] & 0xFF) : 0;
    TIFFPrintDirectory(tif, stdout, print_flags);

    // 3. Try to create a custom directory
    // TIFFCreateCustomDirectory requires a valid TIFFFieldArray pointer.
    // Passing NULL causes a crash inside the library as seen in the ASAN report.
    // We skip this call to avoid the crash, or we would need to construct a valid TIFFFieldArray.
    // Since constructing a valid TIFFFieldArray is complex and requires internal structures,
    // we omit this specific call for safety in this fuzz driver context.
    // TIFFCreateCustomDirectory(tif, NULL); // Removed to prevent crash

    // 4. Try to create an EXIF directory
    // This function internally handles the field array, so it is safe to call.
    TIFFCreateEXIFDirectory(tif);

    // 5. Try to write the directory
    TIFFWriteDirectory(tif);

    // 6. Free the directory
    TIFFFreeDirectory(tif);

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}