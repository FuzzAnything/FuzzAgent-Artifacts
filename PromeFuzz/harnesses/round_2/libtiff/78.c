// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* Custom I/O callbacks */
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

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    /* Create a dummy file with fuzzer data */
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    /* Test TIFFClientOpen and TIFFClientOpenExt */
    TIFF* tif = NULL;
    thandle_t dummy_handle = (thandle_t)0xdeadbeef;
    
    /* Try to open with TIFFClientOpen */
    tif = TIFFClientOpen("dummy", "r", dummy_handle,
                         dummy_read, dummy_write,
                         dummy_seek, dummy_close,
                         dummy_size, dummy_map, dummy_unmap);
    
    if (tif) {
        /* Test getter functions */
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
        
        /* Call the procedures with dummy parameters */
        if (read_proc) {
            char buf[16];
            read_proc(dummy_handle, buf, sizeof(buf));
        }
        if (write_proc) {
            char buf[16] = {0};
            write_proc(dummy_handle, buf, sizeof(buf));
        }
        if (seek_proc) {
            seek_proc(dummy_handle, 0, SEEK_SET);
        }
        if (size_proc) {
            size_proc(dummy_handle);
        }
        
        TIFFClose(tif);
    }
    
    /* Test TIFFClientOpenExt with NULL options */
    tif = TIFFClientOpenExt("dummy_ext", "w", dummy_handle,
                            dummy_read, dummy_write,
                            dummy_seek, dummy_close,
                            dummy_size, dummy_map, dummy_unmap,
                            NULL);
    
    if (tif) {
        /* Test getter functions again */
        TIFFGetReadProc(tif);
        TIFFGetWriteProc(tif);
        TIFFGetSeekProc(tif);
        TIFFGetSizeProc(tif);
        
        TIFFClose(tif);
    }
    
    /* Clean up dummy file */
    unlink("./dummy_file");
    
    return 0;
}