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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"tj3LoadImage8":[[340,7,340,12,3,0,4,0]]},"library_calls":["tj3LoadImage8","tj3InitVersion","tjInitDecompress","tj3DecompressToYUVPlanes8","tjPlaneSizeYUV","tj3SetCroppingRegion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tjDecodeYUV","tjTransform","tj3Compress12","tj3Free","tjFree","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tj3LoadImage8","tj3InitVersion","tjInitDecompress","tj3DecompressToYUVPlanes8","tjPlaneSizeYUV","tj3SetCroppingRegion","tj3YUVBufSize","tj3Alloc","tj3DecompressToYUV8","tjDecodeYUV","tjTransform","tj3Compress12","tj3Free","tjFree","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":0}
/**/


// Include necessary libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful processing
    
    tjhandle handle = nullptr;
    tjhandle old_handle = nullptr;
    tjtransform transform;
    unsigned char* jpegBuf = nullptr;
    unsigned char* dstBuf = nullptr;
    unsigned char** dstPlanes = nullptr;
    unsigned char* yuvBuf = nullptr;
    short* src12Bit = nullptr;
    unsigned char* decodedBuf = nullptr;
    int* strides = nullptr;
    
    // 1. First, try to load an image (simulate file input)
    // Create a temporary file with the input data
    FILE* tmp_file = fmemopen((void*)data, size, "rb");
    if (!tmp_file) {
	assert_file_closed(&tmp_file);
	return 0;
	}
	
    
    int width, height, pixelFormat;
    unsigned char* loadedImage = tj3LoadImage8(nullptr, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat);
    
    // If loading from file fails, use the input data directly as JPEG
    if (!loadedImage) {
        // Use the input data as JPEG buffer
        jpegBuf = (unsigned char*)data;
        size_t jpegSize = size;
        
        // Initialize handles
        handle = tj3InitVersion(fuzz_int32_t_2, fuzz_int32_t_3);
        old_handle = tjInitDecompress();
        if (!handle || !old_handle) goto cleanup;
        
        // 2. Decompress to YUV planes
        int numPlanes = 3;
        dstPlanes = new unsigned char*[numPlanes];
        strides = new int[numPlanes];
        
        for (int i = 0; i < numPlanes; i++) {
            strides[i] = 0; // Let library calculate
            dstPlanes[i] = nullptr;
        }
        
        if (tj3DecompressToYUVPlanes8(handle, jpegBuf, jpegSize, dstPlanes, strides) == 0) {
            // 3. Calculate plane size
            int componentID = 0; // Y component
            unsigned long planeSize = tjPlaneSizeYUV(fuzz_int32_t_4, width, strides[0], height, TJSAMP_420);
            
            // 4. Set cropping region
            tjregion cropRegion;
            cropRegion.x = 0;
            cropRegion.y = 0;
            cropRegion.w = std::min(width / 2, 100);
            cropRegion.h = std::min(height / 2, 100);
            tj3SetCroppingRegion(handle, cropRegion);
            
            // 5. Decompress to YUV buffer
            size_t yuvBufSize = tj3YUVBufSize(width, fuzz_int32_t_5, height, TJSAMP_420);
            yuvBuf = (unsigned char*)tj3Alloc(yuvBufSize);
            if (yuvBuf) {
                tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf, fuzz_int32_t_6);
            }
            
            // 6. Decode YUV to RGB (using old API)
            if (yuvBuf) {
                decodedBuf = (unsigned char*)malloc(width * height * 3);
                if (decodedBuf) {
                    tjDecodeYUV(old_handle, yuvBuf, fuzz_int32_t_7, TJSAMP_420, 
                               decodedBuf, width, fuzz_int32_t_8, height, TJPF_RGB, fuzz_int32_t_9);
                }
            }
            
            // 7. Transform the image
            unsigned char* dstBufs[1];
            unsigned long dstSizes[1];
            tjtransform transforms[1];
            
            transforms[0].r = cropRegion;
            transforms[0].op = TJXOP_NONE;
            transforms[0].options = TJXOPT_CROP;
            transforms[0].data = nullptr;
            transforms[0].customFilter = nullptr;
            
            if (tjTransform(old_handle, jpegBuf, (unsigned long)jpegSize, 1, 
                           dstBufs, dstSizes, transforms, fuzz_int32_t_10) == 0) {
                // 8. Compress 12-bit data (simulate by converting 8-bit to 12-bit)
                if (decodedBuf) {
                    size_t srcSize = width * height * 3;
                    src12Bit = (short*)malloc(srcSize * sizeof(short));
                    if (src12Bit) {
                        // Convert 8-bit to 12-bit by shifting
                        for (size_t i = 0; i < srcSize; i++) {
                            src12Bit[i] = (short)(decodedBuf[i] << 4);
                        }
                        
                        unsigned char* compressedBuf = nullptr;
                        size_t compressedSize = 0;
                        
                        // Try to compress 12-bit data
                        tj3Compress12(handle, src12Bit, width, fuzz_int32_t_11, height, 
                                     TJPF_RGB, &compressedBuf, &compressedSize);
                        
                        if (compressedBuf) {
                            tj3Free(compressedBuf);
                        }
                    }
                }
                
                if (dstBufs[0]) {
                    tjFree(dstBufs[0]);
                }
            }
        }
    }
    
cleanup:
    // Clean up allocated resources
    if (loadedImage) {
        tj3Free(loadedImage);
    }
    
    if (dstPlanes) {
        for (int i = 0; i < 3; i++) {
            if (dstPlanes[i]) {
                tj3Free(dstPlanes[i]);
            }
        }
        delete[] dstPlanes;
    }
    
    if (strides) {
        delete[] strides;
    }
    
    if (yuvBuf) {
        tj3Free(yuvBuf);
    }
    
    if (decodedBuf) {
        free(decodedBuf);
    }
    
    if (src12Bit) {
        free(src12Bit);
    }
    
    if (handle) {
        tj3Destroy(handle);
    }
    
    if (old_handle) {
        tjDestroy(old_handle);
    }
    
    if (tmp_file) {
        assert_file_closed(&tmp_file);;
    }
    
    assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
}