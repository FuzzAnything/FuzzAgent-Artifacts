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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }
    
    // Call functions in the specified order
    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);
    
    // 2. TIFFSetDirectory  
    TIFFSetDirectory(tif, 0);
    
    // 3. TIFFCurrentDirOffset
    TIFFCurrentDirOffset(tif);
    
    // 4. TIFFGetSeekProc
    TIFFGetSeekProc(tif);
    
    // 5. TIFFIsByteSwapped
    TIFFIsByteSwapped(tif);
    
    // 6. TIFFSwabLong
    uint32_t value = 0x12345678;
    TIFFSwabLong(&value);
    
    // 7. TIFFGetWriteProc
    TIFFGetWriteProc(tif);
    
    // 8. TIFFNumberOfDirectories
    TIFFNumberOfDirectories(tif);
    
    // 9. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);
    
    // 10. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);
    
    // 11. TIFFClose
    TIFFClose(tif);
    
    // 12. TIFFClose - need to open another file since first one is closed
    // Re-open the file for the second close
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFClose(tif);
    }
    
    return 0;
}