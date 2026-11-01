// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void create_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header
    // Byte order (Little Endian)
    uint16_t byte_order = 0x002A;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD (we will skip to 8 bytes for simplicity, though invalid)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // Write some dummy data if available
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    void *ptr = NULL;
    tmsize_t strip_size = 0;
    
    // Step 0: Prepare environment
    // Create a dummy file to open. We write the input data to it to ensure 
    // the file exists and has content, even if the content is not a perfect TIFF.
    create_dummy_tiff(Data, Size);
    
    // Open the file for reading
    tif = TIFFOpen("./dummy_file", "r");
    
    if (tif != NULL) {
        // Step 1: Invoke TIFFStripSize
        // This function requires a valid TIFF handle. 
        // It may return 0 if the file is invalid or strip count is zero.
        strip_size = TIFFStripSize(tif);
        
        // Step 2: Invoke _TIFFmalloc
        // We use the strip_size returned above. 
        // If strip_size is 0, _TIFFmalloc returns NULL, which is safe to free.
        // If strip_size is huge, malloc might fail and return NULL.
        // We add a small check to avoid potential integer overflow or extreme allocation 
        // that might crash the fuzzer immediately, though _TIFFmalloc handles 0.
        if (strip_size > 0 && strip_size < (1024 * 1024 * 1024)) { // Limit to 1GB
            ptr = _TIFFmalloc(strip_size);
        } else {
            // Even if we don't allocate, we can try to allocate a small fixed size
            // to exercise the allocation path if strip_size was invalid.
            ptr = _TIFFmalloc(1024);
        }
        
        // Step 3: Invoke _TIFFfree (First call)
        // Free the memory allocated by _TIFFmalloc.
        if (ptr != NULL) {
            _TIFFfree(ptr);
            ptr = NULL; // Ensure we don't double free the same pointer
        }
        
        // Step 4: Invoke _TIFFfree (Second call)
        // The task requires calling _TIFFfree twice.
        // Since ptr is now NULL, this call is safe (free(NULL) is safe).
        // This exercises the NULL check path in _TIFFfree.
        _TIFFfree(ptr);
        
        // Cleanup: Close the TIFF file
        TIFFClose(tif);
    }
    
    // Cleanup: Remove the dummy file
    remove("./dummy_file");
    
    return 0;
}