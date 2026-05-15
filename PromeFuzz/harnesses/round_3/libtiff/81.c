// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSizeProc at tif_open.c:932:14 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
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

/* Helper to create a dummy TIFF file for testing */
static int create_dummy_file(const uint8_t* data, size_t size) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    /* Write minimal TIFF header if we have enough data */
    if (size >= 8) {
        fwrite(data, 1, size > 1024 ? 1024 : size, fp);
    } else {
        /* Write a minimal valid TIFF header */
        uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, sizeof(header), fp);
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
    }
    
    fclose(fp);
    return 1;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    /* Create dummy file for testing */
    if (!create_dummy_file(Data, Size)) {
        return 0;
    }
    
    /* Test 1: TIFFClientOpen with dummy callbacks */
    thandle_t dummy_handle = (thandle_t)0xdeadbeef;
    TIFF* tif = TIFFClientOpen("dummy", "r", dummy_handle,
                              dummy_read, dummy_write,
                              dummy_seek, dummy_close,
                              dummy_size, dummy_map, dummy_unmap);
    
    if (tif) {
        /* Test 2: TIFFGetSeekProc */
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        if (seek_proc) {
            seek_proc(dummy_handle, 0, SEEK_SET);
        }
        
        /* Test 3: TIFFGetWriteProc */
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        if (write_proc) {
            uint8_t buf[16];
            write_proc(dummy_handle, buf, sizeof(buf));
        }
        
        /* Test 4: TIFFGetReadProc */
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        if (read_proc) {
            uint8_t buf[16];
            read_proc(dummy_handle, buf, sizeof(buf));
        }
        
        /* Test 5: TIFFGetSizeProc */
        TIFFSizeProc size_proc = TIFFGetSizeProc(tif);
        if (size_proc) {
            size_proc(dummy_handle);
        }
        
        /* Test 6: TIFFClientOpenExt with options */
        TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
        if (opts) {
            TIFF* tif2 = TIFFClientOpenExt("dummy2", "r", dummy_handle,
                                          dummy_read, dummy_write,
                                          dummy_seek, dummy_close,
                                          dummy_size, dummy_map, dummy_unmap,
                                          opts);
            if (tif2) {
                /* Test the same functions on the second TIFF handle */
                TIFFGetSeekProc(tif2);
                TIFFGetWriteProc(tif2);
                TIFFGetReadProc(tif2);
                TIFFGetSizeProc(tif2);
                TIFFClose(tif2);
            }
            TIFFOpenOptionsFree(opts);
        }
        
        TIFFClose(tif);
    }
    
    /* Also test with a regular file-based TIFF */
    TIFF* file_tif = TIFFOpen("./dummy_file", "r");
    if (file_tif) {
        TIFFGetSeekProc(file_tif);
        TIFFGetWriteProc(file_tif);
        TIFFGetReadProc(file_tif);
        TIFFGetSizeProc(file_tif);
        TIFFClose(file_tif);
    }
    
    /* Clean up */
    unlink("./dummy_file");
    
    return 0;
}