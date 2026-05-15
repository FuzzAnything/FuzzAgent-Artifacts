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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 65, nr_unique_branch: 2
//<Quality> {"density":65,"unique_branches":{"read_markers":[[1010,5,1010,16,0,0,4,0],[1011,11,1011,46,0,0,4,1]]},"library_calls":["tj3InitVersion","tj3DecompressHeader","tj3Destroy","tj3Get","tj3Get","tj3Get","tj3Destroy","TJBUFSIZE","tj3Alloc","tj3Destroy","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3Free","tj3Free","tj3Destroy","tj3DecompressToYUVPlanes8","tj3Free","tj3Destroy","tjInitDecompress","tj3Free","tjDecompressHeader2","tjDestroy","tj3Free","tj3Free","TJBUFSIZE","tj3Alloc","tjDestroy","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjFree","tj3Free","tjDestroy","tjPlaneWidth","tjPlaneHeight","tjDecodeYUVPlanes","tjFree","tj3Free","tjDestroy","tjDestroy","tj3Destroy","tj3InitVersion","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy","tjInitCompress","tj3CompressFromYUVPlanes8","tj3InitVersion","tj3DecompressHeader","tj3Destroy","tj3Free","tj3YUVBufSize","tj3Alloc","tj3CompressFromYUV8","tj3Free","tj3Free","tjCompressFromYUVPlanes","tjFree","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3TransformBufSize","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"critical_calls":["tj3InitVersion","tj3DecompressHeader","tj3Get","tj3Get","tj3Get","TJBUFSIZE","tj3Alloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneSize","tj3Alloc","tj3DecompressToYUVPlanes8","tj3Free","tj3Destroy","tjInitDecompress","tjDecompressHeader2","tj3Free","TJBUFSIZE","tj3Alloc","tjPlaneWidth","tjPlaneHeight","tjPlaneSizeYUV","tjAlloc","tjPlaneWidth","tjPlaneHeight","tjDecodeYUVPlanes","tjDestroy","tj3InitVersion","tjInitCompress","tj3CompressFromYUVPlanes8","tj3InitVersion","tj3DecompressHeader","tj3Destroy","tj3Free","tj3YUVBufSize","tj3Alloc","tj3CompressFromYUV8","tj3Free","tj3Free","tjCompressFromYUVPlanes","tjFree","tj3Alloc","tj3Compress12","tj3Free","tj3Free","tj3TransformBufSize","tj3Alloc","tj3Free","tj3Free","tj3Free","tj3Destroy","tjDestroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    tjhandle handle_decompress = nullptr;
    tjhandle handle_compress = nullptr;
    tjhandle handle_compress2 = nullptr;
    unsigned char* jpegBuf = nullptr;
    unsigned char* dstBuf = nullptr;
    unsigned char* yuvBuf = nullptr;
    unsigned char* compressedBuf = nullptr;
    unsigned char* transformBuf = nullptr;
    unsigned char** yuvPlanes = nullptr;
    int* strides = nullptr;
    size_t jpegSize = 0;
    unsigned long jpegSizeOld = 0;
    int width = 0, height = 0, subsamp = 0;
    
    // Initialize decompressor
    handle_decompress = tj3Init(TJINIT_DECOMPRESS);
    if (!handle_decompress) {
	return 0;
	}
	
    
    // Decompress JPEG header to get dimensions
    if (tj3DecompressHeader(handle_decompress, data, size) != 0) {
        tj3Destroy(handle_decompress);
        return 0;
    }
    
    width = tj3Get(handle_decompress, TJPARAM_JPEGWIDTH);
    height = tj3Get(handle_decompress, TJPARAM_JPEGHEIGHT);
    subsamp = tj3Get(handle_decompress, TJPARAM_SUBSAMP);
    
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tj3Destroy(handle_decompress);
        return 0;
    }
    
    // Use TJBUFSIZE to calculate buffer size
    unsigned long bufSize = TJBUFSIZE(width, height);
    dstBuf = (unsigned char*)tj3Alloc(bufSize);
    if (!dstBuf) {
        tj3Destroy(handle_decompress);
        return 0;
    }
    
    // Decompress to YUV planes using new API
    int numPlanes = (subsamp == TJSAMP_GRAY) ? 1 : 3;
    yuvPlanes = new unsigned char*[numPlanes];
    strides = new int[numPlanes];
    
    for (int i = 0; i < numPlanes; i++) {
        strides[i] = tj3YUVPlaneWidth(i, width, subsamp);
        if (strides[i] <= 0) strides[i] = width;
        
        int planeHeight = tj3YUVPlaneHeight(i, height, subsamp);
        if (planeHeight <= 0) planeHeight = height;
        
        size_t planeSize = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
        yuvPlanes[i] = (unsigned char*)tj3Alloc(planeSize);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) tj3Free(yuvPlanes[j]);
            delete[] yuvPlanes;
            delete[] strides;
            tj3Free(dstBuf);
            tj3Destroy(handle_decompress);
            return 0;
        }
    }
    
    if (tj3DecompressToYUVPlanes8(handle_decompress, data, size, yuvPlanes, strides) != 0) {
        // Cleanup and try alternative path
        for (int i = 0; i < numPlanes; i++) tj3Free(yuvPlanes[i]);
        delete[] yuvPlanes;
        delete[] strides;
        tj3Destroy(handle_decompress);
        
        // Try with old API
        handle_decompress = tjInitDecompress();
        if (!handle_decompress) {
            tj3Free(dstBuf);
            return 0;
        }
        
        if (tjDecompressHeader2(handle_decompress, (unsigned char*)data, size, 
                                &width, &height, &subsamp) != 0) {
            tjDestroy(handle_decompress);
            tj3Free(dstBuf);
            return 0;
        }
        
        // Reallocate with new dimensions
        tj3Free(dstBuf);
        bufSize = TJBUFSIZE(width, height);
        dstBuf = (unsigned char*)tj3Alloc(bufSize);
        if (!dstBuf) {
            tjDestroy(handle_decompress);
            return 0;
        }
        
        // Decode YUV planes using old API
        numPlanes = (subsamp == TJSAMP_GRAY) ? 1 : 3;
        yuvPlanes = new unsigned char*[numPlanes];
        strides = new int[numPlanes];
        
        for (int i = 0; i < numPlanes; i++) {
            strides[i] = tjPlaneWidth(i, width, subsamp);
            if (strides[i] <= 0) strides[i] = width;
            
            int planeHeight = tjPlaneHeight(i, height, subsamp);
            if (planeHeight <= 0) planeHeight = height;
            
            unsigned long planeSize = tjPlaneSizeYUV(i, width, strides[i], height, subsamp);
            yuvPlanes[i] = (unsigned char*)tjAlloc(planeSize);
            if (!yuvPlanes[i]) {
                for (int j = 0; j < i; j++) tjFree(yuvPlanes[j]);
                delete[] yuvPlanes;
                delete[] strides;
                tj3Free(dstBuf);
                tjDestroy(handle_decompress);
                return 0;
            }
        }
        
        // Fill planes with some data for fuzzing
        for (int i = 0; i < numPlanes; i++) {
            int planeWidth = tjPlaneWidth(i, width, subsamp);
            int planeHeight = tjPlaneHeight(i, height, subsamp);
            if (planeWidth > 0 && planeHeight > 0) {
                size_t dataToCopy = std::min(size, (size_t)(planeWidth * planeHeight));
                memcpy(yuvPlanes[i], data, dataToCopy);
            }
        }
        
        // Decode YUV planes to RGB using old API
        if (tjDecodeYUVPlanes(handle_decompress, (const unsigned char**)yuvPlanes, 
                              strides, subsamp, dstBuf, width, 0, height, 
                              TJPF_RGB, 0) != 0) {
            for (int i = 0; i < numPlanes; i++) tjFree(yuvPlanes[i]);
            delete[] yuvPlanes;
            delete[] strides;
            tj3Free(dstBuf);
            tjDestroy(handle_decompress);
            return 0;
        }
        
        tjDestroy(handle_decompress);
    } else {
        tj3Destroy(handle_decompress);
    }
    
    // Initialize compressor for new API
    handle_compress = tj3Init(TJINIT_COMPRESS);
    if (!handle_compress) {
        cleanup:
        for (int i = 0; i < numPlanes; i++) {
            if (yuvPlanes) tj3Free(yuvPlanes[i]);
        }
        delete[] yuvPlanes;
        delete[] strides;
        if (jpegBuf) tj3Free(jpegBuf);
        if (dstBuf) tj3Free(dstBuf);
        if (yuvBuf) tj3Free(yuvBuf);
        if (compressedBuf) tj3Free(compressedBuf);
        if (transformBuf) tj3Free(transformBuf);
        if (handle_compress) tj3Destroy(handle_compress);
        if (handle_compress2) tjDestroy(handle_compress2);
        return 0;
    }
    
    // Initialize compressor for old API
    handle_compress2 = tjInitCompress();
    if (!handle_compress2) goto cleanup;
    
    // Compress from YUV planes using new API
    if (tj3CompressFromYUVPlanes8(handle_compress, (const unsigned char*const*)yuvPlanes, 
                                  width, strides, height, &jpegBuf, &jpegSize) == 0) {
        // Try to decompress the compressed buffer
        tjhandle verify_handle = tj3Init(TJINIT_DECOMPRESS);
        if (verify_handle) {
            tj3DecompressHeader(verify_handle, jpegBuf, jpegSize);
            tj3Destroy(verify_handle);
        }
        tj3Free(jpegBuf);
        jpegBuf = nullptr;
    }
    
    // Allocate YUV buffer and compress using new API
    size_t yuvBufSize = tj3YUVBufSize(width, 1, height, subsamp);
    yuvBuf = (unsigned char*)tj3Alloc(yuvBufSize);
    if (yuvBuf) {
        // Fill with some data
        size_t dataToCopy = std::min(size, yuvBufSize);
        memcpy(yuvBuf, data, dataToCopy);
        
        if (tj3CompressFromYUV8(handle_compress, yuvBuf, width, 1, height, 
                                &jpegBuf, &jpegSize) == 0) {
            tj3Free(jpegBuf);
            jpegBuf = nullptr;
        }
        tj3Free(yuvBuf);
        yuvBuf = nullptr;
    }
    
    // Compress from YUV planes using old API
    if (handle_compress2 && yuvPlanes) {
        jpegBuf = nullptr;
        jpegSizeOld = 0;
        
        if (tjCompressFromYUVPlanes(handle_compress2, (const unsigned char**)yuvPlanes, 
                                    width, strides, height, subsamp, &jpegBuf, 
                                    &jpegSizeOld, 75, 0) == 0) {
            tjFree(jpegBuf);
            jpegBuf = nullptr;
        }
    }
    
    // Try 12-bit compression with dummy data
    if (width > 0 && height > 0) {
        int pitch = width * 3;  // 3 components for 12-bit
        short* srcBuf12 = (short*)tj3Alloc(pitch * height * sizeof(short));
        if (srcBuf12) {
            // Fill with some data
            size_t dataToCopy = std::min(size, (size_t)(pitch * height * sizeof(short)));
            memcpy(srcBuf12, data, dataToCopy);
            
            if (tj3Compress12(handle_compress, srcBuf12, width, pitch, height, 
                              TJPF_RGB, &jpegBuf, &jpegSize) == 0) {
                tj3Free(jpegBuf);
                jpegBuf = nullptr;
            }
            tj3Free(srcBuf12);
        }
    }
    
    // Calculate transform buffer size
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = std::min(width, 100);
    transform.r.h = std::min(height, 100);
    transform.op = TJXOP_NONE;
    transform.options = 0;
    
    size_t transformSize = tj3TransformBufSize(handle_compress, &transform);
    if (transformSize > 0) {
        transformBuf = (unsigned char*)tj3Alloc(transformSize);
        if (transformBuf) {
            // Fill with some data
            size_t dataToCopy = std::min(size, transformSize);
            memcpy(transformBuf, data, dataToCopy);
            tj3Free(transformBuf);
        }
    }
    
    // Cleanup
    for (int i = 0; i < numPlanes; i++) {
        tj3Free(yuvPlanes[i]);
    }
    delete[] yuvPlanes;
    delete[] strides;
    tj3Free(dstBuf);
    tj3Destroy(handle_compress);
    if (handle_compress2) tjDestroy(handle_compress2);
    
    return 0;
}