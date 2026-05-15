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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 176, nr_unique_branch: 47
//<Quality> {"density":22,"unique_branches":{"tj3SaveImage8":[[511,7,511,16,0,0,4,1],[511,20,511,27,0,0,4,1],[511,31,511,40,0,0,4,1],[511,44,511,53,0,0,4,1],[511,57,511,67,0,0,4,1],[512,7,512,22,0,0,4,1],[512,26,512,49,0,0,4,1],[518,7,518,53,0,0,4,1],[526,7,526,45,0,0,4,1],[539,7,539,10,0,0,4,1],[546,14,546,17,0,0,4,1],[570,9,570,29,0,0,4,0],[570,33,570,67,0,0,4,0],[576,9,576,48,0,0,4,1],[587,7,587,17,0,0,4,1],[589,10,589,55,0,0,4,0],[589,10,589,55,0,0,4,1],[592,9,592,15,0,0,4,1],[607,7,607,11,0,0,4,0],[340,7,340,12,2,0,4,1]],"jinit_write_ppm":[[322,7,322,46,0,0,4,1],[322,50,322,75,0,0,4,1],[339,7,339,46,0,0,4,1],[340,7,340,47,0,0,4,1],[350,7,350,29,0,0,4,1],[351,8,351,45,0,0,4,1]],"finish_output_ppm":[[290,7,290,33,0,0,4,1]],"tjDecompressHeader3":[[1932,42,1932,61,0,0,4,0]],"calc_buffer_dimensions_ppm":[[304,7,304,46,0,0,4,1]],"tjSaveImage":[[3180,7,3180,52,0,0,4,1]],"start_output_ppm":[[245,3,245,21,0,0,4,1],[250,3,250,15,0,0,4,1],[251,3,251,19,0,0,4,0],[252,3,252,20,0,0,4,1],[253,3,253,19,0,0,4,1],[254,3,254,20,0,0,4,1],[255,3,255,20,0,0,4,1],[256,3,256,20,0,0,4,1],[257,3,257,20,0,0,4,1],[258,3,258,20,0,0,4,1],[259,3,259,20,0,0,4,1],[260,3,260,20,0,0,4,1],[261,3,261,16,0,0,4,1],[268,3,268,10,0,0,4,1],[78,4,78,17,2,0,4,1],[78,22,78,39,2,0,4,0],[78,43,78,61,2,0,4,0]]},"library_calls":["tjInitCompress","tjInitDecompress","tj3InitVersion","tjLoadImage","tjDecompressHeader2","tjDecompress2","tjSaveImage","tjCompress2","tjBufSizeYUV","tjDecompressToYUV","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjDecompressToYUVPlanes","tj3Compress16","tj3Decompress12","tj3Free","tjFree","tjSaveImage","tjCompress2","tjFree","tjFree","tj3GetErrorStr","tjDestroy","tjDestroy","tj3Destroy"],"critical_calls":["tjInitCompress","tjInitDecompress","tj3InitVersion","tjLoadImage","tjDecompressHeader2","tjDecompress2","tjSaveImage","tjCompress2","tjBufSizeYUV","tjDecompressToYUV","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjDecompressToYUVPlanes","tj3Compress16","tj3Decompress12","tj3Free","tjFree","tj3GetErrorStr","tjDestroy","tjDestroy","tj3Destroy"],"visited":0}
/**/


// Include libjpeg-turbo headers

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
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
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Too small for meaningful JPEG data
    
    // Write input data to a temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Initialize old API handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    
    // 2. Initialize new API handle
    tjhandle handle_v3 = tj3InitVersion(fuzz_int32_t_1, fuzz_int32_t_2);
    
    // 3. Load image from file (old API)
    int width, height, pixelFormat;
    unsigned char *loaded_image = tjLoadImage("input_file", &width, fuzz_int32_t_3, &height, &pixelFormat, fuzz_int32_t_4);
    
    // If loading failed, try to use the input data directly
    if (!loaded_image) {
        // Use the input data as JPEG buffer
        unsigned char *jpegBuf = (unsigned char *)data;
        unsigned long jpegSize = size;
        
        // Get image info
        if (tjDecompressHeader2(decompress_handle, jpegBuf, jpegSize, &width, &height, nullptr) == 0) {
            // Allocate buffer for decompressed image
            int pitch = width * tjPixelSize[pixelFormat];
            unsigned char *dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
            
            // Decompress using old API
            tjDecompress2(decompress_handle, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelFormat, fuzz_int32_t_5);
            
            // Save the decompressed image
            tjSaveImage("output_file", dstBuf, width, pitch, height, pixelFormat, fuzz_int32_t_6);
            
            // Recompress using old API
            unsigned char *recompressedBuf = nullptr;
            unsigned long recompressedSize = 0;
            tjCompress2(compress_handle, dstBuf, width, pitch, height, pixelFormat, 
                       &recompressedBuf, &recompressedSize, TJSAMP_420, fuzz_int32_t_7, fuzz_int32_t_8);
            
            // Decompress to YUV using old API
            unsigned long yuvBufSize = tjBufSizeYUV(width, height, TJSAMP_420);
            unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
            tjDecompressToYUV(decompress_handle, jpegBuf, jpegSize, yuvBuf, fuzz_int32_t_9);
            
            // Decompress to YUV planes using old API
            int strides[3];
            unsigned char *yuvPlanes[3];
            strides[0] = tjPlaneWidth(fuzz_int32_t_10, width, TJSAMP_420);
            strides[1] = tjPlaneWidth(fuzz_int32_t_11, width, TJSAMP_420);
            strides[2] = tjPlaneWidth(fuzz_int32_t_12, width, TJSAMP_420);
            
            for (int i = 0; i < 3; i++) {
                int planeHeight = tjPlaneHeight(fuzz_int32_t_13, height, TJSAMP_420);
                yuvPlanes[i] = (unsigned char *)malloc(strides[i] * planeHeight);
            }
            
            tjDecompressToYUVPlanes(decompress_handle, jpegBuf, jpegSize, yuvPlanes, 
                                   width, strides, height, fuzz_int32_t_14);
            
            // Use new API for 16-bit compression (even if we have 8-bit data)
            unsigned short *src16 = (unsigned short *)malloc(width * height * sizeof(unsigned short));
            for (size_t i = 0; i < width * height && i < size/2; i++) {
                src16[i] = ((unsigned short)data[i*2] << 8) | data[i*2+1];
            }
            
            unsigned char *jpegBuf16 = nullptr;
            size_t jpegSize16 = 0;
            if (handle_v3) {
                tj3Compress16(handle_v3, src16, width, width * sizeof(unsigned short), 
                             height, TJPF_GRAY, &jpegBuf16, &jpegSize16);
            }
            
            // Use new API for 12-bit decompression
            if (jpegBuf16 && jpegSize16 > 0) {
                short *dst12 = (short *)malloc(width * height * sizeof(short));
                if (handle_v3) {
                    tj3Decompress12(handle_v3, jpegBuf16, jpegSize16, dst12, 
                                   width * sizeof(short), TJPF_GRAY);
                }
                free(dst12);
            }
            
            // Cleanup
            free(src16);
            if (jpegBuf16) tj3Free(jpegBuf16);
            
            for (int i = 0; i < 3; i++) {
                free(yuvPlanes[i]);
            }
            free(yuvBuf);
            if (recompressedBuf) tjFree(recompressedBuf);
            free(dstBuf);
        }
    } else {
        // We have a loaded image
        int pitch = width * tjPixelSize[pixelFormat];
        
        // Save the loaded image
        tjSaveImage("output_file", loaded_image, width, pitch, height, pixelFormat, fuzz_int32_t_15);
        
        // Recompress using old API
        unsigned char *recompressedBuf = nullptr;
        unsigned long recompressedSize = 0;
        tjCompress2(compress_handle, loaded_image, width, pitch, height, pixelFormat, 
                   &recompressedBuf, &recompressedSize, TJSAMP_420, fuzz_int32_t_16, fuzz_int32_t_17);
        
        // Cleanup
        if (recompressedBuf) tjFree(recompressedBuf);
        tjFree(loaded_image);
    }
    
    // Get error string from new API
    if (handle_v3) {
        const char *error_str = tj3GetErrorStr(handle_v3);
        (void)error_str; // Use error string to avoid unused variable warning
    }
    
    // Cleanup
    if (compress_handle) tjDestroy(compress_handle);
    if (decompress_handle) tjDestroy(decompress_handle);
    if (handle_v3) tj3Destroy(handle_v3);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}