// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    
    fwrite(data, 1, size, f);
    fclose(f);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    return profile;
}

int LLVMFuzzerTestOneInput_17(const uint8_t* Data, size_t Size) {
    if (Size < 16) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsHPROFILE profiles[4] = {NULL};
    size_t profile_count = Size % 4 + 1;
    
    for (size_t i = 0; i < profile_count && i < 4; i++) {
        size_t offset = (i * Size) / profile_count;
        size_t chunk_size = ((i + 1) * Size) / profile_count - offset;
        
        if (chunk_size > 0) {
            profiles[i] = create_dummy_profile(Data + offset, chunk_size);
        } else {
            profiles[i] = cmsCreate_sRGBProfile();
        }
    }
    
    cmsUInt32Number intent = Data[0] % 4;
    cmsUInt32Number proofing_intent = Data[1] % 4;
    cmsUInt32Number flags = Data[2];
    cmsUInt32Number input_format = Data[3];
    cmsUInt32Number output_format = Data[4];
    cmsUInt32Number n_bytes = Data[5] % 8 + 1;
    cmsBool is_float = Data[6] & 1;
    cmsFloat64Number version = (Data[7] % 100) / 10.0;
    
    if (profiles[0] && profiles[1] && profiles[2]) {
        cmsHTRANSFORM transform1 = cmsCreateProofingTransformTHR(
            ctx,
            profiles[0],
            input_format,
            profiles[1],
            output_format,
            profiles[2],
            intent,
            proofing_intent,
            flags
        );
        
        if (transform1) {
            cmsDeleteTransform(transform1);
        }
        
        cmsHTRANSFORM transform2 = cmsCreateProofingTransform(
            profiles[0],
            input_format,
            profiles[1],
            output_format,
            profiles[2],
            intent,
            proofing_intent,
            flags
        );
        
        if (transform2) {
            cmsHPROFILE devicelink = cmsTransform2DeviceLink(transform2, version, flags);
            if (devicelink) {
                cmsCloseProfile(devicelink);
            }
            cmsDeleteTransform(transform2);
        }
    }
    
    if (profiles[0]) {
        cmsFormatterForPCSOfProfile(profiles[0], n_bytes, is_float);
    }
    
    if (profile_count > 1) {
        cmsHTRANSFORM transform3 = cmsCreateMultiprofileTransformTHR(
            ctx,
            profiles,
            profile_count,
            input_format,
            output_format,
            intent,
            flags
        );
        
        if (transform3) {
            cmsDeleteTransform(transform3);
        }
    }
    
    for (size_t i = 0; i < 4; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}