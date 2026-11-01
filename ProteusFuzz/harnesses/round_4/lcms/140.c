// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Dummy function pointers matching the expected signatures in lcms2_plugin.h
// _cmsStageEvalFn: void (*)(const float *, float *, const cmsStage *)
static void dummy_eval_fn(const cmsFloat32Number *In, cmsFloat32Number *Out, const cmsStage *Stage) {
    (void)In;
    (void)Out;
    (void)Stage;
}

// _cmsStageDupElemFn: void *(*)(cmsStage *)
static void* dummy_dup_fn(cmsStage *Stage) {
    (void)Stage;
    return NULL;
}

// _cmsStageFreeElemFn: void (*)(cmsStage *)
static void dummy_free_fn(cmsStage *Stage) {
    (void)Stage;
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsUInt32Number num = 0;
    cmsUInt32Number size = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsUInt32Number proofingIntent = 0;
    cmsUInt32Number inputChan = 0;
    cmsUInt32Number outputChan = 0;

    if (Size >= 8) {
        num = *(const cmsUInt32Number*)(Data);
        size = *(const cmsUInt32Number*)(Data + 4);
    } else if (Size >= 4) {
        num = *(const cmsUInt32Number*)(Data);
        size = 1;
    }

    // Clamp values to prevent excessive allocation
    if (num > 10000 || size > 10000) {
        num = 100;
        size = 100;
    }

    // Test _cmsCalloc
    void *mem = _cmsCalloc(ContextID, num, size);
    if (mem) {
        _cmsFree(ContextID, mem);
    }

    // Test cmsCreateMultiprofileTransformTHR
    if (Size >= 12) {
        cmsHPROFILE hProfiles[256];
        cmsUInt32Number nProfiles = *(const cmsUInt32Number*)(Data + 8);
        if (nProfiles > 255) nProfiles = 1;
        
        memset(hProfiles, 0, sizeof(hProfiles));
        
        intent = *(const cmsUInt32Number*)(Data + 12) % 4;
        if (Size >= 16) inputFormat = *(const cmsUInt32Number*)(Data + 16) % 100;
        if (Size >= 20) outputFormat = *(const cmsUInt32Number*)(Data + 20) % 100;
        if (Size >= 24) flags = *(const cmsUInt32Number*)(Data + 24) % 100;

        cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(ContextID, hProfiles, nProfiles, inputFormat, outputFormat, intent, flags);
        if (transform) {
            cmsDeleteTransform(transform);
        }
    }

    // Test cmsCreateProofingTransformTHR
    if (Size >= 32) {
        cmsHPROFILE Input = NULL;
        cmsHPROFILE Output = NULL;
        cmsHPROFILE Proofing = NULL;
        
        if (Size >= 16) inputFormat = *(const cmsUInt32Number*)(Data + 16) % 100;
        if (Size >= 20) outputFormat = *(const cmsUInt32Number*)(Data + 20) % 100;
        if (Size >= 24) intent = *(const cmsUInt32Number*)(Data + 24) % 4;
        if (Size >= 28) proofingIntent = *(const cmsUInt32Number*)(Data + 28) % 4;
        if (Size >= 32) flags = *(const cmsUInt32Number*)(Data + 32) % 100;

        cmsHTRANSFORM proofTransform = cmsCreateProofingTransformTHR(ContextID, Input, inputFormat, Output, outputFormat, Proofing, intent, proofingIntent, flags);
        if (proofTransform) {
            cmsDeleteTransform(proofTransform);
        }
    }

    // Test _cmsStageAllocPlaceholder
    if (Size >= 40) {
        cmsStageSignature type = (cmsStageSignature)(*(const cmsUInt32Number*)(Data + 36));
        inputChan = *(const cmsUInt32Number*)(Data + 40) % 10;
        outputChan = *(const cmsUInt32Number*)(Data + 44) % 10;

        cmsStage* stage = _cmsStageAllocPlaceholder(ContextID, type, inputChan, outputChan, dummy_eval_fn, dummy_dup_fn, dummy_free_fn, NULL);
        if (stage) {
            cmsStageFree(stage);
        }
    }

    // Test _cmsDupMem
    if (Size >= 48) {
        const void *org = Data;
        cmsUInt32Number dupSize = *(const cmsUInt32Number*)(Data + 48);
        if (dupSize > 10000) dupSize = 100;

        void *dupMem = _cmsDupMem(ContextID, org, dupSize);
        if (dupMem) {
            _cmsFree(ContextID, dupMem);
        }
    }

    // Test cmsStageAllocCLutFloatGranular
    if (Size >= 64) {
        cmsUInt32Number clutPoints[10];
        cmsUInt32Number i;
        for (i = 0; i < 10 && (Size >= 64 + (i * 4)); i++) {
            clutPoints[i] = *(const cmsUInt32Number*)(Data + 64 + (i * 4)) % 100;
        }
        if (i == 0) clutPoints[0] = 2;
        
        if (Size >= 64 + (i * 4)) inputChan = *(const cmsUInt32Number*)(Data + 64 + (i * 4)) % 10;
        if (Size >= 64 + (i * 4) + 4) outputChan = *(const cmsUInt32Number*)(Data + 64 + (i * 4) + 4) % 10;
        
        cmsFloat32Number *table = NULL;
        cmsUInt32Number tableSize = 1;
        for (i = 0; i < inputChan; i++) {
            if (i < 10) tableSize *= (clutPoints[i] > 0 ? clutPoints[i] : 1);
        }
        
        // Limit table size to prevent OOM
        if (tableSize > 1000) tableSize = 1000;
        
        table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
        if (table) {
            memset(table, 0, tableSize * sizeof(cmsFloat32Number));
        }

        cmsStage* clutStage = cmsStageAllocCLutFloatGranular(ContextID, clutPoints, inputChan, outputChan, table);
        if (clutStage) {
            cmsStageFree(clutStage);
        }
        if (table) {
            free(table);
        }
    }

    cmsDeleteContext(ContextID);
    return 0;
}