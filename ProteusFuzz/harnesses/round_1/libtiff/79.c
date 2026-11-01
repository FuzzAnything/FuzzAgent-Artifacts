// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Fuzz TIFFReadEXIFDirectory
    toff_t diroff = 0;
    if (Size >= sizeof(toff_t)) {
        memcpy(&diroff, Data, sizeof(toff_t));
    }
    (void)TIFFReadEXIFDirectory(tif, diroff);

    // Fuzz TIFFLastDirectory
    (void)TIFFLastDirectory(tif);

    // Fuzz TIFFSetDirectory
    tdir_t dir_idx = 0;
    if (Size >= sizeof(tdir_t) + sizeof(toff_t)) {
        memcpy(&dir_idx, Data + sizeof(toff_t), sizeof(tdir_t));
    }
    (void)TIFFSetDirectory(tif, dir_idx);

    // Fuzz TIFFReadCustomDirectory
    // We use the standard EXIF field array as a proxy for custom fields if available,
    // or simply pass the standard array if we can get it.
    // Since we cannot easily access internal _TIFFGetExifFields without linking internals,
    // we rely on the fact that TIFFReadCustomDirectory expects a valid array.
    // We will skip this specific call with a custom array unless we can safely construct one,
    // but we can try with a null pointer check or a known safe call if the API allows.
    // However, the API requires a const TIFFFieldArray *. We cannot easily construct one.
    // So we skip this specific API call to avoid undefined behavior, or we rely on the
    // fact that TIFFReadEXIFDirectory internally calls it.
    // To be safe and compliant with the task of fuzzing the API, we will not call
    // TIFFReadCustomDirectory directly with a fabricated pointer.
    
    // Fuzz TIFFReadGPSDirectory
    toff_t gps_diroff = 0;
    if (Size >= sizeof(toff_t) * 2) {
        memcpy(&gps_diroff, Data + sizeof(toff_t), sizeof(toff_t));
    }
    (void)TIFFReadGPSDirectory(tif, gps_diroff);

    // Try to open for writing to test TIFFCreateEXIFDirectory
    // We need to create a new file or reuse the dummy file.
    // Let's try to open the dummy file for writing if possible, but TIFFOpen might fail
    // if the file is not a valid TIFF.
    // We will create a temporary write handle.
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write != NULL) {
        (void)TIFFCreateEXIFDirectory(tif_write);
        TIFFClose(tif_write);
    }

    TIFFClose(tif);
    return 0;
}