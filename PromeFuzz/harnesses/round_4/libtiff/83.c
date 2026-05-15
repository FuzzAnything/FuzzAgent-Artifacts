// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

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

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    lseek(fd, 0, SEEK_SET);
    
    uint8_t mode_selector = Data[0] % 4;
    const char* mode = (mode_selector == 0) ? "r" : 
                      (mode_selector == 1) ? "w" : 
                      (mode_selector == 2) ? "a" : "r+";
    
    TIFFOpenOptions* opts = NULL;
    if (Size > 10 && (Data[1] % 2)) {
        opts = TIFFOpenOptionsAlloc();
        if (opts) {
            TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 1024 * 1024);
            TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 10 * 1024 * 1024);
        }
    }
    
    TIFF* tif1 = TIFFFdOpenExt(fd, filename, mode, opts);
    if (tif1) {
        int fileno1 = TIFFFileno(tif1);
        (void)fileno1;
        
        int dummy_fd = open("/dev/null", O_RDONLY);
        if (dummy_fd >= 0) {
            int prev_fd = TIFFSetFileno(tif1, dummy_fd);
            (void)prev_fd;
            close(dummy_fd);
        }
        
        TIFFClose(tif1);
    }
    
    lseek(fd, 0, SEEK_SET);
    
    TIFF* tif2 = TIFFFdOpen(fd, filename, mode);
    if (tif2) {
        int fileno2 = TIFFFileno(tif2);
        (void)fileno2;
        TIFFClose(tif2);
    }
    
    close(fd);
    
    thandle_t client_data = (thandle_t)(uintptr_t)Data;
    TIFF* tif3 = TIFFClientOpen("memory", mode, client_data,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, dummy_map, dummy_unmap);
    if (tif3) {
        TIFFClose(tif3);
    }
    
    TIFF* tif4 = TIFFClientOpenExt("memory_ext", mode, client_data,
                                  dummy_read, dummy_write,
                                  dummy_seek, dummy_close,
                                  dummy_size, dummy_map, dummy_unmap,
                                  opts);
    if (tif4) {
        TIFFClose(tif4);
    }
    
    if (opts) {
        TIFFOpenOptionsFree(opts);
    }
    
    unlink(filename);
    return 0;
}