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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 8
//<Quality> {"density":18,"unique_branches":{"tj3SetScalingFactor":[[2041,7,2041,37,0,0,4,1],[2044,15,2044,24,0,0,4,0],[2045,9,2045,39,0,0,4,0],[2045,9,2045,39,0,0,4,1],[2045,43,2045,77,0,0,4,0],[2045,43,2045,77,0,0,4,1],[2048,7,2048,17,0,0,4,1],[340,7,340,12,1,0,4,1]]},"library_calls":["tj3InitVersion","tj3InitVersion","tj3Destroy","tj3DecompressHeader","tj3Get","tj3Get","tj3SetScalingFactor","tj3LoadImage12","tj3LoadImage16","tj3Alloc","tjCompress2","tj3Transform","tj3Free","tjGetErrorCode","tj3GetErrorStr","tj3Free","tj3Free","tjFree","tjFree","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3SetScalingFactor","tj3LoadImage12","tj3LoadImage16","tj3Alloc","tjCompress2","tj3Transform","tj3Free","tjGetErrorCode","tj3GetErrorStr","tj3Free","tj3Free","tjFree","tjFree","tj3Destroy","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    tjhandle handle = nullptr;
    tjhandle transform_handle = nullptr;
    FILE *temp_file = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *transformedBuf = nullptr;
    size_t transformedSize = 0;
    short *image12 = nullptr;
    unsigned short *image16 = nullptr;
    int width = 0, height = 0, pixelFormat = 0;
    const char *filename = "temp_input.jpg";
    
    // Step 1: Create a temporary file with input data
    temp_file = fopen(filename, "wb");
    if (!temp_file) {
	assert_file_closed(&temp_file);
	return 0;
	}
	
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    temp_file = nullptr;
    
    // Step 2: Initialize handles
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        remove(filename);
        assert_file_closed(&temp_file);
	return 0;
    }
    
    transform_handle = tj3Init(TJINIT_TRANSFORM);
    if (!transform_handle) {
        tj3Destroy(handle);
        remove(filename);
        assert_file_closed(&temp_file);
	return 0;
    }
    
    // Step 3: Try to decompress header from input data
    if (tj3DecompressHeader(handle, data, size) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        // Step 4: Set scaling factor
        tjscalingfactor scaling;
        scaling.num = 1;
        scaling.denom = 2;
        tj3SetScalingFactor(handle, scaling);
        
        // Step 5: Try to load as 12-bit image (may fail if not appropriate format)
        image12 = tj3LoadImage12(handle, filename, &width, 4, &height, &pixelFormat);
        
        // Step 6: Try to load as 16-bit image (may fail if not appropriate format)
        image16 = tj3LoadImage16(handle, filename, &width, 4, &height, &pixelFormat);
        
        // Step 7: If we successfully loaded an image, try to compress it
        if (image12 || image16) {
            int comp_width = width > 0 ? width : 64;
            int comp_height = height > 0 ? height : 64;
            int comp_pitch = comp_width * (pixelFormat == TJPF_GRAY ? 1 : 3);
            
            // Create a simple test image buffer if loaded images are null
            unsigned char *testBuf = nullptr;
            if (!image12 && !image16) {
                testBuf = (unsigned char *)tj3Alloc(comp_width * comp_height * 3);
                if (testBuf) {
                    // Fill with simple pattern
                    for (int i = 0; i < comp_width * comp_height * 3; i++) {
                        testBuf[i] = (i % 256);
                    }
                    
                    // Try compression
                    if (tjCompress2(handle, testBuf, comp_width, comp_pitch, 
                                   comp_height, TJPF_RGB, &jpegBuf, &jpegSize, 
                                   TJSAMP_420, 85, TJFLAG_FASTDCT) == 0) {
                        
                        // Step 8: Try to transform the compressed image
                        if (jpegBuf && jpegSize > 0) {
                            tjtransform transform;
                            memset(&transform, 0, sizeof(transform));
                            transform.r.x = 0;
                            transform.r.y = 0;
                            transform.r.w = comp_width / 2;
                            transform.r.h = comp_height / 2;
                            transform.op = TJXOP_NONE;
                            transform.options = 0;
                            
                            tj3Transform(transform_handle, jpegBuf, jpegSize, 1, 
                                        &transformedBuf, &transformedSize, &transform);
                        }
                    }
                    tj3Free(testBuf);
                }
            }
        }
    }
    
    // Step 9: Error handling - get error codes and strings
    int error_code = tjGetErrorCode(handle);
    if (error_code != 0) {
        const char *error_str = tj3GetErrorStr(handle);
        // Error string can be logged or ignored for fuzzing
    }
    
    // Step 10: Cleanup
    if (image12) tj3Free(image12);
    if (image16) tj3Free(image16);
    if (jpegBuf) tjFree(jpegBuf);
    if (transformedBuf) tjFree(transformedBuf);
    if (transform_handle) tj3Destroy(transform_handle);
    if (handle) tj3Destroy(handle);
    
    remove(filename);
    
    assert_file_closed(&temp_file);
	return 0;
}