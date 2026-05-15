// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static tmsize_t dummyRead(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummyWrite(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return size;
}

static toff_t dummySeek(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummyClose(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummySize(thandle_t handle) {
    (void)handle;
    return 0;
}

static int dummyMap(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void dummyUnmap(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    
    // Write fuzzer data to file
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test TIFFOpen
    TIFF* tif = TIFFOpen(filename, "r");
    if (tif) {
        // Test TIFFClientdata
        thandle_t clientdata = TIFFClientdata(tif);
        (void)clientdata;
        
        // Test TIFFSetClientdata and TIFFClientdata
        thandle_t dummy_handle = (thandle_t)(uintptr_t)0x1234;
        thandle_t prev = TIFFSetClientdata(tif, dummy_handle);
        (void)prev;
        
        // Test TIFFSetClientInfo and TIFFGetClientInfo
        const char* info_name = "test_info";
        void* info_data = (void*)(uintptr_t)0x5678;
        TIFFSetClientInfo(tif, info_data, info_name);
        
        void* retrieved = TIFFGetClientInfo(tif, info_name);
        (void)retrieved;
        
        // Try with different name
        if (Size > 10) {
            char dynamic_name[32];
            size_t name_len = Size % 20 + 1;
            if (name_len > 31) name_len = 31;
            for (size_t i = 0; i < name_len; i++) {
                dynamic_name[i] = 'A' + (Data[i] % 26);
            }
            dynamic_name[name_len] = '\0';
            TIFFSetClientInfo(tif, (void*)(uintptr_t)Data[0], dynamic_name);
            TIFFGetClientInfo(tif, dynamic_name);
        }
        
        TIFFClose(tif);
    }
    
    // Test TIFFClientOpenExt with dummy callbacks
    if (Size > 50) {
        thandle_t custom_handle = (thandle_t)(uintptr_t)0xABCD;
        TIFFOpenOptions* opts = NULL;
        
        TIFF* tif2 = TIFFClientOpenExt(
            "dummy", "r", custom_handle,
            dummyRead, dummyWrite,
            dummySeek, dummyClose, dummySize,
            dummyMap, dummyUnmap,
            opts
        );
        
        if (tif2) {
            // Test client data functions on this handle too
            thandle_t cd = TIFFClientdata(tif2);
            (void)cd;
            
            thandle_t new_handle = (thandle_t)(uintptr_t)0xDEADBEEF;
            TIFFSetClientdata(tif2, new_handle);
            
            TIFFSetClientInfo(tif2, (void*)(uintptr_t)Data[1], "ext_info");
            TIFFGetClientInfo(tif2, "ext_info");
            
            TIFFClose(tif2);
        }
    }
    
    // Try opening in different modes if enough data
    if (Size > 100) {
        const char* modes[] = {"r", "w", "a"};
        int mode_idx = Data[0] % 3;
        
        TIFF* tif3 = TIFFOpen(filename, modes[mode_idx]);
        if (tif3) {
            // Exercise different code paths based on mode
            if (mode_idx == 1 || mode_idx == 2) { // write or append modes
                TIFFSetClientInfo(tif3, (void*)(uintptr_t)Data[2], "write_info");
            }
            TIFFClose(tif3);
        }
    }
    
    // Clean up
    unlink(filename);
    
    return 0;
}