// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    uint16_t resolution_unit = RESUNIT_INCH;
    float x_resolution = 72.0;
    float y_resolution = 72.0;
    char* software = "Fuzz Test";
    char* datetime = "2024:01:01 00:00:00";
    char* artist = "Fuzzer";
    char* host_computer = "Test Machine";
    char* image_description = "Fuzzed Image";
    char* make = "FuzzCorp";
    char* model = "FuzzCam";
    float white_point[2] = {0.3127, 0.3290};
    float primary_chromaticities[6] = {0.64, 0.33, 0.30, 0.60, 0.15, 0.06};
    
    uint16_t *transfer_function_red = (uint16_t*)malloc(256 * sizeof(uint16_t));
    uint16_t *transfer_function_green = (uint16_t*)malloc(256 * sizeof(uint16_t));
    uint16_t *transfer_function_blue = (uint16_t*)malloc(256 * sizeof(uint16_t));
    
    if (transfer_function_red && transfer_function_green && transfer_function_blue) {
        for (int i = 0; i < 256; i++) {
            transfer_function_red[i] = i;
            transfer_function_green[i] = i;
            transfer_function_blue[i] = i;
        }
    }
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    TIFFSetField(tif, TIFFTAG_XRESOLUTION, x_resolution);
    TIFFSetField(tif, TIFFTAG_YRESOLUTION, y_resolution);
    TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, resolution_unit);
    TIFFSetField(tif, TIFFTAG_SOFTWARE, software);
    TIFFSetField(tif, TIFFTAG_DATETIME, datetime);
    TIFFSetField(tif, TIFFTAG_ARTIST, artist);
    TIFFSetField(tif, TIFFTAG_HOSTCOMPUTER, host_computer);
    TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, image_description);
    TIFFSetField(tif, TIFFTAG_MAKE, make);
    TIFFSetField(tif, TIFFTAG_MODEL, model);
    TIFFSetField(tif, TIFFTAG_WHITEPOINT, white_point);
    TIFFSetField(tif, TIFFTAG_PRIMARYCHROMATICITIES, primary_chromaticities);
    
    if (transfer_function_red && transfer_function_green && transfer_function_blue) {
        TIFFSetField(tif, TIFFTAG_TRANSFERFUNCTION, 
                     transfer_function_red, transfer_function_green, transfer_function_blue);
    }
    
    const TIFFField* field = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);
    if (field) {
        TIFFSetField(tif, TIFFTAG_SUBFILETYPE, 0);
        TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    }
    
    uint8_t* scanline = (uint8_t*)malloc(width * samples_per_pixel);
    if (scanline) {
        size_t copy_size = width * samples_per_pixel;
        if (copy_size > Size) copy_size = Size;
        memcpy(scanline, Data, copy_size);
        TIFFWriteScanline(tif, scanline, 0, 0);
        free(scanline);
    }
    
    TIFFWriteDirectory(tif);
    
    TIFFCreateGPSDirectory(tif);
    
    uint8_t gps_version[4] = {2, 2, 0, 0};
    char* latitude_ref = "N";
    float latitude[3] = {40.0, 30.0, 0.0};
    char* longitude_ref = "W";
    float longitude[3] = {74.0, 0.0, 0.0};
    uint8_t altitude_ref = 0;
    float altitude = 100.0;
    float gps_time[3] = {12.0, 30.0, 0.0};
    char* satellites = "12";
    char* status = "A";
    char* measure_mode = "3";
    float dop = 2.5;
    char* speed_ref = "K";
    float speed = 50.0;
    char* track_ref = "T";
    float track = 180.0;
    char* img_direction_ref = "M";
    float img_direction = 90.0;
    char* map_datum = "WGS-84";
    char* dest_lat_ref = "S";
    float dest_lat[3] = {34.0, 0.0, 0.0};
    char* dest_long_ref = "E";
    float dest_long[3] = {151.0, 0.0, 0.0};
    char* dest_bearing_ref = "T";
    float dest_bearing = 45.0;
    char* dest_distance_ref = "K";
    float dest_distance = 1000.0;
    
    TIFFSetField(tif, 0, gps_version);
    TIFFSetField(tif, 1, latitude_ref);
    TIFFSetField(tif, 2, latitude);
    TIFFSetField(tif, 3, longitude_ref);
    TIFFSetField(tif, 4, longitude);
    TIFFSetField(tif, 5, altitude_ref);
    TIFFSetField(tif, 6, altitude);
    TIFFSetField(tif, 7, gps_time);
    TIFFSetField(tif, 8, satellites);
    TIFFSetField(tif, 9, status);
    TIFFSetField(tif, 10, measure_mode);
    TIFFSetField(tif, 11, dop);
    TIFFSetField(tif, 12, speed_ref);
    TIFFSetField(tif, 13, speed);
    TIFFSetField(tif, 14, track_ref);
    TIFFSetField(tif, 15, track);
    TIFFSetField(tif, 16, img_direction_ref);
    TIFFSetField(tif, 17, img_direction);
    TIFFSetField(tif, 18, map_datum);
    TIFFSetField(tif, 19, dest_lat_ref);
    TIFFSetField(tif, 20, dest_lat);
    TIFFSetField(tif, 21, dest_long_ref);
    TIFFSetField(tif, 22, dest_long);
    TIFFSetField(tif, 23, dest_bearing_ref);
    TIFFSetField(tif, 24, dest_bearing);
    TIFFSetField(tif, 25, dest_distance_ref);
    TIFFSetField(tif, 26, dest_distance);
    
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);
    
    if (transfer_function_red) free(transfer_function_red);
    if (transfer_function_green) free(transfer_function_green);
    if (transfer_function_blue) free(transfer_function_blue);
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}