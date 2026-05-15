// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static tmsize_t dummy_read(thandle_t handle, void *buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return (tmsize_t)-1;
}

static tmsize_t dummy_write(thandle_t handle, void *buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return (tmsize_t)-1;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return (toff_t)-1;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    (void)handle;
    return 0;
}

static TIFF* create_dummy_tiff(void) {
    return TIFFClientOpen("dummy", "w", (thandle_t)1,
                          dummy_read, dummy_write,
                          dummy_seek, dummy_close,
                          dummy_size, NULL, NULL);
}

static void call_vsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    TIFF* tif = create_dummy_tiff();
    if (!tif) return 0;
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    switch (selector % 6) {
        case 0: {
            if (data_offset + sizeof(uint32_t) <= Size) {
                uint32_t tag;
                memcpy(&tag, Data + data_offset, sizeof(uint32_t));
                TIFFUnsetField(tif, tag);
            }
            break;
        }
        case 1: {
            if (data_offset + sizeof(int) <= Size) {
                int index;
                memcpy(&index, Data + data_offset, sizeof(int));
                TIFFGetTagListEntry(tif, index);
            }
            break;
        }
        case 2: {
            if (data_offset + sizeof(uint32_t) <= Size) {
                uint32_t tag;
                memcpy(&tag, Data + data_offset, sizeof(uint32_t));
                data_offset += sizeof(uint32_t);
                
                if (data_offset < Size) {
                    uint32_t value;
                    size_t value_size = (Size - data_offset) >= sizeof(uint32_t) ? 
                                        sizeof(uint32_t) : (Size - data_offset);
                    memcpy(&value, Data + data_offset, value_size);
                    
                    call_vsetfield(tif, tag, value);
                }
            }
            break;
        }
        case 3: {
            TIFFCreateCustomDirectory(tif, NULL);
            break;
        }
        case 4: {
            TIFFGetTagListCount(tif);
            break;
        }
        case 5: {
            uint32_t count = 0;
            if (data_offset + sizeof(uint32_t) <= Size) {
                memcpy(&count, Data + data_offset, sizeof(uint32_t));
                count = count % 10;
            }
            TIFFMergeFieldInfo(tif, NULL, count);
            break;
        }
    }
    
    TIFFClose(tif);
    return 0;
}