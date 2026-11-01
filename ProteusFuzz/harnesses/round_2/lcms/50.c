// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_stage(cmsStage *stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    
    uint8_t *profile_data = (uint8_t *)malloc(size);
    if (!profile_data) return NULL;
    
    memcpy(profile_data, data, size);
    
    if (size >= 128) {
        profile_data[4] = 'a';
        profile_data[5] = 'c';
        profile_data[6] = 's';
        profile_data[7] = 'p';
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromMem(profile_data, (cmsUInt32Number)size);
    free(profile_data);
    return profile;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // Removed direct call to internal _cmsCalloc to prevent memory leak reports.
    // Internal allocation functions are tied to the context lifecycle and should not 
    // be called directly in user code without corresponding internal deallocation.
    
    // Test _cmsDupMem
    if (Size >= 4) {
        cmsUInt32Number dup_size = (Data[2] % 100) + 1;
        void *src = malloc(dup_size);
        if (src != NULL) {
            memset(src, 0xAA, dup_size);
            void *dup = _cmsDupMem(ctx, src, dup_size);
            // Memory managed by context, will be freed on cmsDeleteContext
            if (dup != NULL) {
                // No explicit free needed here
            }
            free(src);
        }
    }
    
    // Test cmsStageAllocIdentity
    if (Size >= 1) {
        cmsUInt32Number channels = (Data[0] % 10) + 1;
        cmsStage *stage = cmsStageAllocIdentity(ctx, channels);
        cleanup_stage(stage);
    }
    
    // Test cmsStageAllocCLutFloatGranular
    if (Size >= 6) {
        cmsUInt32Number input_chan = (Data[3] % 5) + 1;
        cmsUInt32Number output_chan = (Data[4] % 5) + 1;
        
        cmsUInt32Number *clutPoints = (cmsUInt32Number *)malloc(input_chan * sizeof(cmsUInt32Number));
        if (clutPoints != NULL) {
            for (int i = 0; i < input_chan; i++) {
                clutPoints[i] = (Data[5] % 10) + 2;
            }
            
            size_t table_size = input_chan * output_chan;
            for (int i = 0; i < input_chan; i++) {
                table_size *= clutPoints[i];
            }
            
            cmsFloat32Number *table = (cmsFloat32Number *)malloc(table_size * sizeof(cmsFloat32Number));
            if (table != NULL) {
                memset(table, 0, table_size * sizeof(cmsFloat32Number));
                cmsStage *stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, input_chan, output_chan, table);
                cleanup_stage(stage);
                free(table);
            }
            free(clutPoints);
        }
    }
    
    // Test cmsCreateProofingTransformTHR and cmsCreateMultiprofileTransformTHR
    if (Size >= 256) {
        cmsHPROFILE input_profile = create_dummy_profile(ctx, Data, Size);
        cmsHPROFILE output_profile = create_dummy_profile(ctx, Data + 128, Size - 128);
        cmsHPROFILE proof_profile = create_dummy_profile(ctx, Data + 64, Size - 64);
        
        if (input_profile && output_profile && proof_profile) {
            cmsUInt32Number input_format = (Data[255] % 4) + 1;
            cmsUInt32Number output_format = (Data[254] % 4) + 1;
            cmsUInt32Number intent = (Data[253] % 4);
            cmsUInt32Number proof_intent = (Data[252] % 4);
            cmsUInt32Number flags = (Data[251] % 2) ? cmsFLAGS_SOFTPROOFING : 0;
            
            cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(
                ctx, input_profile, input_format,
                output_profile, output_format,
                proof_profile, intent, proof_intent, flags
            );
            cleanup_transform(transform);
        }
        
        if (input_profile && output_profile) {
            cmsHPROFILE profiles[2] = {input_profile, output_profile};
            cmsUInt32Number n_profiles = 2;
            cmsUInt32Number input_fmt = (Data[250] % 4) + 1;
            cmsUInt32Number output_fmt = (Data[249] % 4) + 1;
            cmsUInt32Number intent = (Data[248] % 4);
            cmsUInt32Number flags = (Data[247] % 2) ? cmsFLAGS_BLACKPOINTCOMPENSATION : 0;
            
            cmsHTRANSFORM mtransform = cmsCreateMultiprofileTransformTHR(
                ctx, profiles, n_profiles,
                input_fmt, output_fmt, intent, flags
            );
            cleanup_transform(mtransform);
        }
        
        cleanup_profile(input_profile);
        cleanup_profile(output_profile);
        cleanup_profile(proof_profile);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}