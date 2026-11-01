// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIECAM02Forward at cmscam02.c:435:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:461:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void fuzz_ciecam02_forward_reverse(cmsContext ctx, const cmsViewingConditions* vc, const cmsCIEXYZ* xyz_in, cmsJCh* jch_out, cmsCIEXYZ* xyz_out) {
    cmsHANDLE hModel = cmsCIECAM02Init(ctx, vc);
    if (hModel == NULL) {
        return;
    }

    // Forward: XYZ -> JCh
    cmsCIECAM02Forward(hModel, xyz_in, jch_out);

    // Reverse: JCh -> XYZ
    cmsCIECAM02Reverse(hModel, jch_out, xyz_out);

    cmsCIECAM02Done(hModel);
}

static void fuzz_adapt_to_illuminant(const cmsCIEXYZ* source_white, const cmsCIEXYZ* target_illuminant, const cmsCIEXYZ* value, cmsCIEXYZ* result) {
    cmsAdaptToIlluminant(result, source_white, target_illuminant, value);
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Parse input data into structures
    cmsViewingConditions vc;
    cmsCIEXYZ xyz_in, xyz_out, result_adapt;
    cmsJCh jch_out;
    cmsCIEXYZ source_white, target_illuminant, value_adapt;

    // Initialize structures with fuzzed data
    size_t offset = 0;
    
    if (Size >= offset + sizeof(vc)) {
        memcpy(&vc, Data + offset, sizeof(vc));
        offset += sizeof(vc);
    } else {
        memset(&vc, 0, sizeof(vc));
    }

    if (Size >= offset + sizeof(xyz_in)) {
        memcpy(&xyz_in, Data + offset, sizeof(xyz_in));
        offset += sizeof(xyz_in);
    } else {
        memset(&xyz_in, 0, sizeof(xyz_in));
    }

    if (Size >= offset + sizeof(source_white)) {
        memcpy(&source_white, Data + offset, sizeof(source_white));
        offset += sizeof(source_white);
    } else {
        memset(&source_white, 0, sizeof(source_white));
    }

    if (Size >= offset + sizeof(target_illuminant)) {
        memcpy(&target_illuminant, Data + offset, sizeof(target_illuminant));
        offset += sizeof(target_illuminant);
    } else {
        memset(&target_illuminant, 0, sizeof(target_illuminant));
    }

    if (Size >= offset + sizeof(value_adapt)) {
        memcpy(&value_adapt, Data + offset, sizeof(value_adapt));
        offset += sizeof(value_adapt);
    } else {
        memset(&value_adapt, 0, sizeof(value_adapt));
    }

    // Validate white point components in viewing conditions
    if (vc.whitePoint.X < 0) vc.whitePoint.X = 0.0;
    if (vc.whitePoint.Y < 0) vc.whitePoint.Y = 0.0;
    if (vc.whitePoint.Z < 0) vc.whitePoint.Z = 0.0;

    // Test cmsCIECAM02Init, cmsCIECAM02Forward, cmsCIECAM02Reverse, cmsCIECAM02Done
    fuzz_ciecam02_forward_reverse(ctx, &vc, &xyz_in, &jch_out, &xyz_out);

    // Test cmsAdaptToIlluminant
    fuzz_adapt_to_illuminant(&source_white, &target_illuminant, &value_adapt, &result_adapt);

    // Test cmsD50_XYZ
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    if (d50 != NULL) {
        (void)d50->X; // Use the value to avoid unused variable warning
    }

    cmsDeleteContext(ctx);
    return 0;
}