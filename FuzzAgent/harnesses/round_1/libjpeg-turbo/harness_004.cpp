#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "turbojpeg.h"

namespace harness_004 {
/*
 * libjpeg-turbo fuzzing harness targeting TurboJPEG transform operations
 * This harness tests both legacy tjTransform and modern tj3Transform APIs
 * along with related helper functions (tjInitTransform, tjtransform struct,
 * TJXOP operations, TJXOPT flags, tjregion cropping).
 * 
 * Target APIs: tjTransform, tj3Transform, tjInitTransform
 * Transform operations: TJXOP_NONE, TJXOP_HFLIP, TJXOP_VFLIP, TJXOP_TRANSPOSE,
 *                       TJXOP_TRANSVERSE, TJXOP_ROT90, TJXOP_ROT180, TJXOP_ROT270
 * Transform options: TJXOPT_PERFECT, TJXOPT_TRIM, TJXOPT_CROP, TJXOPT_GRAY,
 *                    TJXOPT_NOOUTPUT, TJXOPT_PROGRESSIVE, TJXOPT_ARITHMETIC,
 *                    TJXOPT_OPTIMIZE, TJXOPT_COPYNONE
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 64) return 0;  // Minimum input size for meaningful transform testing
    
    FuzzedDataProvider fdp(data, size);
    
    /* Split input: first part as JPEG data, second part for parameters */
    size_t jpeg_data_size = fdp.ConsumeIntegralInRange<size_t>(16, size - 48);
    if (jpeg_data_size > size) return 0;
    
    std::vector<uint8_t> jpeg_data = fdp.ConsumeBytes<uint8_t>(jpeg_data_size);
    if (jpeg_data.empty()) return 0;
    
    /* Consume transform parameters */
    int op = fdp.ConsumeIntegralInRange<int>(TJXOP_NONE, TJXOP_ROT270);
    int options = 0;
    
    /* Set transform options based on fuzzer input */
    if (fdp.ConsumeBool()) options |= TJXOPT_PERFECT;
    if (fdp.ConsumeBool()) options |= TJXOPT_TRIM;
    if (fdp.ConsumeBool()) options |= TJXOPT_CROP;
    if (fdp.ConsumeBool()) options |= TJXOPT_GRAY;
    if (fdp.ConsumeBool()) options |= TJXOPT_NOOUTPUT;
    if (fdp.ConsumeBool()) options |= TJXOPT_PROGRESSIVE;
    if (fdp.ConsumeBool()) options |= TJXOPT_ARITHMETIC;
    if (fdp.ConsumeBool()) options |= TJXOPT_OPTIMIZE;
    if (fdp.ConsumeBool()) options |= TJXOPT_COPYNONE;
    
    /* Create transform structure */
    tjtransform xform;
    memset(&xform, 0, sizeof(tjtransform));
    xform.op = op;
    xform.options = options;
    
    /* Set cropping region if CROP option is enabled */
    if (options & TJXOPT_CROP) {
        xform.r.x = fdp.ConsumeIntegralInRange<int>(0, 1024);
        xform.r.y = fdp.ConsumeIntegralInRange<int>(0, 1024);
        xform.r.w = fdp.ConsumeIntegralInRange<int>(0, 1024);
        xform.r.h = fdp.ConsumeIntegralInRange<int>(0, 1024);
    }
    
    /* Test 1: Legacy tjTransform API with tjInitTransform */
    {
        tjhandle handle = tjInitTransform();
        if (handle) {
            unsigned char *dstBuf = NULL;
            unsigned long dstSize = 0;
            
            /* Try transform with legacy API */
            tjTransform(handle, jpeg_data.data(), jpeg_data.size(),
                        1, &dstBuf, &dstSize, &xform, 0);
            
            if (dstBuf) {
                tjFree(dstBuf);
            }
            
            tjDestroy(handle);
        }
    }
    
    /* Test 2: Modern tj3Transform API */
    {
        tjhandle handle = tj3Init(TJINIT_TRANSFORM);
        if (handle) {
            unsigned char *dstBuf = NULL;
            size_t dstSize = 0;
            
            /* Try to get buffer size first */
            dstSize = tj3TransformBufSize(handle, &xform);
            
            if (dstSize > 0 && dstSize < 1048576) {  // Limit to 1MB
                dstBuf = (unsigned char *)tj3Alloc(dstSize);
                if (dstBuf) {
                    /* Perform transform with modern API */
                    tj3Transform(handle, jpeg_data.data(), jpeg_data.size(),
                                 1, &dstBuf, &dstSize, &xform);
                    
                    tj3Free(dstBuf);
                }
            } else {
                /* Try without pre-allocated buffer */
                dstBuf = NULL;
                dstSize = 0;
                tj3Transform(handle, jpeg_data.data(), jpeg_data.size(),
                             1, &dstBuf, &dstSize, &xform);
                
                if (dstBuf) {
                    tj3Free(dstBuf);
                }
            }
            
            /* Test multiple transforms */
            if (fdp.remaining_bytes() > 32) {
                int num_transforms = fdp.ConsumeIntegralInRange<int>(1, 4);
                if (num_transforms > 0 && num_transforms <= 4) {
                    std::vector<tjtransform> transforms(num_transforms);
                    std::vector<unsigned char*> dstBufs(num_transforms, NULL);
                    std::vector<size_t> dstSizes(num_transforms, 0);
                    
                    for (int i = 0; i < num_transforms; i++) {
                        transforms[i].op = fdp.ConsumeIntegralInRange<int>(TJXOP_NONE, TJXOP_ROT270);
                        transforms[i].options = fdp.ConsumeIntegral<uint32_t>() & 0xFFFF;
                        
                        if (transforms[i].options & TJXOPT_CROP) {
                            transforms[i].r.x = fdp.ConsumeIntegralInRange<int>(0, 512);
                            transforms[i].r.y = fdp.ConsumeIntegralInRange<int>(0, 512);
                            transforms[i].r.w = fdp.ConsumeIntegralInRange<int>(0, 512);
                            transforms[i].r.h = fdp.ConsumeIntegralInRange<int>(0, 512);
                        }
                    }
                    
                    /* Try multiple transforms */
                    tj3Transform(handle, jpeg_data.data(), jpeg_data.size(),
                                 num_transforms, dstBufs.data(), dstSizes.data(),
                                 transforms.data());
                    
                    /* Clean up */
                    for (int i = 0; i < num_transforms; i++) {
                        if (dstBufs[i]) {
                            tj3Free(dstBufs[i]);
                        }
                    }
                }
            }
            
            tj3Destroy(handle);
        }
    }
    
    /* Test 3: Mixed API usage with different flags */
    if (fdp.remaining_bytes() > 16) {
        int flags = 0;
        if (fdp.ConsumeBool()) flags |= TJFLAG_BOTTOMUP;
        if (fdp.ConsumeBool()) flags |= TJFLAG_FASTDCT;
        if (fdp.ConsumeBool()) flags |= TJFLAG_ACCURATEDCT;
        if (fdp.ConsumeBool()) flags |= TJFLAG_PROGRESSIVE;
        if (fdp.ConsumeBool()) flags |= TJFLAG_NOREALLOC;
        if (fdp.ConsumeBool()) flags |= TJFLAG_STOPONWARNING;
        
        tjhandle handle = tjInitTransform();
        if (handle) {
            unsigned char *dstBuf = NULL;
            unsigned long dstSize = 0;
            
            /* Test with various flags */
            tjTransform(handle, jpeg_data.data(), jpeg_data.size(),
                        1, &dstBuf, &dstSize, &xform, flags);
            
            if (dstBuf) {
                tjFree(dstBuf);
            }
            
            tjDestroy(handle);
        }
    }
    
    return 0;
}

} // namespace harness_004
