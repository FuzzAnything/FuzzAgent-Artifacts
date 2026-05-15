// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

#include "libtiff/tiffio.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    return (written == (ssize_t)size) ? 0 : -1;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    TIFF* tif = NULL;
    const char* filename = "./dummy_file";
    
    uint8_t mode_selector = Data[0] % 3;
    const char* mode_str = NULL;
    switch (mode_selector) {
        case 0: mode_str = "r"; break;
        case 1: mode_str = "w"; break;
        case 2: mode_str = "a"; break;
        default: mode_str = "r"; break;
    }
    
    tif = TIFFOpen(filename, mode_str);
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    size_t data_index = 1;
    
    if (data_index < Size) {
        int mode = Data[data_index] % 3;
        TIFFSetMode(tif, mode);
        data_index++;
    }
    
    if (data_index < Size) {
        TIFFCheckpointDirectory(tif);
        data_index++;
    }
    
    if (data_index + 1 < Size) {
        int tile = Data[data_index] % 2;
        const char* module = "fuzz";
        TIFFWriteCheck(tif, tile, module);
        data_index++;
    }
    
    if (data_index < Size) {
        TIFFCreateDirectory(tif);
        data_index++;
    }
    
    if (data_index < Size) {
        TIFFGetMode(tif);
        data_index++;
    }
    
    if (data_index < Size) {
        TIFFWriteDirectory(tif);
        data_index++;
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}