// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsBigTIFF at tif_open.c:907:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return;
    }
    // Write a minimal valid TIFF header (BigTIFF or Classic)
    // We try to write the fuzz data as the image body to trigger parsing logic
    if (Size > 0) {
        // Write a minimal classic TIFF header (4 bytes) + fuzz data
        // Magic number for little-endian (II)
        uint16_t magic = 0x002A; 
        fwrite(&magic, sizeof(uint16_t), 1, fp);
        // Offset to first IFD (we'll put it right after header, but keep it simple)
        // Actually, let's just write the fuzz data directly after a minimal header
        // to see if libtiff can handle it or if it crashes.
        // A valid IFD needs at least 2 bytes for count, then entries.
        // To ensure we have some structure, we prepend a minimal header.
        // However, the task says "write the necessary data". 
        // Let's write the fuzz data as the raw file content.
        // But libtiff expects a header.
        // Strategy: Write a minimal valid header, then the fuzz data.
        // Minimal header: 2 bytes magic, 2 bytes version, 4 bytes offset (for classic).
        // Let's create a minimal valid header.
        uint16_t magic_le = 0x002A; // II
        uint16_t version = 42;
        uint32_t ifd_offset = 8; // Offset to IFD starts after header
        
        fwrite(&magic_le, sizeof(uint16_t), 1, fp);
        fwrite(&version, sizeof(uint16_t), 1, fp);
        fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);
        
        // Now write the fuzz data. 
        // The fuzz data will be interpreted as the IFD and subsequent data.
        // We need to ensure the IFD count is valid or handle errors.
        // Let's just write the fuzz data.
        fwrite(Data, 1, Size, fp);
    } else {
        // If no data, write a minimal valid file
        uint16_t magic_le = 0x002A;
        uint16_t version = 42;
        uint32_t ifd_offset = 8;
        uint16_t count = 0;
        uint32_t next_ifd = 0;
        
        fwrite(&magic_le, sizeof(uint16_t), 1, fp);
        fwrite(&version, sizeof(uint16_t), 1, fp);
        fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);
        fwrite(&count, sizeof(uint16_t), 1, fp);
        fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
    }
    fclose(fp);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Prepare environment
    if (Size == 0) {
        return 0;
    }

    // Write fuzz data to a dummy file
    write_dummy_file(Data, Size);

    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. Test TIFFIsBigEndian
    int is_be = TIFFIsBigEndian(tif);
    (void)is_be; // Suppress unused warning

    // 2. Test TIFFIsBigTIFF
    int is_big = TIFFIsBigTIFF(tif);
    (void)is_big;

    // 3. Test TIFFReadDirectory
    // This is the main parsing function. It might return 0 (EOF) or -1 (error).
    int read_result = TIFFReadDirectory(tif);
    (void)read_result;

    // 4. Test TIFFSetSubDirectory
    // Try setting subdirectory with 0 (reset) and with some offsets from input data if available
    if (Size >= 8) {
        uint64_t offset = 0;
        memcpy(&offset, Data, 8);
        // Try to set subdirectory. This might fail if offset is invalid, which is expected.
        int set_sub_result = TIFFSetSubDirectory(tif, offset);
        (void)set_sub_result;
    }
    // Also try with 0
    TIFFSetSubDirectory(tif, 0);

    // 5. Test TIFFGetStrileOffsetWithErr
    int err_code = 0;
    // Try with index 0
    uint64_t strile_off = TIFFGetStrileOffsetWithErr(tif, 0, &err_code);
    (void)strile_off;
    (void)err_code;
    
    // Try with a large index derived from input if available
    if (Size >= 4) {
        uint32_t index = 0;
        memcpy(&index, Data, 4);
        strile_off = TIFFGetStrileOffsetWithErr(tif, index, &err_code);
        (void)strile_off;
    }

    // 6. Test TIFFSwabLong8
    // Create a dummy 64-bit value and swap it
    uint64_t val = 0x123456789ABCDEF0ULL;
    TIFFSwabLong8(&val);
    (void)val;

    // Cleanup
    TIFFClose(tif);

    return 0;
}