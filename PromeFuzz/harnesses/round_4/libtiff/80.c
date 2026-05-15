// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
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

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    int fd = open(filename, O_RDWR);
    if (fd < 0) return 0;
    
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 1024 * 1024);
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 10 * 1024 * 1024);
    }
    
    uint8_t mode_selector = Data[0] % 4;
    const char* mode = (mode_selector == 0) ? "r" : 
                      (mode_selector == 1) ? "w" : 
                      (mode_selector == 2) ? "a" : "r+";
    
    TIFF* tif = NULL;
    
    switch (Data[0] % 6) {
        case 0:
            tif = TIFFFdOpenExt(fd, filename, mode, opts);
            break;
        case 1:
            tif = TIFFClientOpen(filename, mode, (thandle_t)(intptr_t)fd,
                                 dummy_read, dummy_write,
                                 dummy_seek, dummy_close,
                                 dummy_size, dummy_map, dummy_unmap);
            break;
        case 2:
            if (tif) {
                int new_fd = TIFFFileno(tif);
                (void)new_fd;
            }
            break;
        case 3:
            tif = TIFFFdOpen(fd, filename, mode);
            break;
        case 4:
            if (tif) {
                int old_fd = TIFFSetFileno(tif, fd);
                (void)old_fd;
            }
            break;
        case 5:
            tif = TIFFClientOpenExt(filename, mode, (thandle_t)(intptr_t)fd,
                                    dummy_read, dummy_write,
                                    dummy_seek, dummy_close,
                                    dummy_size, dummy_map, dummy_unmap,
                                    opts);
            break;
    }
    
    if (tif) {
        int current_fd = TIFFFileno(tif);
        (void)current_fd;
        
        int test_fd = dup(fd);
        if (test_fd >= 0) {
            int old_fd = TIFFSetFileno(tif, test_fd);
            (void)old_fd;
            close(test_fd);
        }
        
        TIFFClose(tif);
    }
    
    if (opts) {
        TIFFOpenOptionsFree(opts);
    }
    
    close(fd);
    unlink(filename);
    
    return 0;
}