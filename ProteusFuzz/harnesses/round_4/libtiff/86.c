// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

/* 
 * Fuzz driver for libtiff API functions.
 * Targets: TIFFSwabArrayOfLong8, _TIFFmemcpy, TIFFSwabLong, 
 *          TIFFReadRawStrip, TIFFReverseBits, TIFFSwabArrayOfLong
 */

static void fuzz_swab_long8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    
    /* Allocate buffer to hold at least one uint64_t */
    size_t count = Size / sizeof(uint64_t);
    if (count == 0) return;
    
    uint64_t *buf = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(uint64_t));
    
    /* Call target function */
    TIFFSwabArrayOfLong8(buf, count);
    
    free(buf);
}

static void fuzz_memcpy(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    /* Ensure we have enough data for source and destination */
    size_t copy_size = Size / 2;
    if (copy_size == 0) return;
    
    void *src = malloc(copy_size);
    void *dst = malloc(copy_size);
    if (!src || !dst) {
        free(src);
        free(dst);
        return;
    }
    
    memcpy(src, Data, copy_size);
    
    /* Call target function */
    _TIFFmemcpy(dst, src, copy_size);
    
    free(src);
    free(dst);
}

static void fuzz_swab_long(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    
    uint32_t val;
    memcpy(&val, Data, sizeof(uint32_t));
    
    /* Call target function */
    TIFFSwabLong(&val);
    
    /* Use the result to prevent optimization */
    volatile uint32_t _ = val;
}

static void fuzz_read_raw_strip(const uint8_t *Data, size_t Size) {
    /* Create a dummy file with minimal TIFF header */
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    /* Write minimal TIFF header (BigTIFF or Classic) */
    /* Classic TIFF: 2 bytes magic, 2 bytes offset to first IFD */
    uint16_t magic = 0x4949; /* Little-endian */
    uint16_t offset = 8;
    
    fwrite(&magic, sizeof(uint16_t), 1, f);
    fwrite(&offset, sizeof(uint16_t), 1, f);
    
    /* Write minimal IFD: 2 bytes count, rest padding to reach offset 8 */
    uint16_t count = 0;
    fwrite(&count, sizeof(uint16_t), 1, f);
    fwrite(&offset, sizeof(uint16_t), 1, f); /* Next IFD offset (0) */
    
    /* If we have more data, write it as strip data */
    if (Size > 16) {
        fwrite(Data + 16, 1, Size - 16, f);
    }
    
    fclose(f);
    
    /* Open the file */
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) return;
    
    /* Allocate buffer for reading */
    size_t buf_size = Size > 1024 ? 1024 : Size;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (!buf) {
        TIFFClose(tif);
        return;
    }
    
    /* Try to read strip 0 */
    TIFFReadRawStrip(tif, 0, buf, buf_size);
    
    free(buf);
    TIFFClose(tif);
    
    /* Clean up dummy file */
    remove(filename);
}

static void fuzz_reverse_bits(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    uint8_t *buf = (uint8_t *)malloc(Size);
    if (!buf) return;
    
    memcpy(buf, Data, Size);
    
    /* Call target function */
    TIFFReverseBits(buf, Size);
    
    free(buf);
}

static void fuzz_swab_array_long(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    
    size_t count = Size / sizeof(uint32_t);
    if (count == 0) return;
    
    uint32_t *buf = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(uint32_t));
    
    /* Call target function */
    TIFFSwabArrayOfLong(buf, count);
    
    free(buf);
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Fuzz TIFFSwabArrayOfLong8 */
    fuzz_swab_long8(Data, Size);
    
    /* Fuzz _TIFFmemcpy */
    fuzz_memcpy(Data, Size);
    
    /* Fuzz TIFFSwabLong */
    fuzz_swab_long(Data, Size);
    
    /* Fuzz TIFFReadRawStrip */
    fuzz_read_raw_strip(Data, Size);
    
    /* Fuzz TIFFReverseBits */
    fuzz_reverse_bits(Data, Size);
    
    /* Fuzz TIFFSwabArrayOfLong */
    fuzz_swab_array_long(Data, Size);
    
    return 0;
}