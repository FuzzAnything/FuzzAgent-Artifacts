// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
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

static void write_dummy_tiff(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header (little-endian)
    // Byte order: II (0x4949)
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    // TIFF version: 42 (0x002A)
    uint16_t version = 42;
    fwrite(&version, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD: 8 (0x00000008)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // IFD entry count: 0 (empty IFD)
    uint16_t ifd_count = 0;
    fwrite(&ifd_count, sizeof(uint16_t), 1, f);
    
    // Next IFD offset: 0 (end of chain)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    const char *filename = "./dummy_file";
    
    // Ensure we have some data to work with, or create a minimal valid file
    if (Size < 8) {
        write_dummy_tiff(filename);
    } else {
        // Write the fuzz input to the dummy file
        FILE *f = fopen(filename, "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }
    }
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }
    
    // Step 1: Invoke TIFFStripSize
    // This function requires a valid TIFF handle and a current directory
    tmsize_t strip_size = TIFFStripSize(tif);
    
    // Step 2: Invoke _TIFFmalloc
    // Allocate memory based on the strip size or a safe default if strip_size is 0
    tmsize_t alloc_size = (strip_size > 0 && strip_size < (1024 * 1024)) ? strip_size : 1024;
    void *ptr = _TIFFmalloc(alloc_size);
    
    // Step 3: Invoke _TIFFfree (first time)
    // Free the allocated memory
    _TIFFfree(ptr);
    
    // Step 4: Invoke _TIFFfree (second time)
    // Freeing NULL is safe according to the spec
    _TIFFfree(NULL);
    
    // Cleanup: Close the TIFF file
    TIFFClose(tif);
    
    return 0;
}