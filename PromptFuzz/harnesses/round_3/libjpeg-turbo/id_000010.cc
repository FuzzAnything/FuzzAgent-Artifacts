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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 18.5, nr_unique_branch: 0
//<Quality> {"density":37,"unique_branches":{},"library_calls":["tj3InitVersion","tj3InitVersion","tj3Destroy","tj3Destroy","tj3Destroy","tj3Destroy","tj3Destroy","tj3Destroy","tjDecompressHeader3","tj3Destroy","tj3Destroy","tjBufSizeYUV2","tj3Alloc","tj3Alloc","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy","tj3Decompress8","tj3SaveImage8","tj3Decompress16","tj3SaveImage16","tj3DecompressToYUV8","tjCompressFromYUV","tjFree","tj3SetCroppingRegion","tj3Alloc","tj3Decompress8","tj3Free","tjCompress2","tjFree","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3InitVersion","tjDecompressHeader3","tjBufSizeYUV2","tj3Alloc","tj3Alloc","tj3Alloc","tj3Decompress8","tj3SaveImage8","tj3Decompress16","tj3SaveImage16","tj3DecompressToYUV8","tjCompressFromYUV","tjFree","tj3SetCroppingRegion","tj3Alloc","tj3Decompress8","tj3Free","tjCompress2","tjFree","tj3Free","tj3Free","tj3Free","tj3Destroy","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for JPEG
    
    // Initialize handles
    tjhandle decompressor = tj3Init(TJINIT_DECOMPRESS);
    tjhandle compressor = tj3Init(TJINIT_COMPRESS);
    if (!decompressor || !compressor) {
        if (decompressor) tj3Destroy(decompressor);
        if (compressor) tj3Destroy(compressor);
        return 0;
    }
    
    // Create temporary files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        tj3Destroy(decompressor);
        tj3Destroy(compressor);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input JPEG data
    unsigned char *jpegBuf = (unsigned char *)malloc(size);
    if (!jpegBuf) {
        assert_file_closed(&in_file);;
        tj3Destroy(decompressor);
        tj3Destroy(compressor);
        assert_file_closed(&in_file);
	return 0;
    }
    fread(jpegBuf, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Decompress header to get image info
    int width, height, jpegSubsamp, jpegColorspace;
    if (tjDecompressHeader3(decompressor, jpegBuf, size, &width, &height, 
                           &jpegSubsamp, &jpegColorspace) != 0) {
        free(jpegBuf);
        tj3Destroy(decompressor);
        tj3Destroy(compressor);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Calculate buffer sizes
    int pitch = width * tjPixelSize[TJPF_RGB];
    unsigned long yuvBufSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
    
    // Allocate buffers
    unsigned char *rgbBuf8 = (unsigned char *)tj3Alloc(pitch * height);
    unsigned short *rgbBuf16 = (unsigned short *)tj3Alloc(pitch * height * sizeof(unsigned short));
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = 0;
    
    if (!rgbBuf8 || !rgbBuf16 || !yuvBuf) {
        if (rgbBuf8) tj3Free(rgbBuf8);
        if (rgbBuf16) tj3Free(rgbBuf16);
        if (yuvBuf) tj3Free(yuvBuf);
        free(jpegBuf);
        tj3Destroy(decompressor);
        tj3Destroy(compressor);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Decompress to 8-bit RGB
    if (tj3Decompress8(decompressor, jpegBuf, size, rgbBuf8, pitch, TJPF_RGB) == 0) {
        // Save 8-bit image
        tj3SaveImage8(compressor, fuzz_str_1, rgbBuf8, width, pitch, height, TJPF_RGB);
    }
    
    // 2. Decompress to 16-bit RGB
    if (tj3Decompress16(decompressor, jpegBuf, size, rgbBuf16, pitch, TJPF_RGB) == 0) {
        // Save 16-bit image
        tj3SaveImage16(compressor, fuzz_str_2, rgbBuf16, width, pitch, height, TJPF_RGB);
    }
    
    // 3. Decompress to YUV
    if (tj3DecompressToYUV8(decompressor, jpegBuf, size, yuvBuf, fuzz_int32_t_3) == 0) {
        // Compress from YUV
        if (tjCompressFromYUV(compressor, yuvBuf, width, fuzz_int32_t_4, height, jpegSubsamp,
                             &jpegBuf2, &jpegSize2, fuzz_int32_t_5, fuzz_int32_t_6) == 0) {
            // Free the compressed buffer
            if (jpegBuf2) tjFree(jpegBuf2);
        }
    }
    
    // 4. Set cropping region
    tjregion cropRegion = {width/4, height/4, width/2, height/2};
    if (tj3SetCroppingRegion(decompressor, cropRegion) == 0) {
        // Decompress cropped region (8-bit)
        unsigned char *croppedBuf = (unsigned char *)tj3Alloc((width/2) * (height/2) * 3);
        if (croppedBuf) {
            tj3Decompress8(decompressor, jpegBuf, size, croppedBuf, (width/2)*3, TJPF_RGB);
            tj3Free(croppedBuf);
        }
    }
    
    // 5. Compress using tjCompress2
    unsigned char *jpegBuf3 = NULL;
    unsigned long jpegSize3 = 0;
    if (tjCompress2(compressor, rgbBuf8, width, pitch, height, TJPF_RGB,
                   &jpegBuf3, &jpegSize3, jpegSubsamp, fuzz_int32_t_7, fuzz_int32_t_8) == 0) {
        if (jpegBuf3) tjFree(jpegBuf3);
    }
    
    // Cleanup
    tj3Free(rgbBuf8);
    tj3Free(rgbBuf16);
    tj3Free(yuvBuf);
    free(jpegBuf);
    tj3Destroy(decompressor);
    tj3Destroy(compressor);
    
    // Remove temporary files
    remove("output8.bmp");
    remove("output16.bmp");
    
    assert_file_closed(&in_file);
	return 0;
}