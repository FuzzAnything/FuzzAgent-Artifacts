// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_icc_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    // Write a minimal dummy ICC profile header (128 bytes)
    unsigned char header[128] = {0};
    // Size (4 bytes)
    header[0] = 0x00; header[1] = 0x00; header[2] = 0x00; header[3] = 0x80;
    // CMM Type (4 bytes)
    header[4] = 'A'; header[5] = 'C'; header[6] = 'S'; header[7] = 'P';
    // Version (4 bytes)
    header[8] = 0x02; header[9] = 0x00; header[10] = 0x00; header[11] = 0x00;
    // Device Class (4 bytes)
    header[12] = 'm'; header[13] = 'n'; header[14] = 't'; header[15] = 'r';
    // Device Manufacturer (4 bytes)
    header[16] = 'A'; header[17] = 'C'; header[18] = 'S'; header[19] = 'P';
    // Device Model (4 bytes)
    header[20] = 0x00; header[21] = 0x00; header[22] = 0x00; header[23] = 0x00;
    // Device Attributes (8 bytes)
    header[24] = 0x00; header[25] = 0x00; header[26] = 0x00; header[27] = 0x00;
    header[28] = 0x00; header[29] = 0x00; header[30] = 0x00; header[31] = 0x00;
    // Rendering Intent (4 bytes)
    header[32] = 0x00; header[33] = 0x00; header[34] = 0x00; header[35] = 0x00;
    // Connection Space (4 bytes)
    header[36] = 'X'; header[37] = 'Y'; header[38] = 'Z'; header[39] = ' ';
    // Date (8 bytes)
    header[40] = 0x00; header[41] = 0x00; header[42] = 0x00; header[43] = 0x00;
    header[44] = 0x00; header[45] = 0x00; header[46] = 0x00; header[47] = 0x00;
    // Profile ID (16 bytes)
    // Device Mfg Desc (4 bytes)
    header[60] = 0x00; header[61] = 0x00; header[62] = 0x00; header[63] = 0x00;
    // Device Desc (4 bytes)
    header[64] = 0x00; header[65] = 0x00; header[66] = 0x00; header[67] = 0x00;
    // View Cond Desc (4 bytes)
    header[68] = 0x00; header[69] = 0x00; header[70] = 0x00; header[71] = 0x00;
    // Copyright (4 bytes)
    header[72] = 0x00; header[73] = 0x00; header[74] = 0x00; header[75] = 0x00;
    // Profile Desc (4 bytes)
    header[76] = 0x00; header[77] = 0x00; header[78] = 0x00; header[79] = 0x00;
    // White Point (20 bytes)
    // Red Tristimulus (20 bytes)
    // Green Tristimulus (20 bytes)
    // Blue Tristimulus (20 bytes)
    // Red Curve (4 bytes)
    header[100] = 0x00; header[101] = 0x00; header[102] = 0x00; header[103] = 0x00;
    // Green Curve (4 bytes)
    header[104] = 0x00; header[105] = 0x00; header[106] = 0x00; header[107] = 0x00;
    // Blue Curve (4 bytes)
    header[108] = 0x00; header[109] = 0x00; header[110] = 0x00; header[111] = 0x00;
    // Red Matrix (36 bytes)
    // Green Matrix (36 bytes)
    // Blue Matrix (36 bytes)
    // Backlight (20 bytes)
    // Profile Sequence Desc (4 bytes)
    header[124] = 0x00; header[125] = 0x00; header[126] = 0x00; header[127] = 0x00;

    fwrite(header, 1, 128, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Ensure dummy file exists
    write_dummy_icc_file("./dummy_file");

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fixed: Removed extra arguments. Intent=0, Flags=0.
    cmsHTRANSFORM xform = cmsCreateTransform(hProfile, cmsSigRgbData, hProfile, cmsSigRgbData, 0, 0);
    cmsCloseProfile(hProfile);

    cmsNAMEDCOLORLIST *list = NULL;
    cmsNAMEDCOLORLIST *dupList = NULL;

    // Test cmsGetNamedColorList
    list = cmsGetNamedColorList(xform);

    // Test cmsNamedColorIndex with various inputs
    if (list) {
        cmsInt32Number idx = cmsNamedColorIndex(list, (const char *)Data);
        (void)idx; // Suppress unused variable warning
    }

    // Test cmsAppendNamedColor
    if (list) {
        cmsUInt16Number pcs[3] = {0, 0, 0};
        cmsUInt16Number colorant[cmsMAXCHANNELS] = {0};
        cmsBool res = cmsAppendNamedColor(list, (const char *)Data, pcs, colorant);
        (void)res;
    }

    // Test cmsDupNamedColorList
    if (list) {
        dupList = cmsDupNamedColorList(list);
    }

    // Test cmsNamedColorInfo
    if (list) {
        char name[256] = {0};
        char prefix[33] = {0};
        char suffix[33] = {0};
        cmsUInt16Number pcsInfo[3] = {0};
        cmsUInt16Number colorantInfo[cmsMAXCHANNELS] = {0};
        cmsBool infoRes = cmsNamedColorInfo(list, 0, name, prefix, suffix, pcsInfo, colorantInfo);
        (void)infoRes;
    }
    if (dupList) {
        char name[256] = {0};
        char prefix[33] = {0};
        char suffix[33] = {0};
        cmsUInt16Number pcsInfo[3] = {0};
        cmsUInt16Number colorantInfo[cmsMAXCHANNELS] = {0};
        cmsBool infoRes = cmsNamedColorInfo(dupList, 0, name, prefix, suffix, pcsInfo, colorantInfo);
        (void)infoRes;
    }

    // Cleanup
    if (xform) cmsDeleteTransform(xform);
    if (list) cmsFreeNamedColorList(list);
    if (dupList) cmsFreeNamedColorList(dupList);
    cmsDeleteContext(ctx);

    return 0;
}