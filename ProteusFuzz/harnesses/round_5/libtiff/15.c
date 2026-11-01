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
#include <tiffio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return 0;
    }
    /* Write a minimal TIFF header to ensure the file is recognized as TIFF */
    /* Little-endian signature (0x4949) */
    uint16_t magic = 0x4949;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    /* Version 42 */
    uint16_t version = 42;
    fwrite(&version, sizeof(uint16_t), 1, f);
    /* Offset to first IFD (we will write it at offset 8) */
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    /* Write the fuzzing data as the IFD content */
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
    return 1;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare the environment: create a dummy file with the input data */
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    /* Open the file for reading */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    /* 
     * Invoke the target functions in the specified order:
     * TIFFCurrentDirectory -> TIFFSetDirectory -> TIFFSetSubDirectory -> 
     * TIFFReadDirectory -> TIFFSetDirectory -> TIFFReadDirectory -> 
     * TIFFCurrentDirectory -> TIFFSetSubDirectory -> TIFFNumberOfDirectories
     */

    /* 1. TIFFCurrentDirectory */
    tdir_t dir_idx = TIFFCurrentDirectory(tif);
    (void)dir_idx; /* Suppress unused variable warning */

    /* 2. TIFFSetDirectory */
    /* Try setting to the current directory or 0 */
    (void)TIFFSetDirectory(tif, dir_idx);

    /* 3. TIFFSetSubDirectory */
    /* Use input data to generate a subdirectory offset if possible, otherwise 0 */
    uint64_t sub_offset = 0;
    if (Size >= 8) {
        sub_offset = *(const uint64_t *)(Data);
    } else if (Size >= 4) {
        sub_offset = *(const uint32_t *)(Data);
    }
    (void)TIFFSetSubDirectory(tif, sub_offset);

    /* 4. TIFFReadDirectory */
    int read_result = TIFFReadDirectory(tif);
    (void)read_result;

    /* 5. TIFFSetDirectory */
    /* Try setting to directory 0 or the previously read index */
    (void)TIFFSetDirectory(tif, 0);

    /* 6. TIFFReadDirectory */
    (void)TIFFReadDirectory(tif);

    /* 7. TIFFCurrentDirectory */
    (void)TIFFCurrentDirectory(tif);

    /* 8. TIFFSetSubDirectory */
    /* Use a different offset derived from input */
    uint64_t sub_offset2 = 0;
    if (Size >= 8) {
        sub_offset2 = *(const uint64_t *)(Data) + 1;
    } else if (Size >= 4) {
        sub_offset2 = *(const uint32_t *)(Data) + 1;
    }
    (void)TIFFSetSubDirectory(tif, sub_offset2);

    /* 9. TIFFNumberOfDirectories */
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    (void)num_dirs;

    /* Cleanup */
    TIFFClose(tif);
    
    return 0;
}