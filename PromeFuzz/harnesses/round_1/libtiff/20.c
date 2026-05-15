// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint32_t rows_per_strip = 1;
    float x_resolution = 72.0;
    float y_resolution = 72.0;
    uint16_t resolution_unit = RESUNIT_INCH;
    char *software = "Fuzz Test";
    char *datetime = "2024:01:01 00:00:00";
    float primary_chromaticities[6] = {0.64, 0.33, 0.30, 0.60, 0.15, 0.06};
    float white_point[2] = {0.3127, 0.3290};
    uint16_t transfer_function[3][256];
    uint16_t *colormap[3];
    uint16_t extra_samples = 0;
    uint16_t sample_format = SAMPLEFORMAT_UINT;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    uint16_t fill_order = FILLORDER_MSB2LSB;
    uint32_t subfile_type = 0;
    uint32_t image_depth = 1;
    uint32_t tile_depth = 1;
    uint32_t tile_width = 1;
    uint32_t tile_length = 1;
    uint32_t sub_ifd_count = 0;
    uint64_t *sub_ifd_offsets = NULL;
    uint16_t predictor = PREDICTOR_NONE;
    uint16_t ink_set = INKSET_CMYK;
    uint16_t number_of_inks = 4;
    uint8_t *ink_names = NULL;
    uint16_t dot_range[2] = {0, 255};
    uint16_t target_printer = 0;
    uint16_t page_number[2] = {0, 1};
    float x_position = 0.0;
    float y_position = 0.0;
    uint32_t free_offsets = 0;
    uint32_t free_byte_counts = 0;
    uint32_t gray_response_curve[256];
    uint16_t gray_response_unit = 2;
    uint32_t color_response_curve[3][256];
    uint16_t color_response_unit = 2;
    
    for (int i = 0; i < 3; i++) {
        colormap[i] = (uint16_t*)malloc(256 * sizeof(uint16_t));
        if (!colormap[i]) {
            for (int j = 0; j < i; j++) free(colormap[j]);
            TIFFClose(tif);
            remove_dummy_file();
            return 0;
        }
    }
    
    for (int i = 0; i < 256; i++) {
        transfer_function[0][i] = i;
        transfer_function[1][i] = i;
        transfer_function[2][i] = i;
        for (int j = 0; j < 3; j++) {
            colormap[j][i] = i;
        }
        gray_response_curve[i] = i;
        color_response_curve[0][i] = i;
        color_response_curve[1][i] = i;
        color_response_curve[2][i] = i;
    }
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, x_resolution);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, y_resolution);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, resolution_unit);
    TIFFSetField(tif, TIFFTAG_SOFTWARE, software);
    TIFFSetField(tif, TIFFTAG_DATETIME, datetime);
    TIFFSetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, primary_chromaticities);
    TIFFSetField(tif, TIFFTAG_WHITEPOINT, white_point);
    TIFFSetField(tif, TIFFTAG_TRANSFERFUNCTION, transfer_function);
    TIFFSetField(tif, TIFFTAG_COLORMAP, colormap[0], colormap[1], colormap[2]);
    TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, extra_samples, &extra_samples);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sample_format);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    
    const TIFFField *field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    
    TIFFSetField(tif, TIFFTAG_FILLORDER, fill_order);
    TIFFSetField(tif, TIFFTAG_SUBFILETYPE, subfile_type);
    
    uint8_t scanline[1] = {Data[0] % 256};
    TIFFWriteScanline(tif, scanline, 0, 0);
    
    TIFFWriteDirectory(tif);
    
    TIFFCreateGPSDirectory(tif);
    
    char *gps_version_id = "\x02\x02\x00\x00";
    char *gps_latitude_ref = "N";
    float gps_latitude[3] = {40.0, 30.0, 0.0};
    char *gps_longitude_ref = "W";
    float gps_longitude[3] = {74.0, 0.0, 0.0};
    uint16_t gps_altitude_ref = 0;
    float gps_altitude = 0.0;
    float gps_time_stamp[3] = {12.0, 30.0, 0.0};
    char *gps_satellites = "12";
    char *gps_status = "A";
    char *gps_measure_mode = "3";
    float gps_dop = 1.0;
    char *gps_speed_ref = "K";
    float gps_speed = 0.0;
    char *gps_track_ref = "T";
    float gps_track = 0.0;
    char *gps_img_direction_ref = "T";
    float gps_img_direction = 0.0;
    char *gps_map_datum = "WGS-84";
    char *gps_dest_latitude_ref = "N";
    float gps_dest_latitude[3] = {40.0, 30.0, 0.0};
    char *gps_dest_longitude_ref = "W";
    float gps_dest_longitude[3] = {74.0, 0.0, 0.0};
    char *gps_dest_bearing_ref = "T";
    float gps_dest_bearing = 0.0;
    char *gps_dest_distance_ref = "K";
    float gps_dest_distance = 0.0;
    char *gps_processing_method = "GPS";
    char *gps_area_information = "Area";
    char *gps_date_stamp = "2024:01:01";
    uint16_t gps_differential = 0;
    
    TIFFSetField(tif, 0x8825, gps_version_id);
    TIFFSetField(tif, 0x8826, gps_latitude_ref);
    TIFFSetField(tif, 0x8827, gps_latitude);
    TIFFSetField(tif, 0x8828, gps_longitude_ref);
    TIFFSetField(tif, 0x8829, gps_longitude);
    TIFFSetField(tif, 0x882A, gps_altitude_ref);
    TIFFSetField(tif, 0x882B, gps_altitude);
    TIFFSetField(tif, 0x882C, gps_time_stamp);
    TIFFSetField(tif, 0x882D, gps_satellites);
    TIFFSetField(tif, 0x882E, gps_status);
    TIFFSetField(tif, 0x882F, gps_measure_mode);
    TIFFSetField(tif, 0x8830, gps_dop);
    TIFFSetField(tif, 0x8831, gps_speed_ref);
    TIFFSetField(tif, 0x8832, gps_speed);
    TIFFSetField(tif, 0x8833, gps_track_ref);
    TIFFSetField(tif, 0x8834, gps_track);
    TIFFSetField(tif, 0x8835, gps_img_direction_ref);
    TIFFSetField(tif, 0x8836, gps_img_direction);
    TIFFSetField(tif, 0x8837, gps_map_datum);
    TIFFSetField(tif, 0x8838, gps_dest_latitude_ref);
    TIFFSetField(tif, 0x8839, gps_dest_latitude);
    TIFFSetField(tif, 0x883A, gps_dest_longitude_ref);
    TIFFSetField(tif, 0x883B, gps_dest_longitude);
    TIFFSetField(tif, 0x883C, gps_dest_bearing_ref);
    TIFFSetField(tif, 0x883D, gps_dest_bearing);
    TIFFSetField(tif, 0x883E, gps_dest_distance_ref);
    TIFFSetField(tif, 0x883F, gps_dest_distance);
    TIFFSetField(tif, 0x8840, gps_processing_method);
    TIFFSetField(tif, 0x8841, gps_area_information);
    TIFFSetField(tif, 0x8842, gps_date_stamp);
    TIFFSetField(tif, 0x8843, gps_differential);
    
    uint64_t custom_dir_offset = 0;
    TIFFWriteCustomDirectory(tif, &custom_dir_offset);
    
    for (int i = 0; i < 3; i++) {
        free(colormap[i]);
    }
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}