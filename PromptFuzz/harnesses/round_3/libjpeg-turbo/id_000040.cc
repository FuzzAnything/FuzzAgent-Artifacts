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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 8
//<Quality> {"density":30,"unique_branches":{"tj3Set":[[750,3,750,23,0,0,4,0],[751,9,751,33,0,0,4,1],[755,3,755,23,0,0,4,0],[719,7,719,23,7,0,4,1],[719,44,719,60,7,0,4,1],[719,7,719,23,8,0,4,1],[719,28,719,40,8,0,4,0],[719,44,719,60,8,0,4,1]]},"library_calls":["tj3InitVersion","tjInitCompress","tj3Destroy","tjDestroy","tj3Set","tj3Set","tj3Set","tj3JPEGBufSize","tj3Destroy","tjDestroy","tj3YUVBufSize","tj3Destroy","tjDestroy","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3Destroy","tjDestroy","tj3CompressFromYUVPlanes8","tj3GetICCProfile","tjCompressFromYUVPlanes","tjFree","tj3Free","tjCompressFromYUV","tjFree","tj3Compress12","tj3Free","tj3SaveImage16","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tjInitCompress","tj3Set","tj3Set","tj3Set","tj3JPEGBufSize","tj3YUVBufSize","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3YUVPlaneSize","tj3CompressFromYUVPlanes8","tj3GetICCProfile","tjCompressFromYUVPlanes","tjFree","tj3Free","tjCompressFromYUV","tjFree","tj3Compress12","tj3Free","tj3SaveImage16","tj3Destroy","tjDestroy"],"visited":0}
/**/


// libjpeg-turbo headers would be included here
// For fuzzing, we need to include the appropriate headers
// Since we're using both tj3 and tj APIs, we need both headers

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<88) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_7, fuzz_int32_t_array_size_7, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_8, fuzz_int32_t_array_size_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
	
		const int * fuzzer_var_tj3CompressFromYUVPlanes8_18_3 = fuzz_int32_t_array_7;
		const int * fuzzer_var_tjCompressFromYUVPlanes_20_3 = fuzz_int32_t_array_8;//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // Initialize handles
    tjhandle handle_new = tj3Init(TJINIT_COMPRESS);
    tjhandle handle_old = tjInitCompress();
    if (!handle_new || !handle_old) {
        if (handle_new) tj3Destroy(handle_new);
        if (handle_old) tjDestroy(handle_old);
        return 0;
    }
    
    // Set some parameters using tj3Set
    tj3Set(handle_new, TJPARAM_QUALITY, fuzz_int32_t_1);
    tj3Set(handle_new, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle_new, TJPARAM_FASTDCT, fuzz_int32_t_2);
    
    // Test dimensions - use values derived from input data
    int width = 64 + (data[0] % 192);  // 64-255
    int height = 64 + (data[1] % 192); // 64-255
    int align = 4;
    int jpegSubsamp = TJSAMP_420;
    
    // Calculate buffer sizes
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    if (jpegBufSize == 0 || jpegBufSize > 100 * 1024 * 1024) { // Sanity check
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        return 0;
    }
    
    // Allocate buffers
    unsigned char* jpegBuf = nullptr;
    size_t jpegSize = 0;
    unsigned long jpegSizeOld = 0;
    
    // For YUV data - we'll use part of input as YUV source
    size_t yuvSize = tj3YUVBufSize(width, fuzz_int32_t_3, height, jpegSubsamp);
    if (yuvSize == 0 || yuvSize > size) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        return 0;
    }
    
    // Create YUV source buffer from input data
    std::vector<unsigned char> yuvSrc(yuvSize);
    size_t copySize = yuvSize < size ? yuvSize : size;
    memcpy(yuvSrc.data(), data, copySize);
    
    // For YUV planes - allocate 3 planes
    const unsigned char* srcPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    
    // Calculate plane sizes and strides
    strides[0] = width;  // Y plane stride
    strides[1] = width / 2;  // U plane stride (4:2:0)
    strides[2] = width / 2;  // V plane stride (4:2:0)
    
    // Allocate plane data from input
    size_t planeYSize = tj3YUVPlaneSize(fuzz_int32_t_4, width, strides[0], height, jpegSubsamp);
    size_t planeUSize = tj3YUVPlaneSize(fuzz_int32_t_5, width, strides[1], height, jpegSubsamp);
    size_t planeVSize = tj3YUVPlaneSize(fuzz_int32_t_6, width, strides[2], height, jpegSubsamp);
    
    if (planeYSize == 0 || planeUSize == 0 || planeVSize == 0 ||
        planeYSize + planeUSize + planeVSize > size) {
        tj3Destroy(handle_new);
        tjDestroy(handle_old);
        return 0;
    }
    
    std::vector<unsigned char> planeY(planeYSize);
    std::vector<unsigned char> planeU(planeUSize);
    std::vector<unsigned char> planeV(planeVSize);
    
    // Copy data to planes
    size_t offset = 0;
    if (offset < size) memcpy(planeY.data(), data + offset, 
                             planeYSize < size - offset ? planeYSize : size - offset);
    offset += planeYSize;
    if (offset < size) memcpy(planeU.data(), data + offset, 
                             planeUSize < size - offset ? planeUSize : size - offset);
    offset += planeUSize;
    if (offset < size) memcpy(planeV.data(), data + offset, 
                             planeVSize < size - offset ? planeVSize : size - offset);
    
    srcPlanes[0] = planeY.data();
    srcPlanes[1] = planeU.data();
    srcPlanes[2] = planeV.data();
    
    // 1. Test tj3CompressFromYUVPlanes8 (new API)
    int result = tj3CompressFromYUVPlanes8(handle_new, srcPlanes, width, fuzzer_var_tj3CompressFromYUVPlanes8_18_3, 
                                          height, &jpegBuf, &jpegSize);
    if (result == 0 && jpegBuf && jpegSize > 0) {
        // 2. Test tj3GetICCProfile (even if no ICC profile, API should handle it)
        unsigned char* iccBuf = nullptr;
        size_t iccSize = 0;
        tj3GetICCProfile(handle_new, &iccBuf, &iccSize);
        // Note: iccBuf is allocated by the library if ICC profile exists
        if (iccBuf) {
            free(iccBuf);  // Must free if allocated by library
        }
        
        // 3. Test tjCompressFromYUVPlanes (old API)
        unsigned char* jpegBufOld = nullptr;
        result = tjCompressFromYUVPlanes(handle_old, srcPlanes, width, fuzzer_var_tjCompressFromYUVPlanes_20_3,
                                        height, jpegSubsamp, &jpegBufOld, 
                                        &jpegSizeOld, fuzz_int32_t_9, fuzz_int32_t_10);
        if (jpegBufOld) {
            tjFree(jpegBufOld);
        }
    }
    
    // Free the jpeg buffer from first compression
    if (jpegBuf) {
        tj3Free(jpegBuf);
        jpegBuf = nullptr;
    }
    
    // 4. Test tjCompressFromYUV (old API)
    unsigned char* jpegBufOld2 = nullptr;
    unsigned long jpegSizeOld2 = 0;
    result = tjCompressFromYUV(handle_old, yuvSrc.data(), width, fuzz_int32_t_11, height,
                              jpegSubsamp, &jpegBufOld2, &jpegSizeOld2, fuzz_int32_t_12, fuzz_int32_t_13);
    if (jpegBufOld2) {
        tjFree(jpegBufOld2);
    }
    
    // 5. Test tj3Compress12 - need 12-bit source data
    // Create dummy 12-bit source (12-bit per sample, packed)
    int pixelFormat12 = TJPF_RGB;
    int pitch12 = width * 3 * sizeof(short); // 3 components, 12-bit each (stored in 16-bit)
    std::vector<short> src12Buf(width * height * 3);
    
    // Fill with data from input
    size_t src12Size = src12Buf.size() * sizeof(short);
    size_t copySize12 = src12Size < size ? src12Size : size;
    memcpy(src12Buf.data(), data, copySize12);
    
    result = tj3Compress12(handle_new, src12Buf.data(), width, pitch12, height,
                          pixelFormat12, &jpegBuf, &jpegSize);
    if (jpegBuf) {
        tj3Free(jpegBuf);
        jpegBuf = nullptr;
    }
    
    // 6. Test tj3SaveImage16 - need 16-bit source data
    // Create dummy 16-bit source
    int pixelFormat16 = TJPF_RGB;
    int pitch16 = width * 3 * sizeof(unsigned short);
    std::vector<unsigned short> src16Buf(width * height * 3);
    
    // Fill with data from input
    size_t src16Size = src16Buf.size() * sizeof(unsigned short);
    size_t copySize16 = src16Size < size ? src16Size : size;
    memcpy(src16Buf.data(), data, copySize16);
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Save 16-bit image
        result = tj3SaveImage16(handle_new, fuzz_str_14, src16Buf.data(), 
                               width, pitch16, height, pixelFormat16);
        assert_file_closed(&out_file);;
        // Remove the output file to avoid accumulating files
        remove("output_file");
    }
    
    // Cleanup
    tj3Destroy(handle_new);
    tjDestroy(handle_old);
    
    assert_file_closed(&out_file);
	return 0;
}