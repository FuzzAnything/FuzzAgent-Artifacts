// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_stage(cmsStage* stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    const char* dummy_file = "./dummy_profile.icc";
    FILE* f = fopen(dummy_file, "wb");
    if (!f) return NULL;
    
    unsigned char header[128] = {0};
    header[0] = 1; header[1] = 0; header[2] = 0; header[3] = 0; 
    fwrite(header, 1, 128, f);
    fclose(f);
    
    cmsHPROFILE h = cmsOpenProfileFromFile(dummy_file, "r");
    return h;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 25) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsUInt32Number nChannels = Data[0] % 10 + 1;
    cmsUInt32Number num = (Data[1] % 100) + 1;
    cmsUInt32Number size = (Data[2] % 100) + 1;
    cmsUInt32Number nProfiles = Data[3] % 255 + 1;
    cmsUInt32Number InputFormat = Data[4] % 10;
    cmsUInt32Number OutputFormat = Data[5] % 10;
    cmsUInt32Number Intent = Data[6] % 4;
    cmsUInt32Number dwFlags = Data[7] % 100;

    cmsStage* stage = cmsStageAllocIdentity(ContextID, nChannels);
    cleanup_stage(stage);

    void* alloc_mem = _cmsCalloc(ContextID, num, size);
    if (alloc_mem) {
        free(alloc_mem);
    }

    if (alloc_mem) {
        alloc_mem = _cmsCalloc(ContextID, num, size);
        if (alloc_mem) {
            void* dup_mem = _cmsDupMem(ContextID, alloc_mem, size);
            if (dup_mem) {
                free(dup_mem);
            }
            free(alloc_mem);
        }
    }

    cmsHPROFILE* hProfiles = NULL;
    if (nProfiles > 0) {
        hProfiles = (cmsHPROFILE*)malloc(nProfiles * sizeof(cmsHPROFILE));
        if (!hProfiles) {
            cleanup_context(ContextID);
            return 0;
        }
        for (int i = 0; i < nProfiles; i++) {
            hProfiles[i] = create_dummy_profile(ContextID);
            if (!hProfiles[i]) {
                for (int j = 0; j < i; j++) {
                    cmsCloseProfile(hProfiles[j]);
                }
                free(hProfiles);
                cleanup_context(ContextID);
                return 0;
            }
        }
    }

    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
        ContextID, hProfiles, nProfiles, InputFormat, OutputFormat, Intent, dwFlags
    );
    cleanup_transform(transform);

    if (hProfiles) {
        for (int i = 0; i < nProfiles; i++) {
            cmsCloseProfile(hProfiles[i]);
        }
        free(hProfiles);
    }

    cmsHPROFILE InputProfile = create_dummy_profile(ContextID);
    cmsHPROFILE OutputProfile = create_dummy_profile(ContextID);
    cmsHPROFILE ProofingProfile = create_dummy_profile(ContextID);

    if (InputProfile && OutputProfile && ProofingProfile) {
        cmsHTRANSFORM proofTransform = cmsCreateProofingTransformTHR(
            ContextID, InputProfile, InputFormat, OutputProfile, OutputFormat,
            ProofingProfile, Intent, Intent, dwFlags
        );
        cleanup_transform(proofTransform);
    }

    if (InputProfile) cmsCloseProfile(InputProfile);
    if (OutputProfile) cmsCloseProfile(OutputProfile);
    if (ProofingProfile) cmsCloseProfile(ProofingProfile);

    cmsUInt32Number clutPoints[10];
    for (int i = 0; i < 10; i++) {
        if (Size > 8 + i) {
            clutPoints[i] = (Data[8 + i] % 10) + 1;
        } else {
            clutPoints[i] = 1;
        }
    }
    
    cmsUInt32Number inputChan = 1;
    if (Size > 18) inputChan = Data[18] % 5 + 1;
    cmsUInt32Number outputChan = 1;
    if (Size > 19) outputChan = Data[19] % 5 + 1;
    
    cmsFloat32Number* table = NULL;
    size_t tableSize = 1;
    for (int i = 0; i < inputChan; i++) {
        if (clutPoints[i] > 100) clutPoints[i] = 100;
        tableSize *= clutPoints[i];
    }
    tableSize *= outputChan;
    
    if (tableSize > 100000) {
        tableSize = 100000;
    }
    
    table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
    if (table) {
        for (size_t i = 0; i < tableSize; i++) {
            size_t dataIdx = 20 + (i % (Size - 20));
            if (Size > dataIdx) {
                table[i] = (float)(Data[dataIdx]) / 255.0f;
            } else {
                table[i] = 0.5f;
            }
        }
        cmsStage* clutStage = cmsStageAllocCLutFloatGranular(
            ContextID, clutPoints, inputChan, outputChan, table
        );
        cleanup_stage(clutStage);
        free(table);
    }

    cleanup_context(ContextID);
    return 0;
}