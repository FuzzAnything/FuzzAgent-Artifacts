// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    
    // Write at least a minimal TIFF header (8 bytes) if input is too small
    if (Size == 0) {
        uint8_t header[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        write(fd, header, 8);
    } else {
        write(fd, Data, Size);
    }
    
    close(fd);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Ensure file exists with some data
    write_dummy_file(Data, Size);

    // Open for reading
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (tif_r) {
        // Fuzz TIFFCurrentDirOffset
        (void)TIFFCurrentDirOffset(tif_r);
        
        // Fuzz TIFFGetStrileOffset with various indices
        uint32_t strile_idx = Data[0] | ((Data[1] << 8) & 0xFF00) | 
                              ((Data[2] << 16) & 0xFF0000) | 
                              ((Data[3] << 24) & 0xFF000000);
        (void)TIFFGetStrileOffset(tif_r, strile_idx);
        
        // Fuzz TIFFSetSubDirectory with various offsets
        uint64_t sub_dir_off = 0;
        if (Size > 8) {
            memcpy(&sub_dir_off, Data, 8);
        }
        (void)TIFFSetSubDirectory(tif_r, sub_dir_off);
        
        TIFFClose(tif_r);
    }

    // Open for writing (may fail if file is not writable or format is invalid, which is fine for fuzzing)
    TIFF *tif_w = TIFFOpen("./dummy_file", "w");
    if (tif_w) {
        // Fuzz TIFFSetWriteOffset
        toff_t write_off = 0;
        if (Size > 8) {
            memcpy(&write_off, Data, 8);
        }
        TIFFSetWriteOffset(tif_w, write_off);
        
        // Fuzz TIFFWriteCheck with various parameters
        int check_result = TIFFWriteCheck(tif_w, Data[0] % 2, "test");
        (void)check_result;
        
        // Attempt to rewrite directory (may fail if no directory exists, which is expected)
        (void)TIFFRewriteDirectory(tif_w);
        
        TIFFClose(tif_w);
    }

    return 0;
}