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
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "tiffio.h"

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

static toff_t dummy_seek(thandle_t handle, toff_t off, int whence) {
    (void)handle;
    (void)off;
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

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    const char* mode = "r";
    
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    lseek(fd, 0, SEEK_SET);
    
    int fd2 = dup(fd);
    if (fd2 < 0) {
        close(fd);
        return 0;
    }
    
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 1024 * 1024);
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 10 * 1024 * 1024);
    }
    
    TIFF* tiff1 = TIFFFdOpen(fd, filename, mode);
    if (tiff1) {
        int old_fd = TIFFSetFileno(tiff1, fd2);
        (void)old_fd;
        
        int current_fd = TIFFFileno(tiff1);
        (void)current_fd;
        
        TIFFClose(tiff1);
    } else {
        close(fd);
        close(fd2);
    }
    
    if (fd2 >= 0) {
        lseek(fd2, 0, SEEK_SET);
        TIFF* tiff2 = TIFFFdOpenExt(fd2, filename, mode, opts);
        if (tiff2) {
            TIFFClose(tiff2);
        } else {
            close(fd2);
        }
    }
    
    int fd3 = open(filename, O_RDONLY);
    if (fd3 >= 0) {
        lseek(fd3, 0, SEEK_SET);
        
        thandle_t handle = (thandle_t)(intptr_t)fd3;
        TIFF* tiff3 = TIFFClientOpen(filename, mode, handle,
                                    dummy_read, dummy_write,
                                    dummy_seek, dummy_close,
                                    dummy_size, dummy_map, dummy_unmap);
        if (tiff3) {
            TIFFClose(tiff3);
        } else {
            close(fd3);
        }
    }
    
    int fd4 = open(filename, O_RDONLY);
    if (fd4 >= 0) {
        lseek(fd4, 0, SEEK_SET);
        
        thandle_t handle = (thandle_t)(intptr_t)fd4;
        TIFF* tiff4 = TIFFClientOpenExt(filename, mode, handle,
                                       dummy_read, dummy_write,
                                       dummy_seek, dummy_close,
                                       dummy_size, dummy_map, dummy_unmap,
                                       opts);
        if (tiff4) {
            TIFFClose(tiff4);
        } else {
            close(fd4);
        }
    }
    
    if (opts) {
        TIFFOpenOptionsFree(opts);
    }
    
    unlink(filename);
    return 0;
}