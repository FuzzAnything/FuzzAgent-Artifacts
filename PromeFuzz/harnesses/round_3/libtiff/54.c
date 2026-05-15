// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Custom I/O callbacks for TIFFClientOpenExt */
static tmsize_t myReadProc(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t myWriteProc(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t mySeekProc(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static int myCloseProc(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t mySizeProc(thandle_t handle) {
    (void)handle;
    return 0;
}

static int myMapProc(thandle_t handle, void** base, toff_t* offset) {
    (void)handle;
    (void)base;
    (void)offset;
    return 0;
}

static void myUnmapProc(thandle_t handle, void* base, toff_t offset) {
    (void)handle;
    (void)base;
    (void)offset;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    const char* filename = "./dummy_file";
    
    /* Write fuzzer data to file for TIFFOpen */
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    /* Test TIFFOpen with different modes */
    const char* modes[] = {"r", "w", "a", "r+"};
    for (size_t i = 0; i < sizeof(modes)/sizeof(modes[0]); i++) {
        TIFF* tif = TIFFOpen(filename, modes[i]);
        if (tif) {
            /* Test TIFFClientdata */
            thandle_t clientdata = TIFFClientdata(tif);
            (void)clientdata;
            
            /* Test TIFFGetClientInfo with various names */
            const char* names[] = {"test", "data", "fuzz", ""};
            for (size_t j = 0; j < sizeof(names)/sizeof(names[0]); j++) {
                void* info = TIFFGetClientInfo(tif, names[j]);
                (void)info;
            }
            
            /* Test TIFFSetClientdata */
            thandle_t prev = TIFFSetClientdata(tif, (thandle_t)0x1234);
            (void)prev;
            
            /* Test TIFFSetClientInfo */
            for (size_t j = 0; j < sizeof(names)/sizeof(names[0]); j++) {
                TIFFSetClientInfo(tif, (void*)(uintptr_t)j, names[j]);
            }
            
            TIFFClose(tif);
        }
    }
    
    /* Test TIFFClientOpenExt with custom I/O */
    TIFFOpenOptions* opts = NULL;
    thandle_t dummy_handle = (thandle_t)0x5678;
    
    TIFF* client_tif = TIFFClientOpenExt("memory", "r", dummy_handle,
                                         myReadProc, myWriteProc,
                                         mySeekProc, myCloseProc,
                                         mySizeProc, myMapProc,
                                         myUnmapProc, opts);
    if (client_tif) {
        /* Exercise the same functions on client-opened TIFF */
        thandle_t cd = TIFFClientdata(client_tif);
        (void)cd;
        
        void* info = TIFFGetClientInfo(client_tif, "client");
        (void)info;
        
        thandle_t prev = TIFFSetClientdata(client_tif, (thandle_t)0x9ABC);
        (void)prev;
        
        TIFFSetClientInfo(client_tif, (void*)Data, "fuzzdata");
        
        TIFFClose(client_tif);
    }
    
    /* Clean up */
    unlink(filename);
    return 0;
}