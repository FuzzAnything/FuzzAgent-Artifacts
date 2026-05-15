// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
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

/* Dummy I/O callbacks for TIFFClientOpenExt */
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

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    /* Write fuzzer data to dummy file */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    /* Test TIFFOpen */
    TIFF* tiff = TIFFOpen("./dummy_file", "r");
    if (tiff) {
        /* Test TIFFSetClientdata and TIFFClientdata */
        thandle_t client_data = (thandle_t)(uintptr_t)0x12345678;
        thandle_t prev_data = TIFFSetClientdata(tiff, client_data);
        thandle_t retrieved_data = TIFFClientdata(tiff);
        (void)prev_data;
        (void)retrieved_data;

        /* Test TIFFSetClientInfo and TIFFGetClientInfo */
        if (Size > 0) {
            /* Use first byte as name length (modulo 16 for safety) */
            size_t name_len = (Data[0] % 16) + 1;
            if (name_len > Size - 1) name_len = Size > 1 ? Size - 1 : 1;
            
            char* name = malloc(name_len + 1);
            if (name) {
                memcpy(name, Data + 1, name_len);
                name[name_len] = '\0';
                
                void* info_data = (void*)(uintptr_t)0x87654321;
                TIFFSetClientInfo(tiff, info_data, name);
                void* retrieved_info = TIFFGetClientInfo(tiff, name);
                (void)retrieved_info;
                
                free(name);
            }
        }

        /* Test with different client data values */
        for (int i = 0; i < 3; i++) {
            thandle_t test_data = (thandle_t)(uintptr_t)(i * 0x1000);
            TIFFSetClientdata(tiff, test_data);
            TIFFClientdata(tiff);
        }

        TIFFClose(tiff);
    }

    /* Test TIFFClientOpenExt */
    TIFFOpenOptions* opts = NULL;
    thandle_t dummy_handle = (thandle_t)(uintptr_t)0xDEADBEEF;
    tiff = TIFFClientOpenExt("dummy", "r", dummy_handle,
                             dummy_read, dummy_write,
                             dummy_seek, dummy_close,
                             dummy_size, dummy_map, dummy_unmap,
                             opts);
    if (tiff) {
        /* Test client data operations */
        thandle_t test_data = (thandle_t)(uintptr_t)0xABCD1234;
        TIFFSetClientdata(tiff, test_data);
        TIFFClientdata(tiff);

        /* Test client info with fuzzer-derived name */
        if (Size > 10) {
            size_t name_len = (Data[10] % 32) + 1;
            if (name_len > Size - 11) name_len = Size > 11 ? Size - 11 : 1;
            
            char* name = malloc(name_len + 1);
            if (name) {
                memcpy(name, Data + 11, name_len);
                name[name_len] = '\0';
                
                void* info_data = (void*)(uintptr_t)0xF00DBEEF;
                TIFFSetClientInfo(tiff, info_data, name);
                TIFFGetClientInfo(tiff, name);
                
                free(name);
            }
        }

        TIFFClose(tiff);
    }

    /* Clean up dummy file */
    unlink("./dummy_file");
    
    return 0;
}