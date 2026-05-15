// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return -1;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return -1;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    (void)handle;
    return 0;
}

static int dummy_map(thandle_t handle, void** base, toff_t* offset) {
    (void)handle;
    (void)base;
    (void)offset;
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t offset) {
    (void)handle;
    (void)base;
    (void)offset;
}

static TIFF* create_dummy_tiff(void) {
    TIFF* tif = TIFFClientOpen("dummy", "w", (thandle_t)1,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, dummy_map, dummy_unmap);
    return tif;
}

static void test_vsetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

static void test_vgetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVGetField(tif, tag, ap);
    va_end(ap);
}

static void test_vgetfielddefaulted(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_dummy_tiff();
    if (!tif) return 0;
    
    uint32_t tag;
    memcpy(&tag, Data, 4);
    Data += 4;
    Size -= 4;
    
    if (Size > 0) {
        TIFFDataType dtype = TIFF_NOTYPE;
        if (Size >= 1) {
            dtype = (TIFFDataType)(Data[0] % 13);
        }
        
        const TIFFField* field1 = TIFFFindField(tif, tag, dtype);
        const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
        
        if (field1 || field2) {
            uint32_t dummy_value = 0;
            if (Size >= 8) {
                memcpy(&dummy_value, Data + 1, 4);
            }
            
            test_vsetfield(tif, tag, dummy_value);
            
            uint32_t get_value;
            test_vgetfield(tif, tag, &get_value);
            
            uint32_t get_defaulted_value;
            test_vgetfielddefaulted(tif, tag, &get_defaulted_value);
            
            if (Size >= 12) {
                uint32_t default_tag;
                memcpy(&default_tag, Data + 5, 4);
                
                uint32_t default_value = 0;
                int result = TIFFGetFieldDefaulted(tif, default_tag, &default_value);
                (void)result;
            }
        }
    }
    
    TIFFClose(tif);
    return 0;
}