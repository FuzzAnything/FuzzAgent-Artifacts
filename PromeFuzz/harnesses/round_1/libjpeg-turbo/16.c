// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    write_dummy_file(Data, Size);
    
    int num_factors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&num_factors);
    if (factors && num_factors > 0) {
        tjscalingfactor test_factor = factors[0];
        
        int num_factors_legacy = 0;
        tjscalingfactor *factors_legacy = tjGetScalingFactors(&num_factors_legacy);
    }
    
    tj3GetScalingFactors(NULL);
    
    tjhandle handle = tjInitDecompress();
    if (handle) {
        int width = 0, height = 0;
        int header_result = tjDecompressHeader(handle, (unsigned char *)Data, 
                                              (unsigned long)Size, &width, &height);
        
        if (header_result == 0 && width > 0 && height > 0) {
            int num_scaling = 0;
            tjscalingfactor *scaling_list = tj3GetScalingFactors(&num_scaling);
            if (scaling_list && num_scaling > 0) {
                int scaling_idx = Size > 0 ? Data[0] % num_scaling : 0;
                tj3SetScalingFactor(handle, scaling_list[scaling_idx]);
                
                int scaled_width = TJSCALED(width, scaling_list[scaling_idx]);
                int scaled_height = TJSCALED(height, scaling_list[scaling_idx]);
                
                if (scaled_width > 0 && scaled_height > 0) {
                    tjregion crop_region;
                    if (Size >= 16) {
                        crop_region.x = Data[1] % scaled_width;
                        crop_region.y = Data[2] % scaled_height;
                        crop_region.w = Data[3] % (scaled_width - crop_region.x + 1);
                        crop_region.h = Data[4] % (scaled_height - crop_region.y + 1);
                        
                        if (crop_region.w > 0 && crop_region.h > 0 &&
                            crop_region.x + crop_region.w <= scaled_width &&
                            crop_region.y + crop_region.h <= scaled_height) {
                            tj3SetCroppingRegion(handle, crop_region);
                        }
                    }
                    
                    tj3SetCroppingRegion(handle, TJUNCROPPED);
                    
                    int pixel_format = TJPF_RGB;
                    int pitch = scaled_width * tjPixelSize[pixel_format];
                    unsigned long dst_size = (unsigned long)pitch * scaled_height;
                    unsigned char *dst_buf = (unsigned char *)malloc(dst_size);
                    
                    if (dst_buf) {
                        int flags = 0;
                        if (Size > 5) {
                            flags = Data[5] & (TJFLAG_FASTDCT | TJFLAG_ACCURATEDCT | 
                                              TJFLAG_FASTUPSAMPLE | TJFLAG_BOTTOMUP);
                        }
                        
                        tjDecompress2(handle, (unsigned char *)Data, (unsigned long)Size,
                                     dst_buf, scaled_width, pitch, scaled_height, 
                                     pixel_format, flags);
                        
                        free(dst_buf);
                    }
                }
            }
        }
        
        tjDestroy(handle);
    }
    
    if (Size > 0) {
        tjhandle invalid_handle = NULL;
        int dummy_width, dummy_height;
        unsigned char dummy_buf[100];
        
        tjDecompressHeader(invalid_handle, (unsigned char *)Data, 
                          (unsigned long)Size, &dummy_width, &dummy_height);
        
        if (Size > 10) {
            tjDecompress2(invalid_handle, (unsigned char *)Data, (unsigned long)Size,
                         dummy_buf, 10, 30, 10, TJPF_GRAY, 0);
        }
    }
    
    remove_dummy_file();
    
    return 0;
}