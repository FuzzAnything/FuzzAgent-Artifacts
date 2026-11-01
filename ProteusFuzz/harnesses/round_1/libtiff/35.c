// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
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
 * Target functions:
 * - _TIFFrealloc
 * - _TIFFcalloc
 * - _TIFFmalloc
 * - TIFFReadBufferSetup
 * - _TIFFmemcpy
 * - _TIFFmemset
 */

/*
 * Helper to extract a tmsize_t from the fuzz input safely.
 * We clamp the value to prevent integer overflow or allocation of
 * excessively large blocks that could crash the fuzzer or OOM.
 */
static tmsize_t get_safe_size(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(tmsize_t) > Size) {
        return 0;
    }
    tmsize_t val;
    memcpy(&val, Data + *offset, sizeof(tmsize_t));
    *offset += sizeof(tmsize_t);

    /* Clamp to a reasonable maximum to avoid OOM */
    if (val < 0) {
        val = 0;
    } else if (val > 1024 * 1024) { /* 1MB limit */
        val = 1024 * 1024;
    }
    return val;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;
    int op = 0;

    /* Read operation type if available */
    if (Size > 0) {
        op = Data[0] % 6;
        offset = 1;
    }

    void *ptr = NULL;
    void *new_ptr = NULL;
    TIFF *tif = NULL;
    uint8_t *custom_buf = NULL;

    switch (op) {
        case 0: {
            /* Test _TIFFmalloc */
            tmsize_t size = get_safe_size(Data, Size, &offset);
            ptr = _TIFFmalloc(size);
            if (ptr != NULL) {
                /* Exercise _TIFFmemset on allocated memory */
                if (Size > offset) {
                    int val = Data[offset];
                    _TIFFmemset(ptr, val, size);
                }
                _TIFFfree(ptr);
            }
            break;
        }
        case 1: {
            /* Test _TIFFcalloc */
            tmsize_t nmemb = get_safe_size(Data, Size, &offset);
            tmsize_t siz = get_safe_size(Data, Size, &offset);
            ptr = _TIFFcalloc(nmemb, siz);
            if (ptr != NULL) {
                /* Exercise _TIFFmemcpy on allocated memory */
                tmsize_t copy_len = get_safe_size(Data, Size, &offset);
                if (copy_len > 0 && copy_len <= nmemb * siz) {
                    _TIFFmemset(ptr, 0x42, copy_len);
                    _TIFFmemcpy(ptr, ptr, copy_len);
                }
                _TIFFfree(ptr);
            }
            break;
        }
        case 2: {
            /* Test _TIFFrealloc */
            tmsize_t init_size = get_safe_size(Data, Size, &offset);
            ptr = _TIFFmalloc(init_size);
            if (ptr != NULL) {
                tmsize_t new_size = get_safe_size(Data, Size, &offset);
                new_ptr = _TIFFrealloc(ptr, new_size);
                if (new_ptr != NULL) {
                    /* Exercise _TIFFmemset on reallocated memory */
                    if (new_size > 0) {
                        int val = (Size > offset) ? Data[offset] : 0;
                        _TIFFmemset(new_ptr, val, new_size);
                    }
                    _TIFFfree(new_ptr);
                } else {
                    /* realloc failed, original ptr is still valid */
                    _TIFFfree(ptr);
                }
            } else {
                /* Test realloc with NULL (acts like malloc) */
                tmsize_t new_size = get_safe_size(Data, Size, &offset);
                new_ptr = _TIFFrealloc(NULL, new_size);
                if (new_ptr != NULL) {
                    _TIFFfree(new_ptr);
                }
            }
            break;
        }
        case 3: {
            /* Test TIFFReadBufferSetup */
            /* Create a dummy file */
            FILE *f = fopen("./dummy_file", "wb");
            if (f != NULL) {
                /* Write minimal TIFF header (4 bytes) to make it openable */
                uint8_t header[4] = {0x49, 0x49, 0x2A, 0x00}; /* II*0 */
                fwrite(header, 1, 4, f);
                fclose(f);

                tif = TIFFOpen("./dummy_file", "r");
                if (tif != NULL) {
                    tmsize_t buf_size = get_safe_size(Data, Size, &offset);
                    if (buf_size > 0) {
                        custom_buf = (uint8_t *)_TIFFmalloc(buf_size);
                        if (custom_buf != NULL) {
                            /* Initialize buffer */
                            _TIFFmemset(custom_buf, 0, buf_size);
                            /* Setup read buffer */
                            TIFFReadBufferSetup(tif, custom_buf, buf_size);
                            _TIFFfree(custom_buf);
                        }
                    }
                    /* Test with NULL buffer (let libtiff allocate) */
                    TIFFReadBufferSetup(tif, NULL, 0);
                    TIFFClose(tif);
                }
                unlink("./dummy_file");
            }
            break;
        }
        case 4: {
            /* Test _TIFFmemcpy and _TIFFmemset independently */
            tmsize_t size = get_safe_size(Data, Size, &offset);
            if (size > 0) {
                ptr = _TIFFmalloc(size);
                if (ptr != NULL) {
                    _TIFFmemset(ptr, 0xAA, size);
                    /* Copy to itself */
                    _TIFFmemcpy(ptr, ptr, size);
                    /* Copy from a static buffer if size allows */
                    static const uint8_t src[] = "FuzzTest";
                    tmsize_t copy_len = (size < sizeof(src)) ? size : sizeof(src);
                    _TIFFmemcpy(ptr, src, copy_len);
                    _TIFFfree(ptr);
                }
            }
            break;
        }
        case 5: {
            /* Mixed operations */
            tmsize_t s1 = get_safe_size(Data, Size, &offset);
            tmsize_t s2 = get_safe_size(Data, Size, &offset);
            
            ptr = _TIFFmalloc(s1);
            if (ptr != NULL) {
                _TIFFmemset(ptr, 0, s1);
                
                new_ptr = _TIFFrealloc(ptr, s2);
                if (new_ptr != NULL) {
                    _TIFFmemset(new_ptr, 0xFF, s2);
                    
                    /* Try calloc and memcpy */
                    void *c_ptr = _TIFFcalloc(s1, 1);
                    if (c_ptr != NULL) {
                        _TIFFmemcpy(c_ptr, new_ptr, (s1 < s2) ? s1 : s2);
                        _TIFFfree(c_ptr);
                    }
                    
                    _TIFFfree(new_ptr);
                } else {
                    _TIFFfree(ptr);
                }
            }
            break;
        }
    }

    return 0;
}