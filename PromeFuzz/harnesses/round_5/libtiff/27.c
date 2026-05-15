// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static tmsize_t dummy_read(thandle_t fd, void* buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void* buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummy_seek(thandle_t fd, toff_t off, int whence) {
    (void)fd;
    (void)off;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t fd) {
    (void)fd;
    return 0;
}

static toff_t dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

static int dummy_map(thandle_t fd, void** base, toff_t* off) {
    (void)fd;
    (void)base;
    (void)off;
    return 0;
}

static void dummy_unmap(thandle_t fd, void* base, toff_t off) {
    (void)fd;
    (void)base;
    (void)off;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);
    
    // Test TIFFOpen
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try with write mode
        tif = TIFFOpen("./dummy_file", "w");
    }
    
    if (tif) {
        // Test TIFFSetClientdata and TIFFClientdata
        thandle_t client_data = (thandle_t)(uintptr_t)Data;
        thandle_t prev_data = TIFFSetClientdata(tif, client_data);
        thandle_t current_data = TIFFClientdata(tif);
        
        // Test TIFFSetClientInfo and TIFFGetClientInfo
        if (Size > 10) {
            char name[11];
            size_t name_len = Size > 20 ? 10 : Size / 2;
            memcpy(name, Data, name_len);
            name[name_len] = '\0';
            
            void* info_data = (void*)(Data + name_len);
            TIFFSetClientInfo(tif, info_data, name);
            void* retrieved = TIFFGetClientInfo(tif, name);
            (void)retrieved;
        }
        
        TIFFClose(tif);
    }
    
    // Test TIFFClientOpenExt
    thandle_t handle = (thandle_t)(uintptr_t)Data;
    TIFFOpenOptions* opts = NULL;
    
    // Try different mode combinations
    const char* modes[] = {"r", "w", "a"};
    for (int i = 0; i < 3; i++) {
        TIFF* client_tif = TIFFClientOpenExt(
            "dummy", modes[i], handle,
            dummy_read, dummy_write,
            dummy_seek, dummy_close, dummy_size,
            dummy_map, dummy_unmap,
            opts
        );
        
        if (client_tif) {
            // Test client data operations on this handle too
            thandle_t dummy_client = (thandle_t)(uintptr_t)(Data + Size - 1);
            TIFFSetClientdata(client_tif, dummy_client);
            TIFFClientdata(client_tif);
            
            if (Size > 5) {
                char name2[6];
                memcpy(name2, Data, 5);
                name2[5] = '\0';
                TIFFSetClientInfo(client_tif, (void*)Data, name2);
                TIFFGetClientInfo(client_tif, name2);
            }
            
            TIFFClose(client_tif);
        }
    }
    
    // Clean up
    unlink("./dummy_file");
    return 0;
}