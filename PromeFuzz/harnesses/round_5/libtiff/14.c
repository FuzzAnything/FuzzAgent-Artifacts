// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width, height;
    uint16_t bits_per_sample, samples_per_pixel, photometric;
    uint32_t rows_per_strip;
    float x_resolution, y_resolution;
    uint16_t resolution_unit;
    char* software = NULL;
    char* datetime = NULL;
    char* artist = NULL;
    char* host_computer = NULL;
    uint16_t* sample_format = NULL;
    uint32_t* strip_offsets = NULL;
    uint32_t* strip_byte_counts = NULL;
    uint32_t* tile_offsets = NULL;
    uint32_t* tile_byte_counts = NULL;
    uint16_t planar_config;
    uint16_t orientation;
    uint16_t min_sample_value, max_sample_value;
    float x_position, y_position;
    uint16_t* ink_names = NULL;
    uint32_t number_of_inks;
    uint16_t* dot_range = NULL;
    char* target_printer = NULL;
    uint16_t* extra_samples = NULL;
    float* smin_sample_value = NULL;
    float* smax_sample_value = NULL;
    void* jpeg_tables = NULL;
    uint16_t jpeg_proc;
    uint16_t jpeg_restart_interval;
    uint16_t* jpeg_lossless_predictors = NULL;
    uint16_t* jpeg_point_transform = NULL;
    void* jpeg_q_tables = NULL;
    void* jpeg_dc_tables = NULL;
    void* jpeg_ac_tables = NULL;
    float* ycbcr_coefficients = NULL;
    uint16_t* ycbcr_subsampling = NULL;
    uint16_t ycbcr_positioning;
    float* reference_black_white = NULL;
    uint32_t subifd_count;
    uint64_t* subifd_offsets = NULL;
    uint32_t gps_offset;
    uint32_t exif_offset;
    tdir_t dir_count;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &x_resolution);
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &y_resolution);
    TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &resolution_unit);
    
    if (TIFFGetField(tif, TIFFTAG_GPSIFD, &gps_offset)) {
        TIFFReadGPSDirectory(tif, gps_offset);
    }
    
    TIFFGetField(tif, TIFFTAG_SOFTWARE, &software);
    TIFFGetField(tif, TIFFTAG_DATETIME, &datetime);
    TIFFGetField(tif, TIFFTAG_ARTIST, &artist);
    TIFFGetField(tif, TIFFTAG_HOSTCOMPUTER, &host_computer);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    TIFFGetField(tif, TIFFTAG_MINSAMPLEVALUE, &min_sample_value);
    TIFFGetField(tif, TIFFTAG_MAXSAMPLEVALUE, &max_sample_value);
    TIFFGetField(tif, TIFFTAG_XPOSITION, &x_position);
    TIFFGetField(tif, TIFFTAG_YPOSITION, &y_position);
    
    TIFFSetDirectory(tif, 0);
    
    TIFFGetField(tif, TIFFTAG_INKNAMES, &ink_names);
    
    if (TIFFGetField(tif, TIFFTAG_EXIFIFD, &exif_offset)) {
        TIFFReadEXIFDirectory(tif, exif_offset);
    }
    
    TIFFGetField(tif, TIFFTAG_NUMBEROFINKS, &number_of_inks);
    TIFFGetField(tif, TIFFTAG_DOTRANGE, &dot_range);
    TIFFGetField(tif, TIFFTAG_TARGETPRINTER, &target_printer);
    TIFFGetField(tif, TIFFTAG_EXTRASAMPLES, &extra_samples);
    TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sample_format);
    TIFFGetField(tif, TIFFTAG_SMINSAMPLEVALUE, &smin_sample_value);
    TIFFGetField(tif, TIFFTAG_SMAXSAMPLEVALUE, &smax_sample_value);
    TIFFGetField(tif, TIFFTAG_JPEGTABLES, &jpeg_tables);
    TIFFGetField(tif, TIFFTAG_JPEGPROC, &jpeg_proc);
    TIFFGetField(tif, TIFFTAG_JPEGRESTARTINTERVAL, &jpeg_restart_interval);
    
    const TIFFField* field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    
    TIFFGetField(tif, TIFFTAG_JPEGLOSSLESSPREDICTORS, &jpeg_lossless_predictors);
    TIFFGetField(tif, TIFFTAG_JPEGPOINTTRANSFORM, &jpeg_point_transform);
    TIFFGetField(tif, TIFFTAG_JPEGQTABLES, &jpeg_q_tables);
    TIFFGetField(tif, TIFFTAG_JPEGDCTABLES, &jpeg_dc_tables);
    TIFFGetField(tif, TIFFTAG_JPEGACTABLES, &jpeg_ac_tables);
    TIFFGetField(tif, TIFFTAG_YCBCRCOEFFICIENTS, &ycbcr_coefficients);
    TIFFGetField(tif, TIFFTAG_YCBCRSUBSAMPLING, &ycbcr_subsampling);
    TIFFGetField(tif, TIFFTAG_YCBCRPOSITIONING, &ycbcr_positioning);
    TIFFGetField(tif, TIFFTAG_REFERENCEBLACKWHITE, &reference_black_white);
    TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &strip_offsets);
    TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &strip_byte_counts);
    TIFFGetField(tif, TIFFTAG_TILEOFFSETS, &tile_offsets);
    TIFFGetField(tif, TIFFTAG_TILEBYTECOUNTS, &tile_byte_counts);
    TIFFGetField(tif, TIFFTAG_SUBIFD, &subifd_count, &subifd_offsets);
    
    TIFFClose(tif);
    
    TIFF* tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        TIFFClose(tif2);
    }
    
    cleanup_dummy_file();
    return 0;
}