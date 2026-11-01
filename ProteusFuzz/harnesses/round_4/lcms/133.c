// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext CreateContext(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    return ctx;
}

static void CleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE CreateProfileFromData(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    cmsHPROFILE h = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
    return h;
}

static cmsStageSignature GetRandomSignature(const uint8_t *Data, size_t Size, size_t *offset) {
    // Use raw values for signatures since the enum might be incomplete or specific constants missing
    // We read a value from the fuzz input to determine the signature, or use a default
    if (*offset + 4 > Size) return (cmsStageSignature)0x63686164; // 'chad'
    
    cmsUInt32Number rawVal = 0;
    memcpy(&rawVal, Data + *offset, 4);
    *offset += 4;
    
    // Map to some known or safe values if needed, but passing raw values tests the internal handling
    // Common signatures: 'LUT ', 'MAT ', 'CHAD', 'CURV', etc.
    // We just return the raw value cast to the enum type.
    return (cmsStageSignature)rawVal;
}

static cmsUInt32Number GetRandomUInt32(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) return 0;
    cmsUInt32Number val = 0;
    memcpy(&val, Data + *offset, 4);
    *offset += 4;
    return val;
}

static cmsFloat32Number GetRandomFloat(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) return 0.0f;
    cmsFloat32Number val = 0.0f;
    memcpy(&val, Data + *offset, 4);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContext(Data, Size);
    if (!ctx) return 0;

    size_t offset = 0;
    cmsUInt32Number num, size, nProfiles, inputFormat, outputFormat, intent, flags;
    cmsUInt32Number inputChan, outputChan;
    cmsStageSignature type;
    cmsHPROFILE hProfiles[256];
    cmsHPROFILE inputProfile, outputProfile, proofingProfile;
    cmsStage *stage = NULL;
    cmsHTRANSFORM transform = NULL;
    void *mem = NULL;

    // Helper lambdas are not supported in C, so we use direct calls or macros.
    // We will just call the functions directly.

    // 1. Fuzz _cmsCalloc
    num = GetRandomUInt32(Data, Size, &offset);
    size = GetRandomUInt32(Data, Size, &offset);
    // Prevent excessive allocation
    if (num > 10000 || size > 1000000 || (num > 0 && size > 0 && num * size > 10000000)) {
        num = 1; size = 1;
    }
    mem = _cmsCalloc(ctx, num, size);
    if (mem) {
        // Optional: write some data to trigger memory checks
        memset(mem, 0xAB, num * size);
        // Memory managed by context, freed on context delete
    }

    // 2. Fuzz _cmsDupMem
    if (Size >= 4) {
        cmsUInt32Number dupSize = GetRandomUInt32(Data, Size, &offset);
        if (dupSize > 1000000) dupSize = 100;
        // Allocate a temporary buffer to duplicate
        void *src = malloc(dupSize);
        if (src) {
            memset(src, 0xCD, dupSize);
            void *dup = _cmsDupMem(ctx, src, dupSize);
            if (dup) {
                // Memory managed by context
            }
            free(src);
        }
    }

    // 3. Fuzz cmsStageAllocCLutFloatGranular
    inputChan = GetRandomUInt32(Data, Size, &offset);
    outputChan = GetRandomUInt32(Data, Size, &offset);
    if (inputChan > 10 || inputChan == 0) inputChan = 3;
    if (outputChan > 10 || outputChan == 0) outputChan = 3;
    
    // Create clutPoints array
    cmsUInt32Number clutPoints[10] = {0};
    for (int i = 0; i < inputChan && i < 10; i++) {
        clutPoints[i] = GetRandomUInt32(Data, Size, &offset);
        if (clutPoints[i] < 2) clutPoints[i] = 2; // Min grid points
        if (clutPoints[i] > 100) clutPoints[i] = 100; // Max grid points
    }

    // Calculate total table size
    cmsUInt32Number totalPoints = 1;
    for (int i = 0; i < inputChan; i++) totalPoints *= clutPoints[i];
    totalPoints *= outputChan;

    cmsFloat32Number *table = NULL;
    if (totalPoints > 0 && totalPoints < 1000000) {
        table = (cmsFloat32Number*)malloc(totalPoints * sizeof(cmsFloat32Number));
        if (table) {
            for (cmsUInt32Number i = 0; i < totalPoints; i++) {
                table[i] = GetRandomFloat(Data, Size, &offset);
            }
            stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table);
            if (stage) {
                cmsStageFree(stage);
                stage = NULL;
            }
            free(table);
        }
    }

    // 4. Fuzz _cmsStageAllocPlaceholder
    type = GetRandomSignature(Data, Size, &offset);
    inputChan = GetRandomUInt32(Data, Size, &offset);
    outputChan = GetRandomUInt32(Data, Size, &offset);
    if (inputChan > 10) inputChan = 3;
    if (outputChan > 10) outputChan = 3;
    
    // Pass NULLs for function pointers to test error handling or internal defaults
    stage = _cmsStageAllocPlaceholder(ctx, type, inputChan, outputChan, NULL, NULL, NULL, NULL);
    if (stage) {
        cmsStageFree(stage);
        stage = NULL;
    }

    // 5. Fuzz cmsCreateMultiprofileTransformTHR
    nProfiles = GetRandomUInt32(Data, Size, &offset);
    if (nProfiles > 255) nProfiles = 1;
    if (nProfiles == 0) nProfiles = 1;

    memset(hProfiles, 0, sizeof(hProfiles));
    cmsHPROFILE profileFromData = CreateProfileFromData(Data, Size);
    
    if (profileFromData) {
        hProfiles[0] = profileFromData;
        if (nProfiles > 1) {
            if (Size > 256) {
                cmsHPROFILE p2 = CreateProfileFromData(Data + 128, Size - 128);
                if (p2) hProfiles[1] = p2;
            }
        }
    }

    inputFormat = GetRandomUInt32(Data, Size, &offset);
    outputFormat = GetRandomUInt32(Data, Size, &offset);
    intent = GetRandomUInt32(Data, Size, &offset);
    flags = GetRandomUInt32(Data, Size, &offset);

    transform = cmsCreateMultiprofileTransformTHR(ctx, hProfiles, nProfiles, inputFormat, outputFormat, intent, flags);
    if (transform) {
        cmsDeleteTransform(transform);
        transform = NULL;
    }

    // 6. Fuzz cmsCreateProofingTransformTHR
    inputProfile = CreateProfileFromData(Data, Size);
    outputProfile = CreateProfileFromData(Data, Size);
    proofingProfile = CreateProfileFromData(Data, Size);

    inputFormat = GetRandomUInt32(Data, Size, &offset);
    outputFormat = GetRandomUInt32(Data, Size, &offset);
    intent = GetRandomUInt32(Data, Size, &offset);
    cmsUInt32Number proofingIntent = GetRandomUInt32(Data, Size, &offset);
    flags = GetRandomUInt32(Data, Size, &offset);

    transform = cmsCreateProofingTransformTHR(ctx, inputProfile, inputFormat, outputProfile, outputFormat, proofingProfile, intent, proofingIntent, flags);
    if (transform) {
        cmsDeleteTransform(transform);
        transform = NULL;
    }

    // Cleanup profiles created from data
    if (profileFromData) cmsCloseProfile(profileFromData);
    if (inputProfile && inputProfile != profileFromData) cmsCloseProfile(inputProfile);
    if (outputProfile && outputProfile != profileFromData && outputProfile != inputProfile) cmsCloseProfile(outputProfile);
    if (proofingProfile && proofingProfile != profileFromData && proofingProfile != inputProfile && proofingProfile != outputProfile) cmsCloseProfile(proofingProfile);

    // Cleanup context
    CleanupContext(ctx);

    return 0;
}