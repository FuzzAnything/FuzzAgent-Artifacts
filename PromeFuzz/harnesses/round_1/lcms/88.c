// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    return cmsCreate_sRGBProfileTHR(ctx);
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    switch (selector) {
        case 0: {
            if (Size - offset < 4) break;
            
            cmsUInt32Number nProfiles = 2;
            cmsUInt32Number Intents[2];
            cmsHPROFILE hProfiles[2];
            cmsBool BPC[2];
            cmsFloat64Number AdaptationStates[2];
            cmsUInt32Number dwFlags = 0;
            
            for (int i = 0; i < 2 && offset + 4 <= Size; i++) {
                Intents[i] = *(cmsUInt32Number*)(Data + offset) % 4;
                offset += 4;
            }
            
            for (int i = 0; i < 2 && offset + 1 <= Size; i++) {
                BPC[i] = (Data[offset] & 1);
                offset += 1;
            }
            
            for (int i = 0; i < 2 && offset + 8 <= Size; i++) {
                AdaptationStates[i] = *(cmsFloat64Number*)(Data + offset);
                offset += 8;
            }
            
            hProfiles[0] = create_dummy_profile(ctx);
            hProfiles[1] = create_dummy_profile(ctx);
            
            if (hProfiles[0] && hProfiles[1]) {
                cmsPipeline* pipeline = _cmsDefaultICCintents(ctx, nProfiles, Intents, 
                                                              hProfiles, BPC, AdaptationStates, dwFlags);
                if (pipeline) {
                    cmsPipelineFree(pipeline);
                }
            }
            
            if (hProfiles[0]) cmsCloseProfile(hProfiles[0]);
            if (hProfiles[1]) cmsCloseProfile(hProfiles[1]);
            break;
        }
        
        case 1: {
            if (Size - offset < 40) break;
            
            cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            cmsFloat64Number Bright = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsFloat64Number Contrast = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsFloat64Number Hue = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsFloat64Number Saturation = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsUInt32Number TempSrc = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            cmsUInt32Number TempDest = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            
            if (nLUTPoints < 2) nLUTPoints = 2;
            
            cmsHPROFILE profile = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, 
                                                               Hue, Saturation, TempSrc, TempDest);
            if (profile) {
                cmsCloseProfile(profile);
            }
            break;
        }
        
        case 2: {
            if (Size - offset < 8) break;
            
            cmsFloat64Number d = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            
            cmsFloat64Number prev = cmsSetAdaptationStateTHR(ctx, d);
            (void)prev;
            break;
        }
        
        case 3: {
            if (Size - offset < 40) break;
            
            cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            cmsFloat64Number Bright = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsFloat64Number Contrast = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsFloat64Number Hue = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsFloat64Number Saturation = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            cmsUInt32Number TempSrc = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            cmsUInt32Number TempDest = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            
            if (nLUTPoints < 2) nLUTPoints = 2;
            
            cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, 
                                                                  Contrast, Hue, Saturation, 
                                                                  TempSrc, TempDest);
            if (profile) {
                cmsCloseProfile(profile);
            }
            break;
        }
        
        case 4: {
            if (Size - offset < 8) break;
            
            cmsFloat64Number d = *(cmsFloat64Number*)(Data + offset);
            offset += 8;
            
            cmsFloat64Number prev = cmsSetAdaptationState(d);
            (void)prev;
            break;
        }
        
        case 5: {
            if (Size - offset < 20) break;
            
            cmsUInt32Number InputFormat = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            cmsUInt32Number OutputFormat = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            cmsUInt32Number Intent = *(cmsUInt32Number*)(Data + offset) % 4;
            offset += 4;
            cmsUInt32Number ProofingIntent = *(cmsUInt32Number*)(Data + offset) % 4;
            offset += 4;
            cmsUInt32Number dwFlags = *(cmsUInt32Number*)(Data + offset);
            offset += 4;
            
            cmsHPROFILE Input = create_dummy_profile(ctx);
            cmsHPROFILE Output = create_dummy_profile(ctx);
            cmsHPROFILE Proofing = create_dummy_profile(ctx);
            
            if (Input && Output && Proofing) {
                cmsHTRANSFORM transform = cmsCreateProofingTransform(Input, InputFormat,
                                                                    Output, OutputFormat,
                                                                    Proofing, Intent,
                                                                    ProofingIntent, dwFlags);
                if (transform) {
                    cmsDeleteTransform(transform);
                }
            }
            
            if (Input) cmsCloseProfile(Input);
            if (Output) cmsCloseProfile(Output);
            if (Proofing) cmsCloseProfile(Proofing);
            break;
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}