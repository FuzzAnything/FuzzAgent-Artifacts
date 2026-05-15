// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
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

/* Dummy callbacks for TIFFClientOpenExt */
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

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    /* Write fuzzer data to dummy file */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    /* Test TIFFOpen with different modes */
    const char* modes[] = {"r", "w", "a"};
    int mode_idx = Data[0] % 3;
    
    TIFF* tif = TIFFOpen("./dummy_file", modes[mode_idx]);
    if (tif) {
        /* Test TIFFClientdata */
        thandle_t clientdata = TIFFClientdata(tif);
        (void)clientdata;
        
        /* Test TIFFSetClientdata and TIFFSetClientdata return value */
        thandle_t dummy_handle = (thandle_t)(uintptr_t)Data[1];
        thandle_t prev = TIFFSetClientdata(tif, dummy_handle);
        (void)prev;
        
        /* Test TIFFSetClientInfo and TIFFGetClientInfo */
        if (Size > 10) {
            char name_buf[10];
            memcpy(name_buf, Data + 2, 9);
            name_buf[9] = '\0';
            
            void* user_data = (void*)(uintptr_t)Data[10];
            TIFFSetClientInfo(tif, user_data, name_buf);
            
            void* retrieved = TIFFGetClientInfo(tif, name_buf);
            (void)retrieved;
            
            /* Test with another name */
            if (Size > 20) {
                char name_buf2[10];
                memcpy(name_buf2, Data + 11, 9);
                name_buf2[9] = '\0';
                void* retrieved2 = TIFFGetClientInfo(tif, name_buf2);
                (void)retrieved2;
            }
        }
        
        TIFFClose(tif);
    }
    
    /* Test TIFFClientOpenExt */
    if (Size > 30) {
        TIFFOpenOptions* opts = NULL;
        thandle_t ext_handle = (thandle_t)(uintptr_t)Data[20];
        
        TIFF* tif2 = TIFFClientOpenExt("dummy", 
                                      modes[mode_idx],
                                      ext_handle,
                                      dummy_read,
                                      dummy_write,
                                      dummy_seek,
                                      dummy_close,
                                      dummy_size,
                                      dummy_map,
                                      dummy_unmap,
                                      opts);
        if (tif2) {
            /* Exercise client functions on this handle too */
            thandle_t cd = TIFFClientdata(tif2);
            (void)cd;
            
            if (Size > 40) {
                char name_buf3[10];
                memcpy(name_buf3, Data + 30, 9);
                name_buf3[9] = '\0';
                void* data3 = (void*)(uintptr_t)Data[40];
                TIFFSetClientInfo(tif2, data3, name_buf3);
            }
            
            TIFFClose(tif2);
        }
    }
    
    /* Clean up */
    unlink("./dummy_file");
    
    return 0;
}