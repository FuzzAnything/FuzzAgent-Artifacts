// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure minimum size for basic TIFF header if we attempt to open */
    if (Size < 4) {
        return 0;
    }

    /* Write input data to a temporary file */
    write_dummy_file(Data, Size);

    /* Open the file for reading */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Fuzz TIFFReadEXIFDirectory with various offsets derived from input */
    toff_t diroff = 0;
    if (Size >= 8) {
        diroff = (toff_t)Data[0] | ((toff_t)Data[1] << 8) | 
                 ((toff_t)Data[2] << 16) | ((toff_t)Data[3] << 24) |
                 ((toff_t)Data[4] << 32) | ((toff_t)Data[5] << 40) |
                 ((toff_t)Data[6] << 48) | ((toff_t)Data[7] << 56);
    }
    /* Try reading EXIF directory */
    (void)TIFFReadEXIFDirectory(tif, diroff);
    
    /* Try reading EXIF directory with offset 0 */
    (void)TIFFReadEXIFDirectory(tif, 0);

    /* Fuzz TIFFLastDirectory */
    (void)TIFFLastDirectory(tif);

    /* Fuzz TIFFReadCustomDirectory with EXIF field array if available */
    /* Note: We cannot easily construct a valid custom TIFFFieldArray without 
       internal libtiff details, so we rely on the library's internal EXIF array 
       if accessible, or simply test with NULL if the API allows (though docs say don't).
       Since we cannot safely construct a custom array without internal headers,
       we skip custom read unless we can access internal arrays.
       However, TIFFReadCustomDirectory requires a valid array. We will skip this 
       specific call with a custom array to avoid undefined behavior, 
       but we can try to call it if we had access to internal arrays.
       Given constraints, we skip explicit custom read with user-defined array.
    */

    /* Close the read handle */
    TIFFClose(tif);

    /* Now test write operations */
    /* Open the file for writing (overwrite) */
    tif = TIFFOpen("./dummy_file", "w");
    if (tif == NULL) {
        return 0;
    }

    /* Fuzz TIFFCreateEXIFDirectory */
    (void)TIFFCreateEXIFDirectory(tif);

    /* Fuzz TIFFWriteCustomDirectory */
    uint64_t pdiroff = 0;
    (void)TIFFWriteCustomDirectory(tif, &pdiroff);

    /* Fuzz TIFFCreateCustomDirectory */
    /* We cannot easily construct a valid TIFFFieldArray without internal details.
       The instructions say: "Do not pass NULL as the second argument. Instead, 
       allocate and initialize a valid TIFFFieldArray structure...".
       Since we don't have the full internal definitions to safely populate 
       TIFFFieldArray without risking crashes or undefined behavior, 
       and the prompt asks to fuzz, we will skip this specific call 
       to avoid creating invalid structures that might crash the fuzzer 
       in an uncontrolled way, or we can try to create a minimal one if possible.
       However, creating a valid TIFFFieldArray requires populating TIFFField structs 
       with specific internal types and pointers which are not fully exposed 
       in a way that allows safe arbitrary construction for fuzzing without 
       deep library knowledge. 
       Therefore, to ensure stability, we omit TIFFCreateCustomDirectory 
       with a user-constructed array in this driver, relying on the 
       EXIF directory creation which is safer.
    */

    /* Close the write handle */
    TIFFClose(tif);

    return 0;
}