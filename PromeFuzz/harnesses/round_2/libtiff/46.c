// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* Structure to hold our fake handle data */
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} fake_handle_t;

/* Custom I/O handlers for TIFFClientOpen/TIFFClientOpenExt */
static tmsize_t fake_read(thandle_t handle, void* buf, tmsize_t size) {
    fake_handle_t* fh = (fake_handle_t*)handle;
    if (!fh || !fh->data) return -1;
    
    if (fh->pos >= fh->size) return 0;
    
    size_t to_read = size;
    if (fh->pos + to_read > fh->size) {
        to_read = fh->size - fh->pos;
    }
    
    if (to_read > 0) {
        memcpy(buf, fh->data + fh->pos, to_read);
        fh->pos += to_read;
    }
    return to_read;
}

static tmsize_t fake_write(thandle_t handle, void* buf, tmsize_t size) {
    /* Dummy write - do nothing */
    return size;
}

static toff_t fake_seek(thandle_t handle, toff_t offset, int whence) {
    fake_handle_t* fh = (fake_handle_t*)handle;
    if (!fh) return -1;
    
    switch (whence) {
        case SEEK_SET:
            if (offset < 0 || (size_t)offset > fh->size) return -1;
            fh->pos = offset;
            break;
        case SEEK_CUR:
            if (fh->pos + offset < 0 || fh->pos + offset > fh->size) return -1;
            fh->pos += offset;
            break;
        case SEEK_END:
            if (fh->size + offset < 0 || fh->size + offset > fh->size) return -1;
            fh->pos = fh->size + offset;
            break;
        default:
            return -1;
    }
    return fh->pos;
}

static int fake_close(thandle_t handle) {
    fake_handle_t* fh = (fake_handle_t*)handle;
    if (fh) {
        free(fh);
    }
    return 0;
}

static toff_t fake_size(thandle_t handle) {
    fake_handle_t* fh = (fake_handle_t*)handle;
    if (!fh) return 0;
    return fh->size;
}

static int fake_map(thandle_t handle, void** base, toff_t* size) {
    /* No mapping support */
    return 0;
}

static void fake_unmap(thandle_t handle, void* base, toff_t size) {
    /* Nothing to do */
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    /* Create dummy file with fuzzer data */
    const char* filename = "./dummy_file";
    FILE* f = fopen(filename, "wb");
    if (!f) return 0;
    
    size_t to_write = Size > 4096 ? 4096 : Size;
    if (to_write > 0 && Data) {
        fwrite(Data, 1, to_write, f);
    }
    fclose(f);
    
    /* Test 1: TIFFFdOpen and TIFFFdOpenExt */
    int fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        TIFF* tif1 = TIFFFdOpen(fd, filename, "r");
        if (tif1) {
            /* Test TIFFFileno */
            int retrieved_fd = TIFFFileno(tif1);
            
            /* Test TIFFSetFileno */
            int old_fd = TIFFSetFileno(tif1, fd);
            
            TIFFClose(tif1);
        } else {
            close(fd);
        }
        
        /* Try again with TIFFFdOpenExt */
        fd = open(filename, O_RDONLY);
        if (fd >= 0) {
            TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
            if (opts) {
                TIFF* tif2 = TIFFFdOpenExt(fd, filename, "r", opts);
                if (tif2) {
                    TIFFClose(tif2);
                }
                TIFFOpenOptionsFree(opts);
            } else {
                close(fd);
            }
        }
    }
    
    /* Test 2: TIFFClientOpen */
    fake_handle_t* fake_handle1 = (fake_handle_t*)malloc(sizeof(fake_handle_t));
    if (fake_handle1) {
        fake_handle1->data = Data;
        fake_handle1->size = Size > 1024 ? 1024 : Size;
        fake_handle1->pos = 0;
        
        TIFF* tif3 = TIFFClientOpen("fake", "r", (thandle_t)fake_handle1,
                                    fake_read, fake_write,
                                    fake_seek, fake_close,
                                    fake_size, fake_map, fake_unmap);
        if (tif3) {
            /* Try to get/set fileno - may not work with custom I/O */
            int dummy_fd = TIFFFileno(tif3);
            if (dummy_fd >= 0) {
                TIFFSetFileno(tif3, dummy_fd);
            }
            TIFFClose(tif3);
        } else {
            /* If TIFFClientOpen failed, we still need to free the handle */
            free(fake_handle1);
        }
    }
    
    /* Test 3: TIFFClientOpenExt */
    fake_handle_t* fake_handle2 = (fake_handle_t*)malloc(sizeof(fake_handle_t));
    if (fake_handle2) {
        fake_handle2->data = Data;
        fake_handle2->size = Size > 1024 ? 1024 : Size;
        fake_handle2->pos = 0;
        
        TIFFOpenOptions* opts2 = TIFFOpenOptionsAlloc();
        if (opts2) {
            TIFF* tif4 = TIFFClientOpenExt("fake2", "r", (thandle_t)fake_handle2,
                                           fake_read, fake_write,
                                           fake_seek, fake_close,
                                           fake_size, fake_map, fake_unmap,
                                           opts2);
            if (tif4) {
                TIFFClose(tif4);
            } else {
                /* If TIFFClientOpenExt failed, free the handle */
                free(fake_handle2);
            }
            TIFFOpenOptionsFree(opts2);
        } else {
            free(fake_handle2);
        }
    }
    
    /* Cleanup */
    unlink(filename);
    return 0;
}