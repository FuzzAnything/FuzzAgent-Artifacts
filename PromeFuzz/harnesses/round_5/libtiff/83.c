// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
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
    return size;
}

static uint64_t dummy_seek(thandle_t handle, uint64_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static uint64_t dummy_size(thandle_t handle) {
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

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    const char* filename = "./dummy_file";
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        return 0;
    }
    
    thandle_t handle = (thandle_t)(intptr_t)fd;
    
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetErrorHandlerExtR(opts, NULL, NULL);
        TIFFOpenOptionsSetWarningHandlerExtR(opts, NULL, NULL);
    }
    
    TIFF* tif = TIFFClientOpenExt("dummy", "r", handle,
                                  dummy_read, dummy_write,
                                  dummy_seek, dummy_close,
                                  dummy_size, dummy_map, dummy_unmap,
                                  opts);
    
    if (tif) {
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        
        (void)write_proc;
        (void)size_proc;
        (void)seek_proc;
        (void)read_proc;
        
        TIFFClose(tif);
    }
    
    close(fd);
    
    fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        handle = (thandle_t)(intptr_t)fd;
        
        TIFF* tif2 = TIFFClientOpen("dummy2", "r", handle,
                                    dummy_read, dummy_write,
                                    dummy_seek, dummy_close,
                                    dummy_size, dummy_map, dummy_unmap);
        
        if (tif2) {
            TIFFReadWriteProc write_proc2 = TIFFGetWriteProc(tif2);
            TIFFSizeProc size_proc2 = TIFFGetSizeProc(tif2);
            TIFFSeekProc seek_proc2 = TIFFGetSeekProc(tif2);
            TIFFReadWriteProc read_proc2 = TIFFGetReadProc(tif2);
            
            (void)write_proc2;
            (void)size_proc2;
            (void)seek_proc2;
            (void)read_proc2;
            
            TIFFClose(tif2);
        }
        
        close(fd);
    }
    
    if (opts) {
        TIFFOpenOptionsFree(opts);
    }
    
    unlink(filename);
    
    return 0;
}