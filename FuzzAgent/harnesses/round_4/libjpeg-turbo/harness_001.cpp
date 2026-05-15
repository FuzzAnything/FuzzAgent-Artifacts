#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

namespace harness_001 {
/*
 * Fuzzing harness for libjpeg-turbo turbojpeg transform operations.
 * Targets both legacy tjTransform API and newer tj3Transform API with full
 * lifecycle: Init -> Configure -> Transform -> Destroy.
 * This harness specifically targets the turbojpeg wrapper layer which
 * currently has 0% coverage, as identified by coverage analysis.
 */


extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for basic JPEG header and parameters
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: Test legacy tjTransform API (TurboJPEG 1.2+)
     **************************************************************************/
    {
        // Create transform handle using legacy API
        tjhandle transform_handle = tjInitTransform();
        if (transform_handle == NULL) {
            // If init fails, continue to test newer API
            goto test_tj3_api;
        }
        
        // Consume parameters from fuzzer input
        int num_transforms = fdp.ConsumeIntegralInRange<int>(1, 3);
        if (num_transforms > 3) num_transforms = 3; // Safety
        
        // Allocate memory for transforms and output buffers
        tjtransform *transforms = (tjtransform *)calloc(num_transforms, sizeof(tjtransform));
        unsigned char **dst_bufs = (unsigned char **)calloc(num_transforms, sizeof(unsigned char *));
        unsigned long *dst_sizes = (unsigned long *)calloc(num_transforms, sizeof(unsigned long));
        
        if (transforms == NULL || dst_bufs == NULL || dst_sizes == NULL) {
            free(transforms);
            free(dst_bufs);
            free(dst_sizes);
            tjDestroy(transform_handle);
            goto test_tj3_api;
        }
        // Initialize transforms with fuzzer data
        for (int i = 0; i < num_transforms; i++) {
            // Clear transform structure
            memset(&transforms[i], 0, sizeof(tjtransform));
            
            // Consume transform operation
            transforms[i].op = fdp.ConsumeIntegralInRange<int>(0, TJ_NUMXOP - 1);
            
            // Consume transform options (bitwise OR of TJXOPT_* flags)
            transforms[i].options = 0;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_PERFECT;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_TRIM;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_CROP;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_GRAY;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_NOOUTPUT;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_PROGRESSIVE;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_COPYNONE;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_ARITHMETIC;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_OPTIMIZE;
            
            // Consume cropping region if CROP option is set
            if (transforms[i].options & TJXOPT_CROP) {
                transforms[i].r.x = fdp.ConsumeIntegralInRange<int>(0, 1000);
                transforms[i].r.y = fdp.ConsumeIntegralInRange<int>(0, 1000);
                transforms[i].r.w = fdp.ConsumeIntegralInRange<int>(0, 1000);
                transforms[i].r.h = fdp.ConsumeIntegralInRange<int>(0, 1000);
            }
            
            // Calculate buffer size for this transform
            // First check if we have enough data for JPEG content
            size_t remaining = fdp.remaining_bytes();
            if (remaining < 50) {
                // Not enough data for meaningful JPEG
                break;
            }
            
            // Consume some JPEG-like data for buffer size calculation
            // Use tjBufSize with estimated dimensions
            int width = fdp.ConsumeIntegralInRange<int>(1, 1024);
            int height = fdp.ConsumeIntegralInRange<int>(1, 1024);
            int subsamp = fdp.ConsumeIntegralInRange<int>(0, TJ_NUMSAMP - 1);
            
            dst_sizes[i] = tjBufSize(width, height, subsamp);
            if (dst_sizes[i] > 0 && dst_sizes[i] < 10 * 1024 * 1024) { // Limit to 10MB
                dst_bufs[i] = (unsigned char *)tjAlloc(dst_sizes[i]);
            } else {
                dst_bufs[i] = NULL;
            }
        }
        // Try to perform transform if we have valid JPEG data
        // Check if we have at least 50 bytes remaining before calling ConsumeIntegralInRange
        size_t remaining_bytes = fdp.remaining_bytes();
        if (remaining_bytes >= 50) {
            size_t jpeg_data_size = fdp.ConsumeIntegralInRange<size_t>(50, remaining_bytes);
            std::vector<uint8_t> jpeg_data = fdp.ConsumeBytes<uint8_t>(jpeg_data_size);
            
            if (jpeg_data.size() >= 50) {
                // Call tjTransform - ignore return value for fuzzing purposes
                // We're interested in exploring the code paths, not correctness
                (void)tjTransform(transform_handle, 
                                 jpeg_data.data(), 
                                 jpeg_data.size(),
                                 num_transforms,
                                 dst_bufs,
                                 dst_sizes,
                                 transforms,
                                 0);  // No flags
            }
        }
        // If we don't have enough data, just cleanup and continue
        // Cleanup
        for (int i = 0; i < num_transforms; i++) {
            if (dst_bufs[i] != NULL) {
                tjFree(dst_bufs[i]);
            }
        }
        
        free(transforms);
        free(dst_bufs);
        free(dst_sizes);
        tjDestroy(transform_handle);
    }
    
test_tj3_api:
    /**************************************************************************
     * PART 2: Test newer tj3Transform API (TurboJPEG 3.0+)
     **************************************************************************/
    {
        // Check if we have enough remaining data
        if (fdp.remaining_bytes() < 100) {
            return 0;
        }
        
        // Create transform handle using newer API
        tjhandle tj3_handle = tj3Init(TJINIT_TRANSFORM);
        if (tj3_handle == NULL) {
            return 0;
        }
        
        // Configure transform parameters using tj3Set
        // Consume various parameters from fuzzer input
        if (fdp.ConsumeBool()) {
            tj3Set(tj3_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
        }
        if (fdp.ConsumeBool()) {
            tj3Set(tj3_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
        }
        if (fdp.ConsumeBool()) {
            tj3Set(tj3_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
        }
        if (fdp.ConsumeBool()) {
            tj3Set(tj3_handle, TJPARAM_NOREALLOC, fdp.ConsumeBool());
        }
        
        // Try to get some parameters back using tj3Get
        if (fdp.ConsumeBool()) {
            (void)tj3Get(tj3_handle, TJPARAM_ARITHMETIC);
        }
        if (fdp.ConsumeBool()) {
            (void)tj3Get(tj3_handle, TJPARAM_PROGRESSIVE);
        }
        
        // Prepare transform structures
        int num_transforms = fdp.ConsumeIntegralInRange<int>(1, 2);
        tjtransform transforms[2];
        unsigned char *dst_bufs[2] = {NULL, NULL};
        size_t dst_sizes[2] = {0, 0};
        
        // Initialize transforms
        for (int i = 0; i < num_transforms && i < 2; i++) {
            memset(&transforms[i], 0, sizeof(tjtransform));
            
            // Consume transform operation from TJXOP enum
            transforms[i].op = fdp.ConsumeIntegralInRange<int>(0, TJ_NUMXOP - 1);
            
            // Consume transform options
            transforms[i].options = 0;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_PERFECT;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_TRIM;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_CROP;
            if (fdp.ConsumeBool()) transforms[i].options |= TJXOPT_GRAY;
            
            // Set cropping region if needed
            if (transforms[i].options & TJXOPT_CROP) {
                transforms[i].r.x = fdp.ConsumeIntegralInRange<int>(0, 500);
                transforms[i].r.y = fdp.ConsumeIntegralInRange<int>(0, 500);
                transforms[i].r.w = fdp.ConsumeIntegralInRange<int>(0, 500);
                transforms[i].r.h = fdp.ConsumeIntegralInRange<int>(0, 500);
            }
            // Calculate buffer size using newer API
            dst_sizes[i] = tj3TransformBufSize(tj3_handle, &transforms[i]);
            if (dst_sizes[i] > 0 && dst_sizes[i] < 5 * 1024 * 1024) {
                dst_bufs[i] = (unsigned char *)tj3Alloc(dst_sizes[i]);
            }
        }
        
        // Consume remaining data as JPEG input
        // Check if we have at least 50 bytes remaining before calling ConsumeIntegralInRange
        size_t tj3_remaining_bytes = fdp.remaining_bytes();
        if (tj3_remaining_bytes >= 50) {
            size_t jpeg_size = fdp.ConsumeIntegralInRange<size_t>(50, tj3_remaining_bytes);
            std::vector<uint8_t> jpeg_data = fdp.ConsumeBytes<uint8_t>(jpeg_size);
            
            if (jpeg_data.size() >= 50) {
                // Try to get JPEG header info first (may fail for invalid data)
                // Call tj3DecompressHeader to parse the JPEG header
                (void)tj3DecompressHeader(tj3_handle, jpeg_data.data(), jpeg_data.size());
                
                // Retrieve parameters using tj3Get
                int width = tj3Get(tj3_handle, TJPARAM_JPEGWIDTH);
                int height = tj3Get(tj3_handle, TJPARAM_JPEGHEIGHT);
                int subsamp = tj3Get(tj3_handle, TJPARAM_SUBSAMP);
                int colorspace = tj3Get(tj3_handle, TJPARAM_COLORSPACE);
                
                // Call tj3Transform
                (void)tj3Transform(tj3_handle, jpeg_data.data(), jpeg_data.size(),
                                  num_transforms, dst_bufs, dst_sizes,
                                  transforms);
            }
        }
        // If we don't have enough data, just cleanup and continue
        // Cleanup
        for (int i = 0; i < 2; i++) {
            if (dst_bufs[i] != NULL) {
                tj3Free(dst_bufs[i]);
            }
        }
        
        tj3Destroy(tj3_handle);
    }
    
    return 0;
}

} // namespace harness_001
