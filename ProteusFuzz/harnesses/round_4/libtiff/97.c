// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
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
    
    // Write a minimal TIFF header
    uint16_t byte_order = 0x4949; // Little endian
    uint16_t magic = 42;
    uint32_t ifd_offset = 8;
    
    fwrite(&byte_order, sizeof(byte_order), 1, f);
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&ifd_offset, sizeof(ifd_offset), 1, f);
    
    // Write a minimal IFD (Image File Directory)
    // We need at least one entry to make it valid for reading
    uint16_t num_entries = 1;
    uint16_t tag = 256; // ImageWidth
    uint16_t type = 3;  // SHORT
    uint32_t count = 1;
    uint32_t value = 100; // Width
    
    fwrite(&num_entries, sizeof(num_entries), 1, f);
    fwrite(&tag, sizeof(tag), 1, f);
    fwrite(&type, sizeof(type), 1, f);
    fwrite(&count, sizeof(count), 1, f);
    fwrite(&value, sizeof(value), 1, f);
    
    // Next IFD offset (0 means no more IFDs)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(next_ifd), 1, f);
    
    // Write some dummy image data (strip data)
    // Ensure we have enough data to simulate a strip
    size_t data_to_write = (Size > 0) ? Size : 1024;
    const uint8_t *src = (Size > 0) ? Data : (const uint8_t *)"\x00";
    size_t len = (Size > 0) ? Size : 1;
    
    for (size_t i = 0; i < data_to_write; i++) {
        fputc(src[i % len], f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment
    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Fuzzing logic
    uint32_t nrows = 0;
    uint32_t strip = 0;
    uint32_t strip_count = 0;
    
    // Extract parameters from input if available
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
        strip = *(const uint32_t *)(Data + 4);
    }
    
    // Get strip count to bound strip index
    // TIFFTAG_STRIPCOUNT is defined in tiffio.h as 279
    if (TIFFGetField(tif, 279, &strip_count) != 0) {
        // Successfully retrieved strip count
    } else {
        // Default to a small number if we can't determine
        strip_count = 1;
    }

    // Ensure strip index is within bounds
    if (strip_count > 0 && strip >= strip_count) {
        strip = strip % strip_count;
    }

    // Allocate buffer for reading
    size_t buf_size = 65536;
    if (Size > buf_size) {
        buf_size = Size;
    }
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }

    // 1. TIFFVStripSize
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
    if (vstrip_size > 0 && vstrip_size < buf_size) {
        // Use the calculated size
    }

    // 2. TIFFRawStripSize
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip);
    if (raw_strip_size > 0 && raw_strip_size < buf_size) {
        // Use the calculated size
    }

    // 3. TIFFReadEncodedStrip
    tmsize_t bytes_read = TIFFReadEncodedStrip(tif, strip, buf, buf_size);
    if (bytes_read < 0) {
        // Error occurred, handle gracefully
    }

    // 4. TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0 && strip_size < buf_size) {
        // Use the calculated size
    }

    // 5. TIFFReadRawStrip
    bytes_read = TIFFReadRawStrip(tif, strip, buf, buf_size);
    if (bytes_read < 0) {
        // Error occurred, handle gracefully
    }

    // 6. TIFFWriteRawStrip (requires write mode, so we skip this for read-only file)
    // To test this, we would need to open in write mode, but for safety in fuzzing,
    // we skip writing to avoid corrupting the test file or causing side effects.
    // If needed, we could open a separate file for writing.

    // Cleanup
    free(buf);
    TIFFClose(tif);
    
    return 0;
}