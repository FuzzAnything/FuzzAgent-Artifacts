// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;

    // Write a minimal valid TIFF header (Little Endian)
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);

    uint16_t version = 42;
    fwrite(&version, sizeof(uint16_t), 1, f);

    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);

    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    }

    fclose(f);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    FILE *stream = fopen("./dummy_output", "w");
    if (!stream) {
        TIFFClose(tif);
        return 0;
    }

    // Try to set directory 0
    if (TIFFSetDirectory(tif, 0)) {
        TIFFPrintDirectory(tif, stream, 0);
        TIFFFreeDirectory(tif);
    }

    // Try setting a non-existent directory index
    tdir_t dir_index = (Size > 0) ? (tdir_t)Data[0] : 1;
    (void)TIFFSetDirectory(tif, dir_index);

    // Test TIFFCreateEXIFDirectory
    (void)TIFFCreateEXIFDirectory(tif);

    // Test TIFFCreateCustomDirectory
    // We cannot easily construct a valid TIFFFieldArray here due to opaque types
    // and internal dependencies in libtiff. Instead, we skip this call to avoid
    // compilation errors, as the instructions say "If custom directory creation 
    // is not required, omit the call to this function entirely."
    // To still exercise the API if possible without full struct definition, 
    // we would need to rely on libtiff's internal definitions which are not
    // fully exposed for external struct initialization in a portable way.
    // Thus, we omit this specific call to ensure compilation.

    // Test TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    fclose(stream);
    TIFFClose(tif);

    return 0;
}