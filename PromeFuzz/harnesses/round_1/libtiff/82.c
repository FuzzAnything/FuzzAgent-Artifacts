// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "libtiff/tiffio.h"

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    size_t read_count = fread(buf, 1, size, file);
    return (tmsize_t)read_count;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    size_t write_count = fwrite(buf, 1, size, file);
    return (tmsize_t)write_count;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) == 0) {
        return ftell(file);
    }
    return (toff_t)-1;
}

static int dummy_close(thandle_t handle) {
    FILE* file = (FILE*)handle;
    return fclose(file);
}

static toff_t dummy_size(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return (toff_t)size;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "w+b");
    if (!file) {
        return NULL;
    }
    
    if (size > 0 && data) {
        fwrite(data, 1, size > 4096 ? 4096 : size, file);
        fflush(file);
    }
    
    rewind(file);
    
    TIFF* tif = TIFFClientOpen("dummy", "w", (thandle_t)file,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, NULL, NULL);
    if (!tif) {
        fclose(file);
        return NULL;
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput_82(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        return 0;
    }
    
    int mode = Data[0] & 0x03;
    int tile_flag = (Data[0] >> 2) & 0x01;
    const char* module_name = "fuzz";
    
    int original_mode = TIFFGetMode(tif);
    
    TIFFSetMode(tif, mode);
    
    int new_mode = TIFFGetMode(tif);
    
    if (TIFFCreateDirectory(tif) == 1) {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        
        TIFFWriteCheck(tif, tile_flag, module_name);
        
        if (Size > 1) {
            TIFFCheckpointDirectory(tif);
        }
        
        if (Size > 2) {
            TIFFWriteDirectory(tif);
        }
    }
    
    if (Size > 3) {
        TIFFSetMode(tif, (Data[1] & 0x03));
        TIFFGetMode(tif);
    }
    
    TIFFClose(tif);
    
    unlink("./dummy_file");
    
    return 0;
}