// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* 
 * TIFFExtendProc is defined as void (*)(TIFF *).
 * The previous attempt used a function with signature void (*)(TIFF *, uint16_t *),
 * which caused the incompatible function pointer type error.
 * We define a dummy extender that matches the expected signature: void (*)(TIFF *).
 */
static void dummy_extender(TIFF *tif) {
    (void)tif;
    /* Do nothing */
}

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f != NULL) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            // Write minimal TIFF header if data is empty to ensure valid file structure attempt
            // Byte order (II or MM) + Magic number (42) + Offset to first IFD
            uint16_t header[] = {0x002A, 0x0000, 0x0008, 0x0000};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    
    // Step 1: Prepare the environment
    // Write the fuzzing data to the dummy file
    write_dummy_file(filename, Data, Size);

    // Step 2: Invoke target functions in order
    // 1. TIFFOpen: Open the file for reading
    TIFF *tif = TIFFOpen(filename, "r");
    
    if (tif != NULL) {
        // 2. TIFFSetTagExtender: Set a custom extender
        // We pass a dummy function or NULL to test different states
        TIFFExtendProc old_extender = TIFFSetTagExtender(dummy_extender);
        
        // Try setting it back to NULL to explore state reset
        TIFFSetTagExtender(NULL);
        
        // Restore previous extender if it was not NULL, or just leave as NULL
        if (old_extender != NULL) {
            TIFFSetTagExtender(old_extender);
        }

        // 3. TIFFClose: Close the file safely
        TIFFClose(tif);
    } else {
        // If TIFFOpen fails (e.g., invalid data), we still test the other functions
        // TIFFSetTagExtender is global, so it can be called even without a valid TIFF handle
        TIFFSetTagExtender(dummy_extender);
        TIFFSetTagExtender(NULL);
        
        // TIFFClose handles NULL gracefully as per documentation
        TIFFClose(NULL);
    }

    // Cleanup: Remove the dummy file
    remove(filename);

    return 0;
}