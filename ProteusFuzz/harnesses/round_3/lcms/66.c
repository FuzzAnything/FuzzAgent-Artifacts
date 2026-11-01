// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profiles(cmsHPROFILE p1, cmsHPROFILE p2, cmsHPROFILE p3) {
    if (p1) cmsCloseProfile(p1);
    if (p2) cmsCloseProfile(p2);
    if (p3) cmsCloseProfile(p3);
}

static void cleanup_transforms(cmsHTRANSFORM t1, cmsHTRANSFORM t2, cmsHTRANSFORM t3) {
    if (t1) cmsDeleteTransform(t1);
    if (t2) cmsDeleteTransform(t2);
    if (t3) cmsDeleteTransform(t3);
}

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    
    // Write minimal ICC profile data to a file
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    
    // Write size and data
    uint32_t profile_size = (size < 65535) ? size : 65535;
    fwrite(&profile_size, sizeof(uint32_t), 1, f);
    fwrite(data, 1, profile_size, f);
    fclose(f);
    
    return cmsOpenProfileFromFile("./dummy_file", "r");
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles from input data
    cmsHPROFILE input_profile = create_dummy_profile(Data, Size);
    cmsHPROFILE output_profile = create_dummy_profile(Data, Size);
    cmsHPROFILE proofing_profile = create_dummy_profile(Data, Size);
    
    if (!input_profile || !output_profile) {
        cleanup_profiles(input_profile, output_profile, proofing_profile);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Extract parameters from input data
    uint32_t input_format = Data[0] | (Data[1] << 8);
    uint32_t output_format = Data[2] | (Data[3] << 8);
    uint32_t intent = Data[4] % 4;
    uint32_t proofing_intent = Data[5] % 4;
    uint32_t flags = Data[6] | (Data[7] << 8);
    
    // Test cmsCreateTransform
    cmsHTRANSFORM transform1 = cmsCreateTransform(
        input_profile, input_format,
        output_profile, output_format,
        intent, flags
    );
    
    // Test cmsCreateProofingTransform
    cmsHTRANSFORM transform2 = cmsCreateProofingTransform(
        input_profile, input_format,
        output_profile, output_format,
        proofing_profile, intent, proofing_intent, flags
    );
    
    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles[3] = {input_profile, output_profile, proofing_profile};
    cmsHTRANSFORM transform3 = cmsCreateMultiprofileTransform(
        profiles, 3,
        input_format, output_format,
        intent, flags
    );
    
    // Test internal flag retrieval functions if transforms are valid
    if (transform1) {
        _cmsGetTransformFlags((struct _cmstransform_struct*)transform1);
        _cmsGetTransformWorkerFlags((struct _cmstransform_struct*)transform1);
    }
    
    if (transform2) {
        _cmsGetTransformFlags((struct _cmstransform_struct*)transform2);
        _cmsGetTransformWorkerFlags((struct _cmstransform_struct*)transform2);
    }
    
    if (transform3) {
        _cmsGetTransformFlags((struct _cmstransform_struct*)transform3);
        _cmsGetTransformWorkerFlags((struct _cmstransform_struct*)transform3);
    }
    
    // Test cmsDoTransformLineStride with valid transforms
    if (transform1 || transform2 || transform3) {
        uint8_t input_buffer[256];
        uint8_t output_buffer[256];
        memset(input_buffer, 0, sizeof(input_buffer));
        memset(output_buffer, 0, sizeof(output_buffer));
        
        cmsUInt32Number pixels_per_line = Data[8] % 10 + 1;
        cmsUInt32Number line_count = Data[9] % 10 + 1;
        cmsUInt32Number bytes_per_line_in = pixels_per_line * 3;
        cmsUInt32Number bytes_per_line_out = pixels_per_line * 3;
        cmsUInt32Number bytes_per_plane_in = bytes_per_line_in;
        cmsUInt32Number bytes_per_plane_out = bytes_per_line_out;
        
        if (transform1) {
            cmsDoTransformLineStride(
                transform1,
                input_buffer, output_buffer,
                pixels_per_line, line_count,
                bytes_per_line_in, bytes_per_line_out,
                bytes_per_plane_in, bytes_per_plane_out
            );
        }
        
        if (transform2) {
            cmsDoTransformLineStride(
                transform2,
                input_buffer, output_buffer,
                pixels_per_line, line_count,
                bytes_per_line_in, bytes_per_line_out,
                bytes_per_plane_in, bytes_per_plane_out
            );
        }
        
        if (transform3) {
            cmsDoTransformLineStride(
                transform3,
                input_buffer, output_buffer,
                pixels_per_line, line_count,
                bytes_per_line_in, bytes_per_line_out,
                bytes_per_plane_in, bytes_per_plane_out
            );
        }
    }
    
    // Cleanup
    cleanup_transforms(transform1, transform2, transform3);
    cleanup_profiles(input_profile, output_profile, proofing_profile);
    cmsDeleteContext(ctx);
    
    return 0;
}