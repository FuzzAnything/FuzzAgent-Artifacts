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
//<ID> 433
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 10
//<Quality> {"density":13,"unique_branches":{"tjDecompress2":[[2156,7,2156,25,0,0,4,1]],"tjDecompressToYUV2":[[2451,7,2451,37,0,0,4,1],[2454,7,2454,22,0,0,4,1],[2454,26,2454,39,0,0,4,1],[2454,43,2454,52,0,0,4,1],[2454,56,2454,66,0,0,4,1],[2482,7,2482,41,0,0,4,0],[2483,7,2483,25,0,0,4,0],[2483,7,2483,25,0,0,4,1],[329,7,329,12,1,0,4,1]]},"library_calls":["tjInitDecompress","tjDecompressHeader3","tj3GetScalingFactors","tjAlloc","tj3YUVBufSize","tj3JPEGBufSize","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjDecompressToYUV2","tjDecodeYUVPlanes","tj3EncodeYUV8","tj3DecodeYUV8","tj3SetCroppingRegion","tjCompressFromYUV","tjFree","tjDecompress2","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader3","tj3GetScalingFactors","tjAlloc","tj3YUVBufSize","tj3JPEGBufSize","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjDecompressToYUV2","tjDecodeYUVPlanes","tj3EncodeYUV8","tj3DecodeYUV8","tj3SetCroppingRegion","tjCompressFromYUV","tjFree","tjDecompress2","tjFree","tjDestroy"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libjpeg-turbo APIs in a coherent sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful JPEG data

    tjhandle handle = nullptr;
    unsigned char *jpeg_buf = nullptr;
    unsigned char *yuv_buf = nullptr;
    unsigned char *rgb_buf = nullptr;
    unsigned char *compressed_buf = nullptr;
    unsigned char *allocated_buf = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    int ret = 0;
    
    // 1. Initialize decompressor handle
    handle = tjInitDecompress();
    if (!handle) {
	return 0;
	}
	
    
    // 2. Decompress header to get image information
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    ret = tjDecompressHeader3(handle, data, size, &width, &height, 
                              &jpegSubsamp, &jpegColorspace);
    
    if (ret != 0 || width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        // Invalid JPEG header, but we'll continue with some operations
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }
    
    // 3. Get scaling factors
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    
    // 4. Allocate memory using tjAlloc
    allocated_buf = tjAlloc(1024); // Small allocation for demonstration
    
    // 5. Calculate buffer sizes
    size_t yuv_buf_size = tj3YUVBufSize(width, 4, height, jpegSubsamp);
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // 6. Calculate YUV plane dimensions
    int y_plane_width = tj3YUVPlaneWidth(0, width, jpegSubsamp);
    int y_plane_height = tj3YUVPlaneHeight(0, height, jpegSubsamp);
    
    // 7. Decompress JPEG to YUV
    if (yuv_buf_size > 0 && yuv_buf_size < 100 * 1024 * 1024) { // Limit size
        yuv_buf = (unsigned char *)malloc(yuv_buf_size);
        if (yuv_buf) {
            ret = tjDecompressToYUV2(handle, data, size, yuv_buf, 
                                     width, 4, height, 0);
            
            if (ret == 0) {
                // 8. Decode YUV to RGB (multiple methods)
                
                // Method 1: Using tj3DecodeYUV8
                if (width > 0 && height > 0) {
                    size_t rgb_size = width * height * 3;
                    rgb_buf = (unsigned char *)malloc(rgb_size);
                    if (rgb_buf) {
                        // Prepare planes for tjDecodeYUVPlanes
                        const unsigned char *src_planes[3];
                        int strides[3] = {0};
                        
                        // For simplicity, use planar YUV
                        src_planes[0] = yuv_buf; // Y plane
                        src_planes[1] = yuv_buf + width * height; // U plane
                        src_planes[2] = src_planes[1] + (width * height) / 4; // V plane
                        
                        strides[0] = width;
                        strides[1] = width / 2;
                        strides[2] = width / 2;
                        
                        // Method 2: Using tjDecodeYUVPlanes
                        ret = tjDecodeYUVPlanes(handle, src_planes, strides, 
                                                jpegSubsamp, rgb_buf, 
                                                width, width * 3, height, 
                                                TJPF_RGB, 0);
                        
                        if (ret == 0) {
                            // 9. Encode RGB back to YUV using tj3EncodeYUV8
                            unsigned char *yuv_buf2 = (unsigned char *)malloc(yuv_buf_size);
                            if (yuv_buf2) {
                                ret = tj3EncodeYUV8(handle, rgb_buf, width, 
                                                    width * 3, height, 
                                                    TJPF_RGB, yuv_buf2, 4);
                                
                                if (ret == 0) {
                                    // 10. Decode the new YUV using tj3DecodeYUV8
                                    unsigned char *rgb_buf2 = (unsigned char *)malloc(rgb_size);
                                    if (rgb_buf2) {
                                        ret = tj3DecodeYUV8(handle, yuv_buf2, 4, 
                                                            rgb_buf2, width, 
                                                            width * 3, height, 
                                                            TJPF_RGB);
                                        free(rgb_buf2);
                                    }
                                }
                                free(yuv_buf2);
                            }
                            
                            // 11. Set cropping region
                            tjregion crop_region;
                            crop_region.x = 0;
                            crop_region.y = 0;
                            crop_region.w = (width > 32) ? 32 : width;
                            crop_region.h = (height > 32) ? 32 : height;
                            tj3SetCroppingRegion(handle, crop_region);
                            
                            // 12. Compress from YUV
                            unsigned char *jpeg_out = nullptr;
                            unsigned long jpeg_out_size = 0;
                            ret = tjCompressFromYUV(handle, yuv_buf, width, 
                                                    4, height, jpegSubsamp, 
                                                    &jpeg_out, &jpeg_out_size, 
                                                    85, 0);
                            
                            if (ret == 0 && jpeg_out) {
                                tjFree(jpeg_out);
                            }
                        }
                        free(rgb_buf);
                    }
                }
            }
            free(yuv_buf);
        }
    }
    
    // 13. Try to decompress with scaling (using a different method)
    if (numScalingFactors > 0 && width > 0 && height > 0) {
        // Use the first scaling factor
        tjscalingfactor sf = scalingFactors[0];
        int scaled_width = (width * sf.num + sf.denom - 1) / sf.denom;
        int scaled_height = (height * sf.num + sf.denom - 1) / sf.denom;
        
        if (scaled_width > 0 && scaled_height > 0 && scaled_width < 4096 && scaled_height < 4096) {
            size_t scaled_rgb_size = scaled_width * scaled_height * 3;
            unsigned char *scaled_rgb = (unsigned char *)malloc(scaled_rgb_size);
            if (scaled_rgb) {
                // Decompress with scaling using flags
                ret = tjDecompress2(handle, data, size, scaled_rgb, 
                                    scaled_width, scaled_width * 3, 
                                    scaled_height, TJPF_RGB, 
                                    TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE);
                free(scaled_rgb);
            }
        }
    }
    
    // 14. Create output file for demonstration
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something minimal
        fwrite("test", 1, 4, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 15. Clean up
    if (allocated_buf) {
        tjFree(allocated_buf);
    }
    
    if (handle) {
        tjDestroy(handle);
    }
    
    assert_file_closed(&out_file);
	return 0;
}