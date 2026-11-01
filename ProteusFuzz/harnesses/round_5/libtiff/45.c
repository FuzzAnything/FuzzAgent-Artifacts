// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test _TIFFmalloc */
    tmsize_t alloc_size = 0;
    if (Size > 0) {
        alloc_size = Data[0];
    }
    if (Size > 1) {
        alloc_size |= ((tmsize_t)Data[1] << 8);
    }
    /* Cap size to prevent OOM during fuzzing */
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    void *ptr_malloc = _TIFFmalloc(alloc_size);
    if (ptr_malloc) {
        _TIFFmemset(ptr_malloc, 0x55, alloc_size);
        _TIFFfree(ptr_malloc);
    }

    /* Test _TIFFcalloc */
    tmsize_t nmemb = 0, siz = 0;
    if (Size > 2) {
        nmemb = Data[2];
        siz = Data[3];
    }
    if (nmemb > 1000) nmemb = 1000;
    if (siz > 1000) siz = 1000;

    void *ptr_calloc = _TIFFcalloc(nmemb, siz);
    if (ptr_calloc) {
        _TIFFfree(ptr_calloc);
    }

    /* Test _TIFFrealloc */
    void *ptr_realloc = _TIFFmalloc(alloc_size > 0 ? alloc_size : 1);
    if (ptr_realloc) {
        tmsize_t new_size = alloc_size;
        if (Size > 4) {
            new_size = Data[4];
        }
        if (new_size > 1024 * 1024) new_size = 1024 * 1024;
        
        void *ptr_new = _TIFFrealloc(ptr_realloc, new_size);
        if (ptr_new) {
            _TIFFmemset(ptr_new, 0xAA, new_size);
            _TIFFfree(ptr_new);
        } else {
            _TIFFfree(ptr_realloc);
        }
    }

    /* Test _TIFFrealloc with NULL (acts like malloc) */
    void *ptr_null_realloc = _TIFFrealloc(NULL, alloc_size);
    if (ptr_null_realloc) {
        _TIFFfree(ptr_null_realloc);
    }

    /* Test _TIFFmemset directly */
    if (alloc_size > 0) {
        void *test_mem = _TIFFmalloc(alloc_size);
        if (test_mem) {
            int val = 0;
            if (Size > 5) val = Data[5];
            _TIFFmemset(test_mem, val, alloc_size);
            _TIFFfree(test_mem);
        }
    }

    /* Test TIFFReadBufferSetup and TIFFWriteBufferSetup */
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
    }

    if (tif) {
        void *custom_buf = NULL;
        tmsize_t buf_size = 0;

        if (Size > 6) {
            buf_size = Data[6];
            if (buf_size > 1024 * 1024) buf_size = 1024 * 1024;
            if (buf_size > 0) {
                custom_buf = _TIFFmalloc(buf_size);
            }
        }

        /* Try ReadBufferSetup */
        if (custom_buf) {
            TIFFReadBufferSetup(tif, custom_buf, buf_size);
        } else {
            TIFFReadBufferSetup(tif, NULL, 0);
        }

        /* Try WriteBufferSetup */
        if (custom_buf) {
            TIFFWriteBufferSetup(tif, custom_buf, buf_size);
        } else {
            TIFFWriteBufferSetup(tif, NULL, -1);
        }

        if (custom_buf) {
            _TIFFfree(custom_buf);
        }

        TIFFClose(tif);
    }

    return 0;
}