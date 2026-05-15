// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzzer input
    // We'll use the first bytes for directory indices and offsets
    uint32_t dir_idx1 = 0, dir_idx2 = 0, dir_idx3 = 0;
    uint64_t offset1 = 0, offset2 = 0, offset3 = 0, offset4 = 0;
    
    if (Size >= sizeof(uint32_t)) {
        memcpy(&dir_idx1, Data, sizeof(uint32_t));
    }
    if (Size >= 2 * sizeof(uint32_t)) {
        memcpy(&dir_idx2, Data + sizeof(uint32_t), sizeof(uint32_t));
    }
    if (Size >= 3 * sizeof(uint32_t)) {
        memcpy(&dir_idx3, Data + 2 * sizeof(uint32_t), sizeof(uint32_t));
    }
    if (Size >= 3 * sizeof(uint32_t) + sizeof(uint64_t)) {
        memcpy(&offset1, Data + 3 * sizeof(uint32_t), sizeof(uint64_t));
    }
    if (Size >= 3 * sizeof(uint32_t) + 2 * sizeof(uint64_t)) {
        memcpy(&offset2, Data + 3 * sizeof(uint32_t) + sizeof(uint64_t), sizeof(uint64_t));
    }
    if (Size >= 3 * sizeof(uint32_t) + 3 * sizeof(uint64_t)) {
        memcpy(&offset3, Data + 3 * sizeof(uint32_t) + 2 * sizeof(uint64_t), sizeof(uint64_t));
    }
    if (Size >= 3 * sizeof(uint32_t) + 4 * sizeof(uint64_t)) {
        memcpy(&offset4, Data + 3 * sizeof(uint32_t) + 3 * sizeof(uint64_t), sizeof(uint64_t));
    }
    
    // Call functions in specified order
    uint64_t offset_result;
    
    // 1. TIFFCurrentDirOffset
    offset_result = TIFFCurrentDirOffset(tif);
    
    // 2. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_idx1);
    
    // 3. TIFFCurrentDirOffset
    offset_result = TIFFCurrentDirOffset(tif);
    
    // 4. TIFFReadDirectory
    TIFFReadDirectory(tif);
    
    // 5. TIFFCurrentDirOffset
    offset_result = TIFFCurrentDirOffset(tif);
    
    // 6. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_idx2);
    
    // 7. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_idx3);
    
    // 8. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_idx1);
    
    // 9. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, offset1);
    
    // 10. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, offset2);
    
    // 11. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, offset3);
    
    // 12. TIFFSetSubDirectory
    TIFFSetSubDirectory(tif, offset4);
    
    // 13. TIFFIsBigEndian
    int is_big_endian = TIFFIsBigEndian(tif);
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}