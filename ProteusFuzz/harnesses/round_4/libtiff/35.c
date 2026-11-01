// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test _TIFFmalloc, _TIFFcalloc, _TIFFrealloc, _TIFFfree
    tmsize_t alloc_size = 0;
    if (Size > 0) {
        alloc_size = (tmsize_t)Data[0];
    }
    if (Size > 1) {
        alloc_size += (tmsize_t)Data[1] * 256;
    }
    // Cap size to prevent OOM
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }
    // Ensure minimum size for safety
    if (alloc_size == 0) alloc_size = 1;

    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr != NULL) {
        // Test _TIFFmemset
        _TIFFmemset(ptr, 0xAB, alloc_size);
        
        // Test _TIFFmemcpy
        // Ensure we do not read more from Data than available or write more to ptr than allocated
        if (Size > 0) {
            tmsize_t copy_len = (tmsize_t)Data[0];
            // Cap copy_len to the smaller of available input data and allocated buffer size
            if (copy_len > Size) copy_len = (tmsize_t)Size;
            if (copy_len > alloc_size) copy_len = alloc_size;
            if (copy_len > 0) {
                _TIFFmemcpy(ptr, Data, copy_len);
            }
        }

        // Test _TIFFrealloc
        tmsize_t new_size = alloc_size;
        if (Size > 2) {
            new_size += (tmsize_t)Data[2];
        }
        if (new_size > 1024 * 1024) new_size = 1024 * 1024;
        if (new_size == 0) new_size = 1;
        
        void *new_ptr = _TIFFrealloc(ptr, new_size);
        if (new_ptr != NULL) {
            _TIFFmemset(new_ptr, 0xFF, new_size);
            _TIFFfree(new_ptr);
        } else {
            _TIFFfree(ptr);
        }
    }

    // Test _TIFFcalloc
    tmsize_t nmemb = (Size > 0) ? Data[0] : 1;
    tmsize_t siz = (Size > 1) ? Data[1] : 1;
    if (nmemb > 10000) nmemb = 10000;
    if (siz > 10000) siz = 10000;
    
    void *calloc_ptr = _TIFFcalloc(nmemb, siz);
    if (calloc_ptr != NULL) {
        tmsize_t total_calloc_size = nmemb * siz;
        // Safety check for overflow or zero size
        if (total_calloc_size > 0) {
            _TIFFmemset(calloc_ptr, 0x12, total_calloc_size);
        }
        _TIFFfree(calloc_ptr);
    }

    // Test TIFFReadBufferSetup
    // We need a valid TIFF handle. Create a dummy file.
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        // Write a minimal valid TIFF header (little endian)
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, fp);
        fclose(fp);

        TIFF *tif = TIFFOpen(filename, "r");
        if (tif != NULL) {
            // Test with NULL buffer (let libtiff allocate)
            TIFFReadBufferSetup(tif, NULL, 0);

            // Test with custom buffer
            tmsize_t buf_size = (Size > 3) ? Data[3] : 64;
            if (buf_size > 1024 * 1024) buf_size = 1024 * 1024;
            if (buf_size == 0) buf_size = 64;
            
            void *custom_buf = _TIFFmalloc(buf_size);
            if (custom_buf != NULL) {
                _TIFFmemset(custom_buf, 0, buf_size);
                TIFFReadBufferSetup(tif, custom_buf, buf_size);
                _TIFFfree(custom_buf);
            }

            // Test with zero size (should fail/log error)
            TIFFReadBufferSetup(tif, NULL, 0);

            cleanup_tiff(tif);
        }
        // Clean up dummy file
        remove(filename);
    }

    return 0;
}