// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    if (data && size > 0) {
        write(fd, data, size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
        // Set minimal required tags for a valid TIFF
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        if (TIFFWriteDirectory(tif) <= 0) {
            TIFFClose(tif);
            return 0;
        }
        TIFFClose(tif);
        // Reopen for reading/writing
        tif = TIFFOpen("./dummy_file", "r+");
        if (!tif) {
            return 0;
        }
    }

    // Fuzz TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset;

    // Fuzz TIFFSetSubDirectory with various offsets derived from input
    if (Size >= 8) {
        uint64_t sub_offset = 0;
        memcpy(&sub_offset, Data, 8);
        TIFFSetSubDirectory(tif, sub_offset);
    } else if (Size >= 4) {
        uint32_t sub_offset_32 = 0;
        memcpy(&sub_offset_32, Data, 4);
        TIFFSetSubDirectory(tif, (uint64_t)sub_offset_32);
    } else {
        TIFFSetSubDirectory(tif, 0);
    }

    // Fuzz TIFFSetDirectory with various indices
    if (Size >= 4) {
        tdir_t dir_idx = 0;
        memcpy(&dir_idx, Data, 4);
        TIFFSetDirectory(tif, dir_idx);
    } else if (Size >= 2) {
        tdir_t dir_idx = 0;
        memcpy(&dir_idx, Data, 2);
        TIFFSetDirectory(tif, dir_idx);
    } else {
        TIFFSetDirectory(tif, 0);
    }

    // Fuzz TIFFGetStrileOffset
    if (Size >= 4) {
        uint32_t strile = 0;
        memcpy(&strile, Data, 4);
        uint64_t strile_off = TIFFGetStrileOffset(tif, strile);
        (void)strile_off;
    } else if (Size >= 2) {
        uint16_t strile = 0;
        memcpy(&strile, Data, 2);
        uint64_t strile_off = TIFFGetStrileOffset(tif, (uint32_t)strile);
        (void)strile_off;
    } else {
        uint64_t strile_off = TIFFGetStrileOffset(tif, 0);
        (void)strile_off;
    }

    // Fuzz TIFFSetWriteOffset
    if (Size >= 8) {
        toff_t write_off = 0;
        memcpy(&write_off, Data, 8);
        TIFFSetWriteOffset(tif, write_off);
    } else if (Size >= 4) {
        uint32_t write_off_32 = 0;
        memcpy(&write_off_32, Data, 4);
        TIFFSetWriteOffset(tif, (toff_t)write_off_32);
    } else {
        TIFFSetWriteOffset(tif, 0);
    }

    // Fuzz TIFFRewriteDirectory
    int ret = TIFFRewriteDirectory(tif);
    (void)ret;

    TIFFClose(tif);
    return 0;
}