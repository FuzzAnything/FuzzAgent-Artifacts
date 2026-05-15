// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy TIFF file
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    close(fd);
    
    // Test TIFFOpen with different modes
    const char* modes[] = {"r", "w", "a", "r+", "w+", "a+"};
    int mode_index = Data[0] % (sizeof(modes) / sizeof(modes[0]));
    
    TIFF* tif = TIFFOpen(filename, modes[mode_index]);
    if (tif) {
        // Test TIFFClientdata
        thandle_t clientdata = TIFFClientdata(tif);
        (void)clientdata;
        
        // Test TIFFGetClientInfo with different names
        const char* names[] = {"test1", "data", "info", "custom", "user"};
        int name_index = (Data[0] + 1) % (sizeof(names) / sizeof(names[0]));
        void* info = TIFFGetClientInfo(tif, names[name_index]);
        (void)info;
        
        // Test TIFFSetClientdata with different values
        thandle_t dummy_handles[] = {NULL, (thandle_t)0x1, (thandle_t)0x2, (thandle_t)tif};
        int handle_index = (Data[0] + 2) % (sizeof(dummy_handles) / sizeof(dummy_handles[0]));
        thandle_t prev = TIFFSetClientdata(tif, dummy_handles[handle_index]);
        (void)prev;
        
        // Test TIFFSetClientInfo with different data
        int* dummy_data = malloc(sizeof(int));
        if (dummy_data) {
            *dummy_data = Data[0];
            TIFFSetClientInfo(tif, dummy_data, names[name_index]);
            free(dummy_data);
        }
        
        TIFFClose(tif);
    }
    
    // Test TIFFClientOpenExt with dummy callbacks
    thandle_t dummy_handle = (thandle_t)0x1234;
    TIFFOpenOptions* opts = NULL;
    
    tif = TIFFClientOpenExt("dummy", "r", dummy_handle,
                           dummy_read, dummy_write,
                           dummy_seek, dummy_close,
                           dummy_size, dummy_map,
                           dummy_unmap, opts);
    
    if (tif) {
        // Repeat the same tests on the client-opened TIFF
        thandle_t clientdata = TIFFClientdata(tif);
        (void)clientdata;
        
        const char* names[] = {"test1", "data", "info", "custom", "user"};
        int name_index = (Data[0] + 3) % (sizeof(names) / sizeof(names[0]));
        void* info = TIFFGetClientInfo(tif, names[name_index]);
        (void)info;
        
        thandle_t dummy_handles[] = {NULL, (thandle_t)0x1, (thandle_t)0x2, (thandle_t)tif};
        int handle_index = (Data[0] + 4) % (sizeof(dummy_handles) / sizeof(dummy_handles[0]));
        thandle_t prev = TIFFSetClientdata(tif, dummy_handles[handle_index]);
        (void)prev;
        
        int* dummy_data = malloc(sizeof(int));
        if (dummy_data) {
            *dummy_data = Data[0] + 1;
            TIFFSetClientInfo(tif, dummy_data, names[name_index]);
            free(dummy_data);
        }
        
        TIFFClose(tif);
    }
    
    // Clean up
    unlink(filename);
    return 0;
}