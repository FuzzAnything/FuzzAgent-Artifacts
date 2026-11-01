// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

/* 
 * Fuzz driver for libtiff memory management and buffer setup functions.
 * Targets: _TIFFcalloc, _TIFFrealloc, _TIFFmalloc, TIFFReadBufferSetup, 
 * TIFFWriteBufferSetup, _TIFFmemset.
 */

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Test 1: Direct memory allocation functions (_TIFFmalloc, _TIFFcalloc, _TIFFrealloc, _TIFFmemset)
     * These do not require a valid TIFF handle.
     */
    
    /* Extract parameters from fuzz input for allocation sizes */
    tmsize_t nmemb = 0;
    tmsize_t siz = 0;
    tmsize_t alloc_size = 0;
    tmsize_t realloc_size = 0;
    int memset_val = 0;
    tmsize_t memset_count = 0;

    if (Size >= 2) {
        nmemb = (tmsize_t)Data[0];
        siz = (tmsize_t)Data[1];
    }
    if (Size >= 4) {
        alloc_size = (tmsize_t)Data[2] | ((tmsize_t)Data[3] << 8);
    }
    if (Size >= 6) {
        realloc_size = (tmsize_t)Data[4] | ((tmsize_t)Data[5] << 8);
    }
    if (Size >= 8) {
        memset_val = (int)Data[6];
        memset_count = (tmsize_t)Data[7] | ((tmsize_t)Data[8] << 8);
    }

    /* Test _TIFFcalloc */
    void *ptr_calloc = _TIFFcalloc(nmemb, siz);
    if (ptr_calloc != NULL) {
        /* Test _TIFFmemset on allocated memory */
        if (nmemb > 0 && siz > 0) {
            _TIFFmemset(ptr_calloc, memset_val, nmemb * siz);
        }
        _TIFFfree(ptr_calloc);
    }

    /* Test _TIFFmalloc */
    void *ptr_malloc = _TIFFmalloc(alloc_size);
    if (ptr_malloc != NULL) {
        /* Test _TIFFmemset on malloc'd memory */
        if (alloc_size > 0) {
            _TIFFmemset(ptr_malloc, memset_val, alloc_size);
        }
        /* Test _TIFFrealloc */
        void *ptr_realloc = _TIFFrealloc(ptr_malloc, realloc_size);
        if (ptr_realloc != NULL) {
            /* Use the new pointer, old one is invalid */
            if (realloc_size > 0) {
                _TIFFmemset(ptr_realloc, memset_val, realloc_size);
            }
            _TIFFfree(ptr_realloc);
        } else {
            /* If realloc failed, original pointer is still valid */
            _TIFFfree(ptr_malloc);
        }
    }

    /* Test _TIFFrealloc with NULL (acts like malloc) */
    void *ptr_realloc_null = _TIFFrealloc(NULL, alloc_size);
    if (ptr_realloc_null != NULL) {
        if (alloc_size > 0) {
            _TIFFmemset(ptr_realloc_null, memset_val, alloc_size);
        }
        _TIFFfree(ptr_realloc_null);
    }

    /* Test _TIFFrealloc with size 0 (acts like free) */
    void *ptr_free_test = _TIFFmalloc(alloc_size);
    if (ptr_free_test != NULL) {
        _TIFFrealloc(ptr_free_test, 0);
    }

    /* 
     * Test 2: Buffer Setup functions (TIFFReadBufferSetup, TIFFWriteBufferSetup)
     * These require a valid TIFF handle. We create a dummy file to open.
     */
    
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        /* Write minimal fuzz data to file to make it openable */
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    TIFF *tif = TIFFOpen(dummy_file, "rb");
    
    /* If read failed, try opening in write mode to test WriteBufferSetup */
    if (tif == NULL) {
        tif = TIFFOpen(dummy_file, "wb");
    }

    if (tif != NULL) {
        /* Prepare custom buffer for setup tests */
        tmsize_t custom_buf_size = (Size > 100) ? Size - 100 : Size;
        uint8_t *custom_buf = NULL;
        
        if (custom_buf_size > 0) {
            custom_buf = _TIFFmalloc(custom_buf_size);
            if (custom_buf != NULL) {
                _TIFFmemset(custom_buf, 0xAA, custom_buf_size);
            }
        }

        /* Test TIFFReadBufferSetup */
        /* Case A: Custom buffer */
        if (custom_buf != NULL && custom_buf_size > 0) {
            TIFFReadBufferSetup(tif, custom_buf, custom_buf_size);
        }
        /* Case B: NULL buffer (let libtiff allocate) */
        TIFFReadBufferSetup(tif, NULL, 0);
        /* Case C: Zero size (should fail/log error) */
        TIFFReadBufferSetup(tif, custom_buf, 0);

        /* Test TIFFWriteBufferSetup */
        /* Case A: Custom buffer */
        if (custom_buf != NULL && custom_buf_size > 0) {
            TIFFWriteBufferSetup(tif, custom_buf, custom_buf_size);
        }
        /* Case B: NULL buffer (let libtiff allocate) */
        TIFFWriteBufferSetup(tif, NULL, -1);
        /* Case C: Zero size (behavior depends on implementation, often frees) */
        TIFFWriteBufferSetup(tif, custom_buf, 0);

        /* Cleanup custom buffer if we still own it (TIFFWriteBufferSetup might have taken ownership or freed it) */
        /* Note: libtiff behavior on buffer ownership varies. 
           If we passed a custom buffer to WriteBufferSetup, libtiff might free it on close.
           To be safe, we don't free custom_buf here if we passed it to setup functions that might own it.
           However, for ReadBufferSetup, libtiff usually doesn't take ownership of the pointer unless specified.
           Given the complexity, we rely on TIFFClose to handle internal buffers.
           We will NOT free custom_buf here to avoid double-free if libtiff took it.
           Instead, we let TIFFClose handle cleanup of the TIFF structure.
        */
        
        cleanup_tiff(tif);
    }

    /* Clean up dummy file */
    unlink(dummy_file);

    return 0;
}