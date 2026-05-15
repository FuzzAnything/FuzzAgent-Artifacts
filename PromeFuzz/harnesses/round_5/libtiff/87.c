// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) == 0) {
        return ftell(file);
    }
    return (toff_t)-1;
}

static int dummy_close(thandle_t handle) {
    FILE* file = (FILE*)handle;
    return fclose(file);
}

static toff_t dummy_size(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

static int dummy_map(thandle_t handle, void** base, toff_t* offset) {
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t offset) {
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    dummy_file = fopen("./dummy_file", "rb");
    if (!dummy_file) {
        return 0;
    }
    
    TIFF* tif = TIFFClientOpen("dummy", "r", (thandle_t)dummy_file,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, dummy_map, dummy_unmap);
    
    if (tif) {
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        
        TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
        if (opts) {
            TIFF* tif2 = TIFFClientOpenExt("dummy2", "r", (thandle_t)dummy_file,
                                          dummy_read, dummy_write,
                                          dummy_seek, dummy_close,
                                          dummy_size, dummy_map, dummy_unmap,
                                          opts);
            if (tif2) {
                TIFFClose(tif2);
            }
            TIFFOpenOptionsFree(opts);
        }
        
        TIFFClose(tif);
    } else {
        fclose(dummy_file);
    }
    
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        TIFF* tif3 = TIFFClientOpen("dummy3", "r", (thandle_t)dummy_file,
                                   dummy_read, dummy_write,
                                   dummy_seek, dummy_close,
                                   dummy_size, dummy_map, dummy_unmap);
        if (tif3) {
            TIFFClose(tif3);
        } else {
            fclose(dummy_file);
        }
    }
    
    remove("./dummy_file");
    return 0;
}