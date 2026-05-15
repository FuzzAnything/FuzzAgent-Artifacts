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
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 176, nr_unique_branch: 31
//<Quality> {"density":44,"unique_branches":{"start_input_png":[[513,3,513,33,0,0,4,0],[514,9,514,45,0,0,4,1],[515,9,515,41,0,0,4,1],[519,9,519,47,0,0,4,1],[532,9,532,59,0,0,4,1],[541,3,541,33,0,0,4,0],[543,9,543,45,0,0,4,1],[547,9,547,53,0,0,4,0],[548,9,548,48,0,0,4,0],[550,10,550,46,0,0,4,0],[563,9,563,59,0,0,4,1],[572,3,572,31,0,0,4,0],[579,9,579,35,0,0,4,0],[582,17,582,52,0,0,4,1],[590,10,590,46,0,0,4,0],[593,9,593,47,0,0,4,1],[621,7,621,21,0,0,4,0],[621,7,621,21,0,0,4,1],[628,27,628,54,0,0,4,1],[643,19,643,32,0,0,4,0],[643,19,643,32,0,0,4,1],[78,4,78,17,12,0,4,1],[78,22,78,39,12,0,4,0],[78,43,78,61,12,0,4,0],[35,7,35,19,19,0,4,0],[78,4,78,17,23,0,4,1],[78,22,78,39,23,0,4,0],[78,43,78,61,23,0,4,0],[35,7,35,19,29,0,4,0]],"calculate_subimages":[[620,33,620,51,0,0,4,0]],"spng_strerror":[[6069,9,6069,29,0,0,4,0]]},"library_calls":["tj3InitVersion","tjInitDecompress","tjInitCompress","tj3Destroy","tjDestroy","tjDestroy","tj3LoadImage8","tj3GetErrorCode","tjGetErrorCode","tj3Free","tjDecompressHeader3","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjPlaneSizeYUV","tjPlaneHeight","tjDecompressToYUVPlanes","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjFree","tjFree","tjDecompressHeader3","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjPlaneSizeYUV","tjPlaneHeight","tjDecompressToYUVPlanes","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjFree","tjFree","tj3Compress12","tj3Free","tj3Free","tj3GetErrorCode","tjGetErrorCode","tj3Destroy","tjDestroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitDecompress","tjInitCompress","tj3LoadImage8","tj3GetErrorCode","tjGetErrorCode","tjDecompressHeader3","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjAlloc","tjPlaneSizeYUV","tjPlaneHeight","tjDecompressToYUVPlanes","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tjFree","tjFree","tj3Compress12","tj3Free","tj3Free","tj3GetErrorCode","tjGetErrorCode","tj3Destroy","tjDestroy","tjDestroy"],"visited":0}
/**/


// Include libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful processing
    
    // Step 1: Write input data to a temporary file for tj3LoadImage8
    const char* input_filename = "input_file";
    FILE* in_file = fopen(input_filename, "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Step 2: Initialize handles
    tjhandle handle3 = tj3Init(TJINIT_DECOMPRESS);
    tjhandle handle_old = tjInitDecompress();
    tjhandle handle_compress = tjInitCompress();
    
    if (!handle3 || !handle_old || !handle_compress) {
        if (handle3) tj3Destroy(handle3);
        if (handle_old) tjDestroy(handle_old);
        if (handle_compress) tjDestroy(handle_compress);
        remove(input_filename);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 3: Use tj3LoadImage8 to load the image
    int width = 0, height = 0, pixelFormat = 0;
    unsigned char* img8 = tj3LoadImage8(handle3, input_filename, &width, fuzz_int32_t_1, &height, &pixelFormat);
    
    // Check error codes
    int error3 = tj3GetErrorCode(handle3);
    int error_old = tjGetErrorCode(handle_old);
    
    // If image loading failed, try with the first part of data as JPEG
    if (!img8 || width <= 0 || height <= 0) {
        // Clean up and try alternative path
        if (img8) tj3Free(img8);
        
        // Use input data directly as JPEG buffer
        if (size > 0) {
            // Step 4: Use tjDecompressToYUVPlanes
            int jpeg_width, jpeg_height, jpeg_subsamp;
            tjDecompressHeader3(handle_old, data, size, &jpeg_width, &jpeg_height, 
                               &jpeg_subsamp, nullptr);
            
            if (jpeg_width > 0 && jpeg_height > 0) {
                // Calculate strides for YUV planes
                int strides[3];
                strides[0] = tjPlaneWidth(fuzz_int32_t_2, jpeg_width, jpeg_subsamp);
                strides[1] = tjPlaneWidth(fuzz_int32_t_3, jpeg_width, jpeg_subsamp);
                strides[2] = tjPlaneWidth(fuzz_int32_t_4, jpeg_width, jpeg_subsamp);
                
                // Allocate YUV planes
                unsigned char* yuv_planes[3];
                for (int i = 0; i < 3; i++) {
                    yuv_planes[i] = (unsigned char*)tjAlloc(
                        tjPlaneSizeYUV(fuzz_int32_t_5, strides[i], strides[i], 
                                      tjPlaneHeight(fuzz_int32_t_6, jpeg_height, jpeg_subsamp), 
                                      jpeg_subsamp));
                }
                
                // Decompress to YUV planes
                tjDecompressToYUVPlanes(handle_old, data, size, yuv_planes, 
                                       jpeg_width, strides, jpeg_height, fuzz_int32_t_7);
                
                // Step 5: Use tjEncodeYUV2
                // Allocate destination buffer for YUV encoding
                size_t yuv_size = tjBufSizeYUV2(jpeg_width, 4, jpeg_height, jpeg_subsamp);
                unsigned char* yuv_buffer = (unsigned char*)tjAlloc(yuv_size);
                
                if (yuv_buffer) {
                    // Encode back to YUV format
                    tjEncodeYUV2(handle_compress, img8 ? img8 : (unsigned char*)data, 
                                jpeg_width, fuzz_int32_t_8, jpeg_height, TJPF_RGB, 
                                yuv_buffer, jpeg_subsamp, fuzz_int32_t_9);
                    
                    tjFree(yuv_buffer);
                }
                
                // Clean up YUV planes
                for (int i = 0; i < 3; i++) {
                    if (yuv_planes[i]) tjFree(yuv_planes[i]);
                }
            }
        }
    } else {
        // Image loaded successfully, continue with processing
        
        // Step 4: Use tjDecompressToYUVPlanes with original input data
        int jpeg_width, jpeg_height, jpeg_subsamp;
        tjDecompressHeader3(handle_old, data, size, &jpeg_width, &jpeg_height, 
                           &jpeg_subsamp, nullptr);
        
        if (jpeg_width > 0 && jpeg_height > 0) {
            int strides[3];
            strides[0] = tjPlaneWidth(fuzz_int32_t_10, jpeg_width, jpeg_subsamp);
            strides[1] = tjPlaneWidth(fuzz_int32_t_11, jpeg_width, jpeg_subsamp);
            strides[2] = tjPlaneWidth(fuzz_int32_t_12, jpeg_width, jpeg_subsamp);
            
            unsigned char* yuv_planes[3];
            for (int i = 0; i < 3; i++) {
                yuv_planes[i] = (unsigned char*)tjAlloc(
                    tjPlaneSizeYUV(fuzz_int32_t_13, strides[i], strides[i], 
                                  tjPlaneHeight(fuzz_int32_t_14, jpeg_height, jpeg_subsamp), 
                                  jpeg_subsamp));
            }
            
            tjDecompressToYUVPlanes(handle_old, data, size, yuv_planes, 
                                   jpeg_width, strides, jpeg_height, fuzz_int32_t_15);
            
            // Step 5: Use tjEncodeYUV2 with loaded image
            size_t yuv_size = tjBufSizeYUV2(width, 4, height, jpeg_subsamp);
            unsigned char* yuv_buffer = (unsigned char*)tjAlloc(yuv_size);
            
            if (yuv_buffer) {
                tjEncodeYUV2(handle_compress, img8, fuzz_int32_t_16, fuzz_int32_t_17, fuzz_int32_t_18, fuzz_int32_t_19, 
                            yuv_buffer, jpeg_subsamp, fuzz_int32_t_20);
                tjFree(yuv_buffer);
            }
            
            // Clean up YUV planes
            for (int i = 0; i < 3; i++) {
                if (yuv_planes[i]) tjFree(yuv_planes[i]);
            }
        }
        
        // Step 6: Use tj3Compress12 - convert 8-bit to 12-bit
        if (img8 && width > 0 && height > 0) {
            // Create 12-bit buffer by scaling 8-bit values
            size_t pixel_count = width * height;
            std::vector<short> img12_buffer(pixel_count * 3);  // Assume RGB
            
            for (size_t i = 0; i < pixel_count * 3 && i < size; i++) {
                // Scale 8-bit (0-255) to 12-bit (0-4095)
                img12_buffer[i] = static_cast<short>((img8[i] * 4095) / 255);
            }
            
            unsigned char* jpeg_buf = nullptr;
            size_t jpeg_size = 0;
            
            // Try to compress 12-bit image
            tj3Compress12(handle3, img12_buffer.data(), fuzz_int32_t_21, fuzz_int32_t_22, fuzz_int32_t_23, 
                         TJPF_RGB, &jpeg_buf, &jpeg_size);
            
            if (jpeg_buf) {
                tj3Free(jpeg_buf);
            }
        }
        
        // Free the loaded image
        tj3Free(img8);
    }
    
    // Step 7: Check error codes again
    error3 = tj3GetErrorCode(handle3);
    error_old = tjGetErrorCode(handle_old);
    
    // Cleanup
    tj3Destroy(handle3);
    tjDestroy(handle_old);
    tjDestroy(handle_compress);
    
    // Remove temporary file
    remove(input_filename);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}