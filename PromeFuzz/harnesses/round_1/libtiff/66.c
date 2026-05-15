// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static tmsize_t dummyReadProc(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    return size;
}

static tmsize_t dummyWriteProc(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    return size;
}

static toff_t dummySeekProc(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummyCloseProc(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummySizeProc(thandle_t handle) {
    (void)handle;
    return 0;
}

static int dummyMapProc(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void dummyUnmapProc(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* dummy_filename = "./dummy_file";
    int fd = open(dummy_filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    lseek(fd, 0, SEEK_SET);
    
    // Test TIFFFdOpenExt
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFF* tif1 = TIFFFdOpenExt(fd, dummy_filename, "r", opts);
        if (tif1) {
            // Test TIFFFileno
            int ret_fd = TIFFFileno(tif1);
            (void)ret_fd;
            
            // Test TIFFSetFileno
            int old_fd = TIFFSetFileno(tif1, fd);
            (void)old_fd;
            
            TIFFClose(tif1);
        }
        TIFFOpenOptionsFree(opts);
    }
    
    // Test TIFFFdOpen
    lseek(fd, 0, SEEK_SET);
    TIFF* tif2 = TIFFFdOpen(fd, dummy_filename, "r");
    if (tif2) {
        TIFFClose(tif2);
    }
    
    // Test TIFFClientOpen
    lseek(fd, 0, SEEK_SET);
    TIFF* tif3 = TIFFClientOpen(dummy_filename, "r", (thandle_t)(intptr_t)fd,
                                dummyReadProc, dummyWriteProc,
                                dummySeekProc, dummyCloseProc,
                                dummySizeProc, dummyMapProc,
                                dummyUnmapProc);
    if (tif3) {
        TIFFClose(tif3);
    }
    
    // Test TIFFClientOpenExt
    lseek(fd, 0, SEEK_SET);
    TIFFOpenOptions* opts2 = TIFFOpenOptionsAlloc();
    if (opts2) {
        TIFF* tif4 = TIFFClientOpenExt(dummy_filename, "r", (thandle_t)(intptr_t)fd,
                                       dummyReadProc, dummyWriteProc,
                                       dummySeekProc, dummyCloseProc,
                                       dummySizeProc, dummyMapProc,
                                       dummyUnmapProc, opts2);
        if (tif4) {
            TIFFClose(tif4);
        }
        TIFFOpenOptionsFree(opts2);
    }
    
    close(fd);
    unlink(dummy_filename);
    
    return 0;
}