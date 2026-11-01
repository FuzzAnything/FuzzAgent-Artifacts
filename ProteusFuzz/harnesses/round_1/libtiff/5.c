// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsBigTIFF at tif_open.c:907:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    } else {
        // Write a minimal valid TIFF header if input is empty to avoid immediate crash on open
        uint16_t magic = 0x4949; // Little endian
        uint16_t version = 42;
        uint32_t offset = 8;
        fwrite(&magic, 2, 1, fp);
        fwrite(&version, 2, 1, fp);
        fwrite(&offset, 4, 1, fp);
    }
    fclose(fp);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (tif == NULL) {
        // If read+write fails, try write mode to create a file for writing tests
        tif = TIFFOpen("./dummy_file", "w");
    }

    if (tif == NULL) {
        return 0;
    }

    // 1. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 2. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 0);

    // 3. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 1);

    // 4. TIFFReadDirectory
    (void)TIFFReadDirectory(tif);

    // 5. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 0);

    // 6. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 1);

    // 7. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 2);

    // 8. TIFFSetSubDirectory
    (void)TIFFSetSubDirectory(tif, 0);

    // 9. TIFFSetSubDirectory
    (void)TIFFSetSubDirectory(tif, 100);

    // 10. TIFFIsBigEndian
    (void)TIFFIsBigEndian(tif);

    // 11. TIFFIsBigTIFF
    (void)TIFFIsBigTIFF(tif);

    // 12. TIFFIsByteSwapped
    (void)TIFFIsByteSwapped(tif);

    // 13. TIFFIsByteSwapped
    (void)TIFFIsByteSwapped(tif);

    // 14. TIFFNumberOfDirectories
    (void)TIFFNumberOfDirectories(tif);

    // 15. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 0);

    // 16. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 1);

    // 17. TIFFIsByteSwapped
    (void)TIFFIsByteSwapped(tif);

    // 18. TIFFNumberOfDirectories
    (void)TIFFNumberOfDirectories(tif);

    // 19. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 0);

    // 20. TIFFSetDirectory
    (void)TIFFSetDirectory(tif, 1);

    // 21. TIFFClose
    TIFFClose(tif);

    // 22. TIFFClose (Safe to call on NULL or already closed, but we pass NULL here to test robustness if needed, 
    // though the sequence implies closing the same handle twice or a second handle. 
    // Since we only have one handle, we close it again (safe in libtiff usually) or pass NULL.
    // The prompt says "TIFFClose" -> "TIFFClose". Let's close the same handle again if it was valid, 
    // but since it's closed, it might be NULL internally. 
    // To strictly follow the sequence with a valid pointer, we would need a second file.
    // However, TIFFClose handles NULL gracefully. Let's just call it again on the variable which is now invalid logic-wise 
    // but the function handles it. Or better, open a second dummy file to satisfy the sequence strictly.
    
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2 != NULL) {
        TIFFClose(tif2);
    }
    
    // If the first close didn't invalidate the pointer variable in C (it doesn't), 
    // calling TIFFClose(tif) again is safe if libtiff handles it, but usually we shouldn't.
    // The prompt asks for the sequence. If tif is closed, tif2 is the second close.
    // If tif2 failed to open, we call TIFFClose(NULL) which is safe.
    
    return 0;
}