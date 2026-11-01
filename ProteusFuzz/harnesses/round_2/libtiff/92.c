// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
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
        if (Size > 0 && Data != NULL) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty to ensure file exists
            uint16_t header[] = {0x4949, 0x002A, 0x00000008}; 
            fwrite(header, sizeof(header), 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure a file exists for the fuzzer to operate on
    write_dummy_file(Data, Size);

    // Open the file for writing to test directory creation and writing functions
    TIFF *tif = TIFFOpen("./dummy_file", "w8");
    if (!tif) {
        return 0;
    }

    // Fuzz TIFFCreateDirectory
    // This initializes a new directory. It always returns 0 on success.
    TIFFCreateDirectory(tif);

    // Fuzz TIFFWriteDirectory
    // Writes the current directory. Returns 1 on success.
    (void)TIFFWriteDirectory(tif);

    // Fuzz TIFFCreateEXIFDirectory
    // Initializes for EXIF metadata.
    (void)TIFFCreateEXIFDirectory(tif);

    // Fuzz TIFFCreateGPSDirectory
    // Initializes for GPS metadata.
    (void)TIFFCreateGPSDirectory(tif);

    // Fuzz TIFFCreateCustomDirectory
    // Requires a valid TIFFFieldArray. Since TIFFFieldArray is an incomplete type in tiffio.h,
    // we cannot allocate or access its members directly. 
    // However, the API documentation states "Do not pass NULL".
    // To fuzz this, we can try to pass a pointer to a small allocated buffer that mimics the start
    // of the structure, hoping the internal implementation reads specific offsets.
    // This is a heuristic approach for fuzzing when internal structures are opaque.
    
    // Allocate a buffer large enough to hold at least the first few fields of TIFFFieldArray
    // Based on the provided struct definition: type (4), allocated_size (4), count (4), fields (8 on 64bit)
    // We allocate a safe size, e.g., 32 bytes.
    void *custom_info_buf = malloc(32);
    if (custom_info_buf) {
        // Zero out the buffer
        memset(custom_info_buf, 0, 32);
        
        // Cast to TIFFFieldArray* for the function call.
        // The function will likely read the 'type' and 'count' fields.
        // Since we zeroed it, count is 0, which is a valid state for an empty array.
        (void)TIFFCreateCustomDirectory(tif, (const TIFFFieldArray *)custom_info_buf);
        
        free(custom_info_buf);
    }

    // Fuzz TIFFFreeDirectory
    // Frees memory allocated for the current directory.
    TIFFFreeDirectory(tif);

    // Cleanup
    TIFFClose(tif);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}