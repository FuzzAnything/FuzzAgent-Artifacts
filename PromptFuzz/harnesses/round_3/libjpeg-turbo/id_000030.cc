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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 9
//<Quality> {"density":21,"unique_branches":{"get_dac":[[410,9,410,18,0,0,4,0]],"my_emit_message":[[95,9,95,29,0,0,4,0]],"tj3Set":[[739,3,739,29,0,0,4,0],[725,7,725,16,2,0,4,1],[725,20,725,29,2,0,4,1]],"save_marker":[[844,3,844,10,0,0,4,1]],"tj3LoadImage12":[[459,7,459,45,0,0,4,0],[467,7,467,11,0,0,4,1],[468,7,468,17,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3Set","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress16","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3LoadImage12","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3EncodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3Set","tj3DecompressHeader","tj3Get","tj3Get","tj3Alloc","tj3Decompress16","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3LoadImage12","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3EncodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
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
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // Write input data to a temporary file for tj3LoadImage12
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize decompressor handle
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use tj3Set to configure decompressor
    tj3Set(handle, TJPARAM_STOPONWARNING, fuzz_int32_t_1);
    
    // First, try to decompress the input as JPEG using tj3Decompress16
    unsigned short *dstBuf16 = nullptr;
    int width = 0, height = 0, pixelFormat = TJPF_RGB;
    int decompressResult = tj3DecompressHeader(handle, data, size);
    
    if (decompressResult == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        int pitch = width * tjPixelSize[TJPF_RGB];
        dstBuf16 = (unsigned short*)tj3Alloc((size_t)pitch * height);
        
        if (dstBuf16) {
            tj3Decompress16(handle, data, size, dstBuf16, pitch, TJPF_RGB);
        }
    }
    
    // Try to transform the input JPEG
    unsigned char *dstBufTransform = nullptr;
    size_t dstSizeTransform = 0;
    if (decompressResult == 0 && width > 0 && height > 0) {
        // Calculate buffer size for transformation
        dstSizeTransform = tj3TransformBufSize(handle, nullptr);
        if (dstSizeTransform > 0) {
            dstBufTransform = (unsigned char*)tj3Alloc(dstSizeTransform);
            if (dstBufTransform) {
                tjtransform transform;
                memset(&transform, 0, sizeof(transform));
                transform.op = TJXOP_NONE;
                transform.options = TJXOPT_CROP;
                transform.r.x = 0;
                transform.r.y = 0;
                transform.r.w = (width > 100) ? 100 : width / 2;
                transform.r.h = (height > 100) ? 100 : height / 2;
                
                tj3Transform(handle, data, size, 1, &dstBufTransform, 
                            &dstSizeTransform, &transform);
            }
        }
    }
    
    // Try to load the input file as 12-bit image
    int width12 = 0, height12 = 0, pixelFormat12 = 0;
    short *image12 = tj3LoadImage12(handle, "input_file", &width12, fuzz_int32_t_2, &height12, &pixelFormat12);
    
    // If we have 12-bit image data, try to encode it to YUV planes
    if (image12 && width12 > 0 && height12 > 0) {
        // Convert 12-bit to 8-bit for encoding
        unsigned char *srcBuf8 = (unsigned char*)tj3Alloc((size_t)width12 * height12 * 3);
        if (srcBuf8) {
            // Simple downscale from 12-bit to 8-bit (just take higher 8 bits)
            for (size_t i = 0; i < (size_t)width12 * height12 * 3; i++) {
                srcBuf8[i] = (unsigned char)(image12[i] >> 4);
            }
            
            // Set up destination planes for YUV encoding
            const int subsamp = TJSAMP_420;
            int strides[3];
            unsigned char *dstPlanes[3];
            
            for (int i = 0; i < 3; i++) {
                strides[i] = tj3YUVPlaneWidth(fuzz_int32_t_3, fuzz_int32_t_4, subsamp);
                int planeHeight = tj3YUVPlaneHeight(fuzz_int32_t_5, fuzz_int32_t_6, subsamp);
                size_t planeSize = tj3YUVPlaneSize(fuzz_int32_t_7, fuzz_int32_t_8, strides[i], fuzz_int32_t_9, subsamp);
                dstPlanes[i] = (unsigned char*)tj3Alloc(planeSize);
            }
            
            // Encode to YUV planes
            tj3EncodeYUVPlanes8(handle, srcBuf8, fuzz_int32_t_10, 
                               width12 * 3, fuzz_int32_t_11, fuzz_int32_t_12, 
                               dstPlanes, strides);
            
            // Clean up YUV planes
            for (int i = 0; i < 3; i++) {
                if (dstPlanes[i]) tj3Free(dstPlanes[i]);
            }
            tj3Free(srcBuf8);
        }
    }
    
    // Clean up
    if (image12) tj3Free(image12);
    if (dstBuf16) tj3Free(dstBuf16);
    if (dstBufTransform) tj3Free(dstBufTransform);
    assert_file_closed(&in_file);;
    remove("input_file");
    remove("output_file");
    tj3Destroy(handle);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}