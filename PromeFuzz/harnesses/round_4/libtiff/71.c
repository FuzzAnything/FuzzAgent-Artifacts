// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "libtiff/tiffio.h"

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    FILE* f = (FILE*)handle;
    size_t result = fread(buf, 1, size, f);
    return (tmsize_t)result;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    FILE* f = (FILE*)handle;
    size_t result = fwrite(buf, 1, size, f);
    return (tmsize_t)result;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    FILE* f = (FILE*)handle;
    if (fseek(f, (long)offset, whence) == 0) {
        return (toff_t)ftell(f);
    }
    return (toff_t)-1;
}

static int dummy_close(thandle_t handle) {
    FILE* f = (FILE*)handle;
    return fclose(f);
}

static toff_t dummy_size(thandle_t handle) {
    FILE* f = (FILE*)handle;
    long pos = ftell(f);
    if (fseek(f, 0, SEEK_END) != 0) {
        return (toff_t)-1;
    }
    long size = ftell(f);
    fseek(f, pos, SEEK_SET);
    return (toff_t)size;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size, int write_mode) {
    FILE* f = fopen("./dummy_file", write_mode ? "wb+" : "rb");
    if (!f) return NULL;
    
    if (write_mode) {
        if (size > 0) {
            size_t to_write = size > 1024 ? 1024 : size;
            fwrite(data, 1, to_write, f);
        }
        fseek(f, 0, SEEK_SET);
    } else {
        if (size > 0) {
            size_t to_write = size > 4096 ? 4096 : size;
            fwrite(data, 1, to_write, f);
        }
        fseek(f, 0, SEEK_SET);
    }
    
    TIFF* tif = TIFFClientOpen("dummy", write_mode ? "w" : "r", f,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, NULL, NULL);
    
    if (!tif) {
        fclose(f);
    }
    
    return tif;
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

static void call_tiffvsetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_71(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a TIFF handle for reading
    TIFF* tif_read = create_dummy_tiff(Data, Size, 0);
    if (!tif_read) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Create a TIFF handle for writing
    TIFF* tif_write = create_dummy_tiff(Data, Size, 1);
    if (!tif_write) {
        TIFFClose(tif_read);
        cleanup_dummy_file();
        return 0;
    }
    
    // Use first byte to determine which operations to perform
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // 1. Test TIFFFindField
    if (data_offset + 8 <= Size) {
        uint32_t tag;
        memcpy(&tag, Data + data_offset, sizeof(tag));
        uint32_t type_val;
        memcpy(&type_val, Data + data_offset + 4, sizeof(type_val));
        TIFFDataType type = (TIFFDataType)(type_val % 13);
        const TIFFField* field = TIFFFindField(tif_read, tag, type);
        (void)field;
        data_offset += 8;
    }
    
    // 2. Test TIFFVSetField using helper function
    if (data_offset + 8 <= Size && tif_write) {
        uint32_t tag;
        memcpy(&tag, Data + data_offset, sizeof(tag));
        uint32_t value;
        memcpy(&value, Data + data_offset + 4, sizeof(value));
        
        call_tiffvsetfield(tif_write, tag, value);
        data_offset += 8;
    }
    
    // 3. Test TIFFUnsetField
    if (data_offset + 4 <= Size && tif_write) {
        uint32_t tag;
        memcpy(&tag, Data + data_offset, sizeof(tag));
        TIFFUnsetField(tif_write, tag);
        data_offset += 4;
    }
    
    // 4. Test TIFFGetTagListEntry
    if (data_offset + 4 <= Size) {
        int index;
        memcpy(&index, Data + data_offset, sizeof(index));
        index = index % 100;
        TIFFGetTagListEntry(tif_read, index);
        data_offset += 4;
    }
    
    // 5. Test TIFFPrintDirectory
    if (tif_read) {
        FILE* out = fopen("./dummy_output", "w");
        if (out) {
            long flags = 0;
            if (data_offset + 4 <= Size) {
                memcpy(&flags, Data + data_offset, sizeof(flags));
                data_offset += 4;
            }
            TIFFPrintDirectory(tif_read, out, flags);
            fclose(out);
            remove("./dummy_output");
        }
    }
    
    // 6. Test TIFFMergeFieldInfo (requires field info array)
    if (data_offset + 8 <= Size && tif_write) {
        // Create a minimal field info array
        static const TIFFFieldInfo field_info[] = {
            { 65000, 1, 1, TIFF_LONG, 0, 1, 0, "CustomTag" }
        };
        uint32_t num_fields = 1;
        
        if ((selector & 0x1) && tif_write) {
            TIFFMergeFieldInfo(tif_write, field_info, num_fields);
        }
    }
    
    // Cleanup
    TIFFClose(tif_read);
    TIFFClose(tif_write);
    cleanup_dummy_file();
    
    return 0;
}