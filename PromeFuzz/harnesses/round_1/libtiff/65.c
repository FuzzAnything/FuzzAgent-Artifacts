// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static tmsize_t dummy_readproc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* f = (FILE*)handle;
    return (tmsize_t)fread(buf, 1, size, f);
}

static tmsize_t dummy_writeproc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* f = (FILE*)handle;
    return (tmsize_t)fwrite(buf, 1, size, f);
}

static toff_t dummy_seekproc(thandle_t handle, toff_t offset, int whence) {
    FILE* f = (FILE*)handle;
    if (fseek(f, offset, whence) == 0) {
        return ftell(f);
    }
    return (toff_t)-1;
}

static int dummy_closeproc(thandle_t handle) {
    FILE* f = (FILE*)handle;
    return fclose(f);
}

static toff_t dummy_sizeproc(thandle_t handle) {
    FILE* f = (FILE*)handle;
    long current = ftell(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, current, SEEK_SET);
    return size;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size, const char* mode) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(data, 1, size, f);
    fclose(f);
    
    f = fopen("./dummy_file", "rb+");
    if (!f) return NULL;
    
    TIFF* tif = TIFFClientOpen("dummy", mode, (thandle_t)f,
                              dummy_readproc, dummy_writeproc,
                              dummy_seekproc, dummy_closeproc,
                              dummy_sizeproc, NULL, NULL);
    return tif;
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_65(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy TIFF file with fuzz data
    TIFF* tif = create_dummy_tiff(Data, Size, "w");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Use fuzz data to determine which operations to perform
    uint8_t op_selector = Data[0] % 7;
    
    switch (op_selector) {
        case 0: {
            // Test TIFFCreateCustomDirectory with NULL field array
            TIFFCreateCustomDirectory(tif, NULL);
            break;
        }
        case 1: {
            // Test TIFFAccessTagMethods
            TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
            (void)methods;
            break;
        }
        case 2: {
            // Test TIFFFreeDirectory
            if (TIFFCreateDirectory(tif) == 1) {
                TIFFFreeDirectory(tif);
            }
            break;
        }
        case 3: {
            // Test TIFFCreateDirectory
            TIFFCreateDirectory(tif);
            break;
        }
        case 4: {
            // Test TIFFReadCustomDirectory with fuzz-derived offset
            if (Size >= 9) {
                uint64_t diroff;
                memcpy(&diroff, Data + 1, sizeof(uint64_t));
                TIFFReadCustomDirectory(tif, diroff, NULL);
            }
            break;
        }
        case 5: {
            // Test TIFFWriteCustomDirectory
            uint64_t offset = 0;
            TIFFWriteCustomDirectory(tif, &offset);
            break;
        }
        case 6: {
            // Test combination: Create directory, then free it
            if (TIFFCreateDirectory(tif) == 1) {
                TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
                (void)methods;
                TIFFFreeDirectory(tif);
            }
            break;
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}