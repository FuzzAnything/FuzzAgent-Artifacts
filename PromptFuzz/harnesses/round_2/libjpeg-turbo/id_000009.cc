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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 3
//<Quality> {"density":45,"unique_branches":{"tj3Set":[[781,3,781,23,0,0,4,0],[725,7,725,16,18,0,4,1],[725,20,725,29,18,0,4,1]]},"library_calls":["tj3InitVersion","tj3Set","tj3Set","tj3TransformBufSize","tj3Destroy","tj3Alloc","tj3Destroy","tj3Transform","tj3Free","tj3Destroy","tj3DecompressHeader","tj3Free","tj3Destroy","tj3Get","tj3Get","tj3Get","tj3Free","tj3Destroy","tjBufSizeYUV","tj3Free","tj3Destroy","tj3Alloc","tj3Free","tj3Destroy","tj3DecompressToYUV8","tj3Free","tj3Free","tj3Destroy","tj3Alloc","tj3Free","tj3Free","tj3Destroy","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Destroy","tjEncodeYUV3","tj3CompressFromYUV8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3Set","tj3Set","tj3TransformBufSize","tj3Alloc","tj3Transform","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","tjBufSizeYUV","tj3Alloc","tj3DecompressToYUV8","tj3Alloc","tj3Alloc","tjEncodeYUV3","tj3CompressFromYUV8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	 // Need at least minimal JPEG data

    tjhandle handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) {
	return 0;
	}
	

    // Set some parameters for transform operation
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
    tj3Set(handle, TJPARAM_FASTDCT, 1);

    // Step 1: Transform the input JPEG (e.g., rotate 90 degrees)
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    
    tjtransform transform;
    memset(&transform, 0, sizeof(tjtransform));
    transform.op = TJXOP_ROT90;
    transform.options = TJXOPT_CROP;
    
    // Allocate buffer for transformed JPEG
    dstSize = tj3TransformBufSize(handle, &transform);
    if (dstSize == 0) {
        tj3Destroy(handle);
        return 0;
    }
    
    dstBuf = (unsigned char *)tj3Alloc(dstSize);
    if (!dstBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Perform transformation
    int result = tj3Transform(handle, data, size, 1, &dstBuf, &dstSize, &transform);
    if (result != 0) {
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Step 2: Get JPEG header info from transformed image
    if (tj3DecompressHeader(handle, dstBuf, dstSize) != 0) {
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    
    if (width <= 0 || height <= 0) {
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Step 3: Decompress transformed JPEG to YUV
    // Calculate YUV buffer size using tjBufSizeYUV
    unsigned long yuvSize = tjBufSizeYUV(width, height, subsamp);
    if (yuvSize == 0) {
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Decompress to YUV format with alignment of 1
    result = tj3DecompressToYUV8(handle, dstBuf, dstSize, yuvBuf, 1);
    if (result != 0) {
        tj3Free(yuvBuf);
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Step 4: Re-encode YUV with different parameters using tjEncodeYUV3
    // First create RGB buffer from YUV (simulate this by using a small buffer)
    unsigned long rgbBufSize = width * height * 3;
    unsigned char *rgbBuf = (unsigned char *)tj3Alloc(rgbBufSize);
    if (!rgbBuf) {
        tj3Free(yuvBuf);
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Decode YUV to RGB (simplified - using minimal data)
    // In real scenario, we'd need proper YUV to RGB conversion
    // Here we just fill with pattern to avoid crashes
    memset(rgbBuf, 128, rgbBufSize);
    
    // Encode RGB to YUV using different alignment and subsampling
    unsigned char *yuvBuf2 = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf2) {
        tj3Free(rgbBuf);
        tj3Free(yuvBuf);
        tj3Free(dstBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Use tjEncodeYUV3 with different parameters
    result = tjEncodeYUV3(handle, rgbBuf, width, width*3, height, TJPF_RGB,
                         yuvBuf2, 4, TJSAMP_420, 0);
    // Note: We ignore result as parameters might not be optimal
    
    // Step 5: Compress the YUV data back to JPEG
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    // Use the original YUV buffer for compression
    result = tj3CompressFromYUV8(handle, yuvBuf, width, 1, height, &jpegBuf, &jpegSize);
    if (result != 0) {
        // Clean up but continue
    }

    // Cleanup
    if (jpegBuf) tj3Free(jpegBuf);
    if (yuvBuf2) tj3Free(yuvBuf2);
    if (rgbBuf) tj3Free(rgbBuf);
    if (yuvBuf) tj3Free(yuvBuf);
    if (dstBuf) tj3Free(dstBuf);
    
    tj3Destroy(handle);
    
    return 0;
}