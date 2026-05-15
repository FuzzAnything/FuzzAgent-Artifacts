// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    write(fd, Data, Size);
    close(fd);

    TIFF* tif = TIFFOpen(filename, "r+");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    uint64_t offset = 0;
    int operation = Data[0] % 6;
    
    switch (operation) {
        case 0:
            TIFFFreeDirectory(tif);
            break;
            
        case 1: {
            toff_t diroff = 0;
            if (Size >= sizeof(toff_t) + 1) {
                memcpy(&diroff, Data + 1, sizeof(toff_t));
            }
            TIFFReadCustomDirectory(tif, diroff, NULL);
            break;
        }
            
        case 2: {
            TIFFSetTagExtender(NULL);
            break;
        }
            
        case 3:
            TIFFWriteCustomDirectory(tif, &offset);
            break;
            
        case 4:
            TIFFCreateCustomDirectory(tif, NULL);
            break;
            
        case 5:
            TIFFWriteDirectory(tif);
            break;
    }

    TIFFClose(tif);
    unlink(filename);
    return 0;
}