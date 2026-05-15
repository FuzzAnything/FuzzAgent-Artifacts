// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "libtiff/tiffio.h"

static tmsize_t read_tmsize_t(const uint8_t **data, const uint8_t *end) {
    if (*data + sizeof(tmsize_t) > end) {
        return 0;
    }
    tmsize_t value;
    memcpy(&value, *data, sizeof(tmsize_t));
    *data += sizeof(tmsize_t);
    return value;
}

static int read_int(const uint8_t **data, const uint8_t *end) {
    if (*data + sizeof(int) > end) {
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    return value;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const uint8_t *ptr = Data;
    const uint8_t *end = Data + Size;

    // Use first byte to determine operation mix
    uint8_t op_seed = *ptr++;
    
    // Test _TIFFmalloc and _TIFFfree
    if (op_seed & 0x01) {
        tmsize_t alloc_size = read_tmsize_t(&ptr, end);
        void *mem = _TIFFmalloc(alloc_size);
        if (mem) {
            _TIFFfree(mem);
        }
    }

    // Test _TIFFcalloc
    if (op_seed & 0x02) {
        tmsize_t nmemb = read_tmsize_t(&ptr, end);
        tmsize_t size = read_tmsize_t(&ptr, end);
        void *mem = _TIFFcalloc(nmemb, size);
        if (mem) {
            _TIFFfree(mem);
        }
    }

    // Test _TIFFrealloc
    if (op_seed & 0x04) {
        tmsize_t initial_size = read_tmsize_t(&ptr, end);
        tmsize_t new_size = read_tmsize_t(&ptr, end);
        
        void *initial_mem = _TIFFmalloc(initial_size);
        if (initial_mem) {
            void *realloc_mem = _TIFFrealloc(initial_mem, new_size);
            if (realloc_mem) {
                // If realloc succeeded, the original pointer was either freed or
                // moved to the new location. We should only free the new pointer.
                _TIFFfree(realloc_mem);
            } else {
                // If realloc failed, the original pointer is still valid
                // and needs to be freed.
                _TIFFfree(initial_mem);
            }
        }
    }

    // Test _TIFFmemcpy
    if (op_seed & 0x08) {
        tmsize_t copy_size = read_tmsize_t(&ptr, end);
        if (copy_size > 0 && copy_size < 1024) { // Limit size for safety
            void *src = _TIFFmalloc(copy_size);
            void *dst = _TIFFmalloc(copy_size);
            
            if (src && dst) {
                // Initialize source with some data from fuzzer input
                size_t init_size = copy_size < (size_t)(end - ptr) ? copy_size : (size_t)(end - ptr);
                if (init_size > 0) {
                    memcpy(src, ptr, init_size);
                    ptr += init_size;
                }
                
                _TIFFmemcpy(dst, src, copy_size);
            }
            
            if (src) _TIFFfree(src);
            if (dst) _TIFFfree(dst);
        }
    }

    // Test _TIFFmemset
    if (op_seed & 0x10) {
        tmsize_t set_size = read_tmsize_t(&ptr, end);
        int set_value = read_int(&ptr, end);
        
        if (set_size > 0 && set_size < 1024) { // Limit size for safety
            void *mem = _TIFFmalloc(set_size);
            if (mem) {
                _TIFFmemset(mem, set_value, set_size);
                _TIFFfree(mem);
            }
        }
    }

    // Test combination of operations
    if (op_seed & 0x20) {
        tmsize_t size1 = read_tmsize_t(&ptr, end);
        tmsize_t size2 = read_tmsize_t(&ptr, end);
        
        void *mem1 = _TIFFmalloc(size1);
        void *mem2 = _TIFFcalloc(1, size2);
        
        if (mem1 && mem2 && size1 > 0 && size2 > 0) {
            // Copy from mem2 to mem1 (limited by smaller size)
            tmsize_t copy_size = size1 < size2 ? size1 : size2;
            _TIFFmemcpy(mem1, mem2, copy_size);
            
            // Set mem2 to a value
            _TIFFmemset(mem2, 0xFF, size2);
            
            // Reallocate mem1
            void *mem3 = _TIFFrealloc(mem1, size2);
            if (mem3) {
                _TIFFfree(mem3);
                // mem1 is now invalid (either freed or moved)
                mem1 = NULL;
            } else {
                // realloc failed, mem1 is still valid
                // Keep it for cleanup below
            }
        }
        
        if (mem1) _TIFFfree(mem1);
        if (mem2) _TIFFfree(mem2);
    }

    return 0;
}