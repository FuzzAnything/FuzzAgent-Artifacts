#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <turbojpeg.h>
}
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["tj3InitVersion","tj3SetScalingFactor","tj3Destroy","tjDecompressHeader3","tj3Destroy","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjFree","tj3Destroy","tjDecompressToYUVPlanes","tjFree","tj3Destroy","tj3LoadImage12","tj3Free","tj3LoadImage16","tj3SaveImage16","tj3Free","tjFree","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3SetScalingFactor","tjDecompressHeader3","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjDecompressToYUVPlanes","tj3LoadImage12","tj3Free","tj3LoadImage16","tj3SaveImage16","tj3Free","tjFree","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize decompressor handle
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // Set scaling factor (scale down by half)
    tjscalingfactor scaling_factor;
    scaling_factor.num = 1;
    scaling_factor.denom = 2;
    int result = tj3SetScalingFactor(handle, scaling_factor);
    if (result != 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Decompress JPEG header to get image dimensions
    int width, height, jpegSubsamp, jpegColorspace;
    unsigned long jpeg_size = static_cast<unsigned long>(size);
    
    if (tjDecompressHeader3(handle, data, jpeg_size, &width, &height,
                           &jpegSubsamp, &jpegColorspace) != 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Calculate buffer size for YUV planes
    int strides[3];
    unsigned char *dst_planes[3] = {nullptr, nullptr, nullptr};
    
    // Allocate memory for YUV planes
    for (int i = 0; i < 3; i++) {
        strides[i] = tjPlaneWidth(i, width, jpegSubsamp);
        int plane_height = tjPlaneHeight(i, height, jpegSubsamp);
        size_t plane_size = tjPlaneSizeYUV(i, width, strides[i], plane_height, jpegSubsamp);
        dst_planes[i] = static_cast<unsigned char*>(tjAlloc(plane_size));
        if (!dst_planes[i]) {
            for (int j = 0; j < i; j++) {
                tjFree(dst_planes[j]);
            }
            tj3Destroy(handle);
            return 0;
        }
    }

    // Decompress JPEG to YUV planes
    result = tjDecompressToYUVPlanes(handle, data, jpeg_size,
                                    dst_planes, width, strides,
                                    height, 0);
    if (result != 0) {
        for (int i = 0; i < 3; i++) {
            tjFree(dst_planes[i]);
        }
        tj3Destroy(handle);
        return 0;
    }

    // Create temporary file names
    const char* input12_file = "input12_file";
    const char* input16_file = "input16_file";
    const char* output16_file = "output16_file";

    // Create dummy 12-bit and 16-bit image files for loading
    FILE* in12 = fopen(input12_file, "wb");
    if (in12) {
        // Write minimal PPM header for 12-bit image (small 2x2 image)
        const char* ppm12_header = "P6\n2 2\n4095\n";
        fwrite(ppm12_header, 1, strlen(ppm12_header), in12);
        // Write dummy 12-bit pixel data (2x2x3 pixels, 2 bytes per component)
        uint16_t dummy12_pixels[12] = {0};
        for (int i = 0; i < 12; i++) {
            dummy12_pixels[i] = 2048; // Mid-value for 12-bit
        }
        fwrite(dummy12_pixels, 2, 12, in12);
        assert_file_closed(&in12);;
    }

    FILE* in16 = fopen(input16_file, "wb");
    if (in16) {
        // Write minimal PPM header for 16-bit image (small 2x2 image)
        const char* ppm16_header = "P6\n2 2\n65535\n";
        fwrite(ppm16_header, 1, strlen(ppm16_header), in16);
        // Write dummy 16-bit pixel data (2x2x3 pixels, 2 bytes per component)
        uint16_t dummy16_pixels[12] = {0};
        for (int i = 0; i < 12; i++) {
            dummy16_pixels[i] = 32768; // Mid-value for 16-bit
        }
        fwrite(dummy16_pixels, 2, 12, in16);
        assert_file_closed(&in16);;
    }

    // Load 12-bit image (even though we don't use it further)
    int width12, height12, pixelFormat12;
    short* image12 = tj3LoadImage12(handle, input12_file, &width12, 0, 
                                   &height12, &pixelFormat12);
    if (image12) {
        tj3Free(image12);
    }

    // Load 16-bit image
    int width16, height16, pixelFormat16;
    unsigned short* image16 = tj3LoadImage16(handle, input16_file, &width16, 0,
                                           &height16, &pixelFormat16);
    
    if (image16) {
        // Save the loaded 16-bit image
        result = tj3SaveImage16(handle, output16_file, image16, width16, 
                              width16 * tjPixelSize[pixelFormat16],
                              height16, pixelFormat16);
        
        tj3Free(image16);
        
        // Clean up output file
        remove(output16_file);
    }

    // Clean up input files
    remove(input12_file);
    remove(input16_file);

    // Free YUV planes
    for (int i = 0; i < 3; i++) {
        if (dst_planes[i]) {
            tjFree(dst_planes[i]);
        }
    }

    // Destroy handle
    tj3Destroy(handle);

    assert_file_closed(&in16);
	assert_file_closed(&in12);
	return 0;
}