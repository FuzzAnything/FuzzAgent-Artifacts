// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "libtiff/tiffio.h"

static int write_data_to_file(const uint8_t *data, size_t size, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    const char *filename = "./dummy_file";
    if (!write_data_to_file(Data, Size, filename)) return 0;
    
    TIFF *tif = TIFFOpen(filename, "r+");
    if (!tif) {
        tif = TIFFOpen(filename, "w");
        if (!tif) return 0;
    }
    
    uint32_t tag_value;
    uint32_t width, height;
    uint16_t bits_per_sample, samples_per_pixel;
    uint32_t rows_per_strip;
    uint32_t strip_size;
    uint64_t subdir_offset;
    tdir_t dir_num, current_dir;
    int ret;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &strip_size);
    
    subdir_offset = (uint64_t)Data[0] << 56 | (uint64_t)Data[1] << 48 | 
                    (uint64_t)Data[2] << 40 | (uint64_t)Data[3] << 32 |
                    (uint64_t)Data[4] << 24 | (uint64_t)Data[5] << 16 |
                    (uint64_t)Data[6] << 8 | Data[7];
    TIFFSetSubDirectory(tif, subdir_offset);
    
    TIFFReadDirectory(tif);
    
    dir_num = Data[8] % 10;
    TIFFSetDirectory(tif, dir_num);
    
    subdir_offset = (uint64_t)Data[9] << 56 | (uint64_t)Data[10] << 48 | 
                    (uint64_t)Data[11] << 40 | (uint64_t)Data[12] << 32 |
                    (uint64_t)Data[13] << 24 | (uint64_t)Data[14] << 16 |
                    (uint64_t)Data[15] << 8 | Data[16];
    TIFFSetSubDirectory(tif, subdir_offset);
    
    TIFFReadDirectory(tif);
    
    dir_num = Data[17] % 10;
    TIFFSetDirectory(tif, dir_num);
    
    TIFFReadDirectory(tif);
    
    current_dir = TIFFCurrentDirectory(tif);
    
    subdir_offset = (uint64_t)Data[18] << 56 | (uint64_t)Data[19] << 48 | 
                    (uint64_t)Data[20] << 40 | (uint64_t)Data[21] << 32 |
                    (uint64_t)Data[22] << 24 | (uint64_t)Data[23] << 16 |
                    (uint64_t)Data[24] << 8 | Data[25];
    TIFFSetSubDirectory(tif, subdir_offset);
    
    dir_num = TIFFNumberOfDirectories(tif);
    
    TIFFCreateDirectory(tif);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)Data[26] << 24 | Data[27] << 16 | Data[28] << 8 | Data[29]);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)Data[30] << 24 | Data[31] << 16 | Data[32] << 8 | Data[33]);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)(Data[34] % 16 + 1));
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)(Data[35] % 5 + 1));
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (uint32_t)Data[36] << 24 | Data[37] << 16 | Data[38] << 8 | Data[39]);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    
    TIFFWriteDirectory(tif);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)Data[40] << 24 | Data[41] << 16 | Data[42] << 8 | Data[43]);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)Data[44] << 24 | Data[45] << 16 | Data[46] << 8 | Data[47]);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFClose(tif);
    
    TIFFClose(tif);
    
    remove(filename);
    return 0;
}