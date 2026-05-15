// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetCloseProc at tif_open.c:927:15 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    (void)handle;
    return 0;
}

static int dummy_map(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    dummy_file = fopen("./dummy_file", "rb");
    if (!dummy_file) return 0;
    
    thandle_t handle = (thandle_t)dummy_file;
    
    TIFF* tif = TIFFClientOpen("dummy", "r", handle,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, dummy_map, dummy_unmap);
    
    if (tif) {
        TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
        if (size_proc) {
            toff_t file_size = size_proc(handle);
            (void)file_size;
        }
        
        TIFFCloseProc close_proc = TIFFGetCloseProc(tif);
        if (close_proc) {
            int close_result = close_proc(handle);
            (void)close_result;
        }
        
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        if (read_proc) {
            char buffer[16];
            tmsize_t read_result = read_proc(handle, buffer, sizeof(buffer));
            (void)read_result;
        }
        
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        if (map_proc) {
            void* base;
            toff_t map_size;
            int map_result = map_proc(handle, &base, &map_size);
            (void)map_result;
        }
        
        TIFF* tif_ext = TIFFClientOpenExt("dummy_ext", "r", handle,
                                         dummy_read, dummy_write,
                                         dummy_seek, dummy_close,
                                         dummy_size, dummy_map, dummy_unmap,
                                         NULL);
        if (tif_ext) {
            TIFFClose(tif_ext);
        }
        
        TIFFClose(tif);
    } else {
        fclose(dummy_file);
    }
    
    remove("./dummy_file");
    return 0;
}