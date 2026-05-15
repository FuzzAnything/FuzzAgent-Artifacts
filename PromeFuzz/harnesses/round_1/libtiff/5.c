// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzing data to dummy file
    write_dummy_file(Data, Size);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Call functions in specified order
    // First two TIFFSetDirectory calls
    TIFFSetDirectory(tif, (tdir_t)(Data[0] % 10));
    TIFFSetDirectory(tif, (tdir_t)(Data[0] % 10));
    
    // TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset; // Suppress unused warning
    
    // TIFFGetSeekProc
    TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
    (void)seek_proc;
    
    // TIFFIsByteSwapped
    int swapped = TIFFIsByteSwapped(tif);
    (void)swapped;
    
    // TIFFSwabLong
    uint32_t long_val = 0x12345678;
    if (Size >= 4) {
        long_val = *(uint32_t*)Data;
    }
    TIFFSwabLong(&long_val);
    
    // TIFFGetWriteProc
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    (void)write_proc;
    
    // TIFFNumberOfDirectories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    (void)num_dirs;
    
    // Two more TIFFSetDirectory calls
    TIFFSetDirectory(tif, (tdir_t)((Data[0] + 1) % 10));
    TIFFSetDirectory(tif, (tdir_t)((Data[0] + 2) % 10));
    
    // First TIFFClose
    TIFFClose(tif);
    
    // Re-open for second close
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Second TIFFClose
        TIFFClose(tif);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}