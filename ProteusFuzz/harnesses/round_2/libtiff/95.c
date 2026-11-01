// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open the file for reading/writing
    TIFF *tif = TIFFOpen("./dummy_file", "rw");
    if (!tif) {
        return 0;
    }

    // Fuzz TIFFCreateDirectory
    TIFFCreateDirectory(tif);

    // Fuzz TIFFFreeDirectory
    // Note: TIFFFreeDirectory is void, no return value to check
    TIFFFreeDirectory(tif);

    // Re-create directory for subsequent tests
    TIFFCreateDirectory(tif);

    // Fuzz TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // Re-create directory for subsequent tests
    TIFFCreateDirectory(tif);

    // Fuzz TIFFCreateGPSDirectory
    TIFFCreateGPSDirectory(tif);

    // Re-create directory for subsequent tests
    TIFFCreateDirectory(tif);

    // Fuzz TIFFWriteDirectory
    // This may fail if no tags are set, but it's safe to call
    TIFFWriteDirectory(tif);

    // Re-create directory for custom directory test
    TIFFCreateDirectory(tif);

    // Fuzz TIFFCreateCustomDirectory
    // We need a valid TIFFFieldArray. Since we don't have a custom one,
    // we can try to use the standard EXIF or GPS arrays if accessible,
    // but the API requires a pointer. For fuzzing, we can't easily construct
    // a valid custom array without internal knowledge. 
    // Instead, we can try to call it with a NULL pointer to test error handling,
    // but the instructions say "Do not pass NULL". 
    // Since we cannot construct a valid custom array here, we skip this call
    // to avoid undefined behavior or crashes.
    // However, to explore more states, we can try to set some fields and then write.
    
    // Set some basic fields to make TIFFWriteDirectory more interesting
    uint16_t photometric = 1; // MinIsBlack
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    
    uint16_t bitspersample = 8;
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    
    uint32_t width = (Size > 0) ? Data[0] : 1;
    uint32_t length = (Size > 1) ? Data[1] : 1;
    if (width == 0) width = 1;
    if (length == 0) length = 1;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);

    // Try to write the directory again with some data set
    TIFFWriteDirectory(tif);

    // Cleanup
    TIFFClose(tif);

    return 0;
}