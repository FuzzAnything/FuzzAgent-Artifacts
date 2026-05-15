// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* Dummy I/O callbacks for TIFFClientOpenExt */
static tmsize_t dummyReadProc(thandle_t fd, void* buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummyWriteProc(thandle_t fd, void* buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummySeekProc(thandle_t fd, toff_t offset, int whence) {
    (void)fd;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummyCloseProc(thandle_t fd) {
    (void)fd;
    return 0;
}

static toff_t dummySizeProc(thandle_t fd) {
    (void)fd;
    return 0;
}

static int dummyMapProc(thandle_t fd, void** base, toff_t* size) {
    (void)fd;
    (void)base;
    (void)size;
    return 0;
}

static void dummyUnmapProc(thandle_t fd, void* base, toff_t size) {
    (void)fd;
    (void)base;
    (void)size;
}

/* Helper to create dummy file */
static void create_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    thandle_t client_data = (thandle_t)0x12345678;
    thandle_t old_client_data;
    void *client_info_ptr = NULL;
    const char *info_name = "test_info";
    int fd = -1;
    
    /* Create dummy file for file-based operations */
    create_dummy_file(Data, Size);
    
    /* Test 1: TIFFFdOpen */
    fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        tif = TIFFFdOpen(fd, "./dummy_file", "r");
        if (tif) {
            /* Test 2: TIFFClientdata */
            thandle_t retrieved_data = TIFFClientdata(tif);
            (void)retrieved_data;
            
            /* Test 3: TIFFSetClientdata */
            old_client_data = TIFFSetClientdata(tif, client_data);
            (void)old_client_data;
            
            /* Test 4: TIFFGetClientInfo */
            client_info_ptr = TIFFGetClientInfo(tif, info_name);
            (void)client_info_ptr;
            
            /* Test 5: TIFFSetClientInfo */
            TIFFSetClientInfo(tif, (void*)0xABCDEF, info_name);
            
            /* Clean up */
            TIFFClose(tif);
        }
        close(fd);
    }
    
    /* Test 6: TIFFClientOpenExt */
    TIFFOpenOptions *opts = NULL; /* NULL options for simplicity */
    thandle_t dummy_handle = (thandle_t)0xDEADBEEF;
    
    tif = TIFFClientOpenExt("dummy", "r", dummy_handle,
                            dummyReadProc, dummyWriteProc,
                            dummySeekProc, dummyCloseProc,
                            dummySizeProc, dummyMapProc,
                            dummyUnmapProc, opts);
    
    if (tif) {
        /* Exercise the functions again with different parameters */
        thandle_t dummy_data = TIFFClientdata(tif);
        (void)dummy_data;
        
        thandle_t prev_data = TIFFSetClientdata(tif, (thandle_t)0x87654321);
        (void)prev_data;
        
        /* Try different client info names */
        if (Size > 0) {
            char dynamic_name[256];
            size_t name_len = Size < 255 ? Size : 255;
            memcpy(dynamic_name, Data, name_len);
            dynamic_name[name_len] = '\0';
            
            TIFFSetClientInfo(tif, (void*)0x1234, dynamic_name);
            void *retrieved = TIFFGetClientInfo(tif, dynamic_name);
            (void)retrieved;
        }
        
        TIFFClose(tif);
    }
    
    /* Cleanup */
    cleanup_dummy_file();
    
    return 0;
}