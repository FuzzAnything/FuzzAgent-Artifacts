// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* Custom I/O handlers for TIFFClientOpen */
static tmsize_t custom_read(thandle_t handle, void* buf, tmsize_t size) {
    int fd = *(int*)handle;
    return read(fd, buf, size);
}

static tmsize_t custom_write(thandle_t handle, void* buf, tmsize_t size) {
    int fd = *(int*)handle;
    return write(fd, buf, size);
}

static toff_t custom_seek(thandle_t handle, toff_t offset, int whence) {
    int fd = *(int*)handle;
    return lseek(fd, offset, whence);
}

static int custom_close(thandle_t handle) {
    int fd = *(int*)handle;
    return close(fd);
}

static toff_t custom_size(thandle_t handle) {
    int fd = *(int*)handle;
    off_t cur = lseek(fd, 0, SEEK_CUR);
    off_t end = lseek(fd, 0, SEEK_END);
    lseek(fd, cur, SEEK_SET);
    return end;
}

static int custom_map(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void custom_unmap(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Write fuzzer data to dummy file */
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    lseek(fd, 0, SEEK_SET);
    
    /* Test TIFFFdOpen */
    TIFF* tif1 = TIFFFdOpen(fd, "./dummy_file", "r");
    if (tif1) {
        /* Test TIFFFileno */
        int fileno1 = TIFFFileno(tif1);
        (void)fileno1;
        
        /* Test TIFFFlush */
        TIFFFlush(tif1);
        
        /* Test TIFFSetFileno */
        int new_fd = dup(fd);
        if (new_fd >= 0) {
            int old_fileno = TIFFSetFileno(tif1, new_fd);
            (void)old_fileno;
            close(new_fd);
        }
        
        TIFFClose(tif1);
    }
    
    /* Reset file position for next tests */
    lseek(fd, 0, SEEK_SET);
    
    /* Test TIFFFdOpenExt - pass NULL for opts since we can't initialize the opaque struct */
    TIFF* tif2 = TIFFFdOpenExt(fd, "./dummy_file", "r", NULL);
    if (tif2) {
        TIFFFlush(tif2);
        TIFFClose(tif2);
    }
    
    /* Reset file position for TIFFClientOpen */
    lseek(fd, 0, SEEK_SET);
    
    /* Test TIFFClientOpen */
    int handle_fd = dup(fd);
    if (handle_fd >= 0) {
        thandle_t handle = (thandle_t)&handle_fd;
        TIFF* tif3 = TIFFClientOpen("./dummy_file", "r", handle,
                                   custom_read, custom_write,
                                   custom_seek, custom_close,
                                   custom_size, custom_map, custom_unmap);
        if (tif3) {
            TIFFFlush(tif3);
            TIFFClose(tif3);
        } else {
            close(handle_fd);
        }
    }
    
    close(fd);
    unlink("./dummy_file");
    
    return 0;
}