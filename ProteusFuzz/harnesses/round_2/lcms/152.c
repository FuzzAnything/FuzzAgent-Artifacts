// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformStride at cmsxform.c:211:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 100) return NULL;
    return cmsOpenProfileFromMem(data, size);
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsHTRANSFORM hTransform = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsHPROFILE profiles[2];
    
    // Allocate buffers for input and output
    size_t buf_size = Size > 1024 ? 1024 : Size;
    uint8_t *in_buf = (uint8_t *)malloc(buf_size * 4);
    uint8_t *out_buf = (uint8_t *)malloc(buf_size * 4);
    
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    // Initialize buffers with fuzz data
    memset(in_buf, 0, buf_size * 4);
    memcpy(in_buf, Data, Size);
    memset(out_buf, 0, buf_size * 4);

    // Strategy 1: Create a single profile transform
    if (Size > 100) {
        hProfile = create_dummy_profile(Data, Size);
        if (hProfile) {
            // Use standard pixel format constants defined in lcms2.h
            // cmsMakeInputFormat and cmsMakeOutputFormat are not standard public APIs in all versions.
            // We use the standard constants directly: TYPE_RGB_8, TYPE_RGBA_8, etc.
            cmsUInt32Number input_fmt = TYPE_RGB_8;
            cmsUInt32Number output_fmt = TYPE_RGB_8;
            
            hTransform = cmsCreateTransform(hProfile, input_fmt,
                                            hProfile, output_fmt,
                                            INTENT_PERCEPTUAL, 0);
            
            if (hTransform) {
                // Test cmsGetTransformInputFormat
                cmsUInt32Number in_fmt = cmsGetTransformInputFormat(hTransform);
                
                // Test cmsGetTransformOutputFormat
                cmsUInt32Number out_fmt = cmsGetTransformOutputFormat(hTransform);

                // Calculate pixel count. Assume 3 bytes per pixel for RGB.
                cmsUInt32Number pixel_count = 0;
                if (in_fmt && out_fmt) {
                    // Heuristic: assume 3 bytes per pixel for TYPE_RGB_8
                    pixel_count = (buf_size * 4) / 3; 
                } else {
                    pixel_count = 1;
                }

                if (pixel_count > 0) {
                    // Test cmsDoTransform
                    cmsDoTransform(hTransform, in_buf, out_buf, pixel_count);
                    
                    // Test cmsDoTransformStride (Deprecated but target)
                    cmsDoTransformStride(hTransform, in_buf, out_buf, pixel_count, 3);
                    
                    // Test cmsDoTransformLineStride
                    cmsDoTransformLineStride(hTransform, in_buf, out_buf, 
                                            pixel_count, 1, 3, 3, 0, 0);
                }
                
                cleanup_transform(hTransform);
            }
            cleanup_profile(hProfile);
        }
    }

    // Strategy 2: Create a multiprofile transform
    if (Size > 200) {
        hProfile = create_dummy_profile(Data, Size / 2);
        hProfile2 = create_dummy_profile(Data + (Size / 2), Size / 2);
        
        if (hProfile && hProfile2) {
            profiles[0] = hProfile;
            profiles[1] = hProfile2;
            
            cmsUInt32Number input_fmt = TYPE_RGB_8;
            cmsUInt32Number output_fmt = TYPE_RGB_8;
            
            hTransform = cmsCreateMultiprofileTransform(profiles, 2,
                                                        input_fmt,
                                                        output_fmt,
                                                        INTENT_PERCEPTUAL, 0);
            
            if (hTransform) {
                cmsUInt32Number in_fmt = cmsGetTransformInputFormat(hTransform);
                cmsUInt32Number out_fmt = cmsGetTransformOutputFormat(hTransform);
                
                cmsUInt32Number pixel_count = 1;
                if (in_fmt && out_fmt) {
                     pixel_count = (buf_size * 4) / 3;
                }
                
                if (pixel_count > 0) {
                    cmsDoTransform(hTransform, in_buf, out_buf, pixel_count);
                    cmsDoTransformStride(hTransform, in_buf, out_buf, pixel_count, 3);
                    cmsDoTransformLineStride(hTransform, in_buf, out_buf, 
                                            pixel_count, 1, 3, 3, 0, 0);
                }
                cleanup_transform(hTransform);
            }
            cleanup_profile(hProfile);
            cleanup_profile(hProfile2);
        }
    }

    // Strategy 3: Test with NULL or invalid handles (Edge cases)
    cmsUInt32Number null_in = cmsGetTransformInputFormat(NULL);
    cmsUInt32Number null_out = cmsGetTransformOutputFormat(NULL);
    
    free(in_buf);
    free(out_buf);
    
    return 0;
}