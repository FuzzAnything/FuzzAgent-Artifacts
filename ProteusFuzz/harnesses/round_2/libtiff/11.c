// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tiffio.h>

static void create_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    
    // Ensure we have at least a minimal TIFF header (8 bytes) to avoid immediate crash
    if (Size < 8) {
        uint8_t header[8];
        // Minimal valid TIFF header (Little Endian)
        header[0] = 'I';
        header[1] = 'I';
        header[2] = 42;
        header[3] = 0;
        header[4] = 8; // Offset to first IFD
        header[5] = 0;
        header[6] = 0;
        header[7] = 0;
        
        write(fd, header, 8);
        if (Size > 0) {
            write(fd, Data, Size);
        }
    } else {
        write(fd, Data, Size);
    }
    
    close(fd);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    
    // Create a dummy file from fuzzing input
    create_dummy_file(Data, Size);
    
    // Open the file in read-only mode
    tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }
    
    // 1. TIFFCurrentDirectory
    tdir_t dir1 = TIFFCurrentDirectory(tif);
    (void)dir1;
    
    // 2. TIFFSetDirectory (try to set to index 0)
    if (TIFFSetDirectory(tif, 0) != 1) {
        // If setting directory 0 fails, we might still proceed if the file is valid but empty
        // We continue to explore other states
    }
    
    // 3. TIFFSetSubDirectory (try to set to 0 to reset or a random offset from input)
    uint64_t sub_offset = 0;
    if (Size >= 8) {
        sub_offset = *(const uint64_t*)(Data);
    } else if (Size > 0) {
        sub_offset = Data[0];
    }
    TIFFSetSubDirectory(tif, sub_offset);
    
    // 4. TIFFReadDirectory
    int read_res1 = TIFFReadDirectory(tif);
    (void)read_res1;
    
    // 5. TIFFSetDirectory (try index 1)
    TIFFSetDirectory(tif, 1);
    
    // 6. TIFFReadDirectory
    int read_res2 = TIFFReadDirectory(tif);
    (void)read_res2;
    
    // 7. TIFFCurrentDirectory
    tdir_t dir2 = TIFFCurrentDirectory(tif);
    (void)dir2;
    
    // 8. TIFFSetSubDirectory (try another offset)
    uint64_t sub_offset2 = 0;
    if (Size >= 16) {
        sub_offset2 = *(const uint64_t*)(Data + 8);
    } else if (Size > 8) {
        sub_offset2 = Data[8];
    }
    TIFFSetSubDirectory(tif, sub_offset2);
    
    // 9. TIFFNumberOfDirectories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    (void)num_dirs;
    
    // Cleanup
    TIFFClose(tif);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}