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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 45
//<Quality> {"density":25,"unique_branches":{"jpeg_calc_output_dimensions":[[338,3,338,21,0,0,4,0]],"tj3Compress16":[[87,7,87,35,0,0,4,1],[90,7,90,21,0,0,4,1],[90,25,90,35,0,0,4,1],[90,39,90,48,0,0,4,1],[90,52,90,63,0,0,4,1],[91,7,91,22,0,0,4,1],[91,26,91,49,0,0,4,1],[91,53,91,68,0,0,4,1],[92,7,92,23,0,0,4,1],[95,7,95,22,0,0,4,0],[95,26,95,45,0,0,4,0],[138,7,138,41,0,0,4,1],[140,7,140,41,0,0,4,1],[140,45,140,57,0,0,4,0],[143,7,143,25,0,0,4,1],[317,7,317,12,4,0,4,1]],"tj3SetCroppingRegion":[[2065,7,2065,37,0,0,4,0]],"j16init_write_ppm":[[324,7,324,46,0,0,4,1],[325,7,325,50,0,0,4,1],[339,7,339,46,0,0,4,1],[340,7,340,47,0,0,4,1],[366,9,366,32,0,0,4,0],[369,16,369,50,0,0,4,1],[78,4,78,17,11,0,4,1],[78,22,78,39,11,0,4,1]],"tj3SaveImage16":[[511,7,511,16,0,0,4,1],[511,20,511,27,0,0,4,1],[511,31,511,40,0,0,4,1],[511,44,511,53,0,0,4,1],[511,57,511,67,0,0,4,1],[512,7,512,22,0,0,4,1],[512,26,512,49,0,0,4,1],[518,7,518,53,0,0,4,1],[526,7,526,45,0,0,4,1],[539,7,539,10,0,0,4,1],[546,14,546,17,0,0,4,1],[572,9,572,47,0,0,4,1],[576,9,576,48,0,0,4,1],[587,7,587,17,0,0,4,1],[589,10,589,55,0,0,4,0],[589,10,589,55,0,0,4,1],[592,9,592,15,0,0,4,1],[607,7,607,11,0,0,4,0],[340,7,340,12,2,0,4,1]]},"library_calls":["tj3InitVersion","tj3DecompressHeader","tj3Destroy","tj3InitVersion","tj3Alloc","tj3Destroy","tj3Compress16","tj3Alloc","tj3Compress12","tj3Free","tj3GetScalingFactors","tj3SetCroppingRegion","tj3SaveImage16","tj3GetErrorCode","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Get","tj3Get","tj3Destroy","tj3SetCroppingRegion","tj3GetScalingFactors","tj3YUVBufSize","tj3Alloc","tj3Destroy","tj3DecompressToYUV8","tj3Alloc","tj3DecodeYUV8","tj3Alloc","tj3SaveImage16","tj3Compress16","tj3Free","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","tj3GetErrorCode","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3SetCroppingRegion","tj3GetScalingFactors","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tj3Alloc","tj3DecodeYUV8","tj3Alloc","tj3SaveImage16","tj3Compress16","tj3Free","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3Free","tj3Free","tj3GetErrorCode","tj3Free","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<104) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimal data for JPEG header
    
    tjhandle handle = nullptr;
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = 0;
    unsigned short *dstBuf16 = nullptr;
    unsigned char *yuvBuf = nullptr;
    unsigned char *rgbBuf8 = nullptr;
    short *srcBuf12 = nullptr;
    unsigned short *srcBuf16 = nullptr;
    char *errorStr = nullptr;
    int ret = 0;
    
    // Initialize decompressor handle
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    // Try to decompress header first to get image info
    if (tj3DecompressHeader(handle, data, size) != 0) {
        // Not a valid JPEG, try alternative approach
        tj3Destroy(handle);
        
        // Try to use the input as raw 16-bit image data for compression
        handle = tj3Init(TJINIT_COMPRESS);
        if (!handle) {
	return 0;
	}
	
        
        // Create synthetic 16-bit image data (16-bit grayscale)
        int width = 64;
        int height = 64;
        int pixelFormat = TJPF_GRAY;
        int pitch = width * tjPixelSize[pixelFormat];
        
        // Allocate 16-bit source buffer
        srcBuf16 = (unsigned short*)tj3Alloc(width * height * sizeof(unsigned short));
        if (!srcBuf16) {
            tj3Destroy(handle);
            return 0;
        }
        
        // Fill with pattern from input data
        for (int i = 0; i < width * height && i * 2 + 1 < (int)size; i++) {
            srcBuf16[i] = (data[i * 2 % size] << 8) | data[(i * 2 + 1) % size];
        }
        
        // Compress 16-bit image
        ret = tj3Compress16(handle, srcBuf16, fuzz_int32_t_1, pitch, fuzz_int32_t_2, pixelFormat, 
                           &jpegBuf, &jpegSize);
        
        // Also try 12-bit compression with scaled down values
        srcBuf12 = (short*)tj3Alloc(width * height * sizeof(short));
        if (srcBuf12) {
            for (int i = 0; i < width * height && i < (int)size; i++) {
                srcBuf12[i] = (short)((data[i] * 4095) / 255);  // Scale to 12-bit
            }
            unsigned char *jpegBuf12 = nullptr;
            size_t jpegSize12 = 0;
            tj3Compress12(handle, srcBuf12, fuzz_int32_t_3, pitch, fuzz_int32_t_4, pixelFormat,
                         &jpegBuf12, &jpegSize12);
            if (jpegBuf12) tj3Free(jpegBuf12);
        }
        
        // Get scaling factors (just to use the API)
        int numScalingFactors = 0;
        tjscalingfactor* scalingFactors = tj3GetScalingFactors(&numScalingFactors);
        
        // Set cropping region (middle quarter of image)
        tjregion cropRegion;
        cropRegion.x = width / 4;
        cropRegion.y = height / 4;
        cropRegion.w = width / 2;
        cropRegion.h = height / 2;
        tj3SetCroppingRegion(handle, cropRegion);
        
        // Save 16-bit image to file
        tj3SaveImage16(handle, fuzz_str_5, srcBuf16, fuzz_int32_t_6, pitch, fuzz_int32_t_7, pixelFormat);
        
        // Get error code
        int errorCode = tj3GetErrorCode(handle);
        
        // Clean up
        if (srcBuf16) tj3Free(srcBuf16);
        if (srcBuf12) tj3Free(srcBuf12);
        if (jpegBuf) tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Valid JPEG - get dimensions
    int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Set cropping region
    tjregion cropRegion;
    cropRegion.x = (width > 16) ? 8 : 0;
    cropRegion.y = (height > 16) ? 8 : 0;
    cropRegion.w = (width > 32) ? width - 16 : width;
    cropRegion.h = (height > 32) ? height - 16 : height;
    if (cropRegion.w <= 0) cropRegion.w = width;
    if (cropRegion.h <= 0) cropRegion.h = height;
    tj3SetCroppingRegion(handle, cropRegion);
    
    // Get scaling factors
    int numScalingFactors = 0;
    tjscalingfactor* scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    
    // Allocate YUV buffer
    int align = 1;
    size_t yuvSize = tj3YUVBufSize(fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10, TJSAMP_420);
    yuvBuf = (unsigned char*)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Decompress to YUV
    if (tj3DecompressToYUV8(handle, data, size, yuvBuf, fuzz_int32_t_11) == 0) {
        // Allocate RGB buffer for decoding
        int pixelFormat = TJPF_RGB;
        int pitch = width * tjPixelSize[pixelFormat];
        rgbBuf8 = (unsigned char*)tj3Alloc(height * pitch);
        
        if (rgbBuf8) {
            // Decode YUV back to RGB
            tj3DecodeYUV8(handle, yuvBuf, fuzz_int32_t_12, rgbBuf8, fuzz_int32_t_13, pitch, fuzz_int32_t_14, pixelFormat);
            
            // Allocate 16-bit destination buffer
            dstBuf16 = (unsigned short*)tj3Alloc(height * pitch * 2);  // 16-bit
            if (dstBuf16) {
                // Convert 8-bit to 16-bit by scaling
                for (int i = 0; i < height * pitch; i++) {
                    dstBuf16[i] = (unsigned short)(rgbBuf8[i] * 257);  // Scale to 16-bit
                }
                
                // Save 16-bit image
                tj3SaveImage16(handle, fuzz_str_15, dstBuf16, fuzz_int32_t_16, pitch, fuzz_int32_t_17, pixelFormat);
                
                // Compress 16-bit image
                unsigned char *compressedBuf = nullptr;
                size_t compressedSize = 0;
                tj3Compress16(handle, dstBuf16, fuzz_int32_t_18, pitch, fuzz_int32_t_19, pixelFormat,
                             &compressedBuf, &compressedSize);
                
                if (compressedBuf) {
                    tj3Free(compressedBuf);
                }
                
                // Also try 12-bit compression with converted data
                short *src12 = (short*)tj3Alloc(height * pitch * sizeof(short));
                if (src12) {
                    for (int i = 0; i < height * pitch; i++) {
                        src12[i] = (short)(dstBuf16[i] >> 4);  // Convert to 12-bit
                    }
                    unsigned char *compressed12Buf = nullptr;
                    size_t compressed12Size = 0;
                    tj3Compress12(handle, src12, fuzz_int32_t_20, pitch, fuzz_int32_t_21, pixelFormat,
                                 &compressed12Buf, &compressed12Size);
                    if (compressed12Buf) tj3Free(compressed12Buf);
                    tj3Free(src12);
                }
                
                tj3Free(dstBuf16);
            }
            tj3Free(rgbBuf8);
        }
    }
    
    // Get error code
    int errorCode = tj3GetErrorCode(handle);
    
    // Clean up
    if (yuvBuf) tj3Free(yuvBuf);
    tj3Destroy(handle);
    
    return 0;
}