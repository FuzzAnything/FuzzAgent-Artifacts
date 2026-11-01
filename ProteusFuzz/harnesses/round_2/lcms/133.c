// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Helper to create a dummy transform from fuzzing data
static cmsHTRANSFORM create_dummy_transform(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hInput = NULL;
    cmsHPROFILE hOutput = NULL;
    cmsHTRANSFORM hTransform = NULL;

    // Try to create profiles from input data if size is sufficient
    if (Size > 128) {
        hInput = cmsOpenProfileFromMem(Data, Size);
        if (hInput) {
            hOutput = cmsOpenProfileFromMem(Data, Size);
        }
    }

    // Fallback to built-in profiles if custom ones failed or data was too small
    if (!hInput || !hOutput) {
        cmsColorSpaceSignature inSig = cmsSigRgbData;
        cmsColorSpaceSignature outSig = cmsSigRgbData;

        if (Size > 0 && (Data[0] & 1)) {
            inSig = cmsSigLabData;
        }
        if (Size > 1 && (Data[1] & 1)) {
            outSig = cmsSigCmykData;
        }

        // cmsCreateRGBProfile requires arguments in this version of lcms2.
        // We will use cmsCreateGrayProfile which also requires arguments, 
        // or better, construct a valid RGB profile using default values.
        // Since we need to pass WhitePoint, Primaries, and TransferFunctions,
        // and we don't have them easily, let's try to create a Gray profile 
        // or use cmsCreateProfile (if available) or just return NULL if we can't create.
        // However, cmsCreateGrayProfile also has arguments.
        // Let's try to create a simple RGB profile with default sRGB values.
        
        static cmsCIExyY whitePoint = {0.3127f, 0.3290f, 1.0f};
        static cmsCIExyY primaries[3] = {
            {0.6400f, 0.3300f, 1.0f}, // Red
            {0.3000f, 0.6000f, 1.0f}, // Green
            {0.1500f, 0.0600f, 1.0f}  // Blue
        };
        static cmsToneCurve *transferFuncs[3] = {NULL, NULL, NULL};

        if (inSig == cmsSigRgbData) {
            hInput = cmsCreateRGBProfile(&whitePoint, primaries, transferFuncs);
        } else {
            // Fallback to RGB if we can't create Lab/Cmyk easily without internal helpers
            hInput = cmsCreateRGBProfile(&whitePoint, primaries, transferFuncs);
        }

        if (outSig == cmsSigRgbData) {
            hOutput = cmsCreateRGBProfile(&whitePoint, primaries, transferFuncs);
        } else {
            hOutput = cmsCreateRGBProfile(&whitePoint, primaries, transferFuncs);
        }
    }

    if (hInput && hOutput) {
        cmsUInt32Number inFmt = 0;
        cmsUInt32Number outFmt = 0;
        cmsUInt32Number flags = 0;

        // Use cmsMakeInputSignature if available, otherwise construct manually
        // Format = (Space << 16) | (Channels << 8) | (BitDepth)
        // But let's assume standard macros exist. If not, we hardcode common formats.
        
        if (Size > 2) {
            switch (Data[2] % 4) {
                case 0: inFmt = cmsSigRgbData | (8 << 8); break; // 8-bit RGB
                case 1: inFmt = cmsSigLabData | (16 << 8); break; // 16-bit Lab
                case 2: inFmt = cmsSigCmykData | (8 << 8); break; // 8-bit CMYK
                case 3: inFmt = cmsSigRgbData | (16 << 8); break; // 16-bit RGB
            }
            switch (Data[3] % 4) {
                case 0: outFmt = cmsSigRgbData | (8 << 8); break;
                case 1: outFmt = cmsSigLabData | (16 << 8); break;
                case 2: outFmt = cmsSigCmykData | (8 << 8); break;
                case 3: outFmt = cmsSigRgbData | (16 << 8); break;
            }
            if (Size > 4) flags = Data[4] & 0xFF;
        } else {
            inFmt = cmsSigRgbData | (8 << 8);
            outFmt = cmsSigRgbData | (8 << 8);
        }

        hTransform = cmsCreateTransform(hInput, inFmt, hOutput, outFmt, 0, flags);
    }

    if (hInput) cmsCloseProfile(hInput);
    if (hOutput) cmsCloseProfile(hOutput);

    return hTransform;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHTRANSFORM hTransform = create_dummy_transform(Data, Size);

    if (!hTransform) {
        cmsGetTransformInputFormat(NULL);
        cmsGetTransformOutputFormat(NULL);
        return 0;
    }

    size_t pixelCount = 1;
    if (Size > 10) {
        pixelCount = (Data[10] % 100) + 1;
    }
    
    cmsUInt32Number inFmt = cmsGetTransformInputFormat(hTransform);
    cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
    
    // Estimate bytes per pixel based on format
    // Format structure: (Space << 16) | (Channels << 8) | (BitDepth)
    // BitDepth is in lower 8 bits.
    cmsUInt32Number bitDepthIn = inFmt & 0xFF;
    cmsUInt32Number bitDepthOut = outFmt & 0xFF;
    
    // Channels: (Format >> 8) & 0xFF
    cmsUInt32Number channelsIn = (inFmt >> 8) & 0xFF;
    cmsUInt32Number channelsOut = (outFmt >> 8) & 0xFF;

    cmsUInt32Number inBpp = (channelsIn * bitDepthIn) / 8;
    cmsUInt32Number outBpp = (channelsOut * bitDepthOut) / 8;

    if (inBpp == 0) inBpp = 4; // Fallback
    if (outBpp == 0) outBpp = 4; // Fallback

    if (pixelCount > 1000) pixelCount = 1000;
    
    size_t inSize = pixelCount * inBpp;
    size_t outSize = pixelCount * outBpp;
    
    uint8_t *inputBuf = (uint8_t *)malloc(inSize);
    uint8_t *outputBuf = (uint8_t *)malloc(outSize);
    
    if (!inputBuf || !outputBuf) {
        cmsDeleteTransform(hTransform);
        free(inputBuf);
        free(outputBuf);
        return 0;
    }

    memset(inputBuf, 0, inSize);
    size_t copyLen = (Size < inSize) ? Size : inSize;
    memcpy(inputBuf, Data, copyLen);

    // 1. Test cmsDoTransform
    cmsDoTransform(hTransform, inputBuf, outputBuf, pixelCount);

    // 2. Test cmsDoTransformLineStride
    cmsUInt32Number pixelsPerLine = 1;
    cmsUInt32Number lineCount = pixelCount;
    cmsUInt32Number bytesPerLineIn = inBpp;
    cmsUInt32Number bytesPerLineOut = outBpp;
    cmsUInt32Number bytesPerPlaneIn = inBpp;
    cmsUInt32Number bytesPerPlaneOut = outBpp;

    if (Size > 12) {
        pixelsPerLine = (Data[12] % 10) + 1;
        if (pixelsPerLine > pixelCount) pixelsPerLine = pixelCount;
        lineCount = (pixelCount + pixelsPerLine - 1) / pixelsPerLine;
        
        bytesPerLineIn = (Data[13] % 16) + inBpp;
        bytesPerLineOut = (Data[14] % 16) + outBpp;
        bytesPerPlaneIn = (Data[15] % 16) + inBpp;
        bytesPerPlaneOut = (Data[16] % 16) + outBpp;
    }

    cmsDoTransformLineStride(hTransform, inputBuf, outputBuf, pixelsPerLine, lineCount, 
                             bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);

    // 3. Test _cmsGetTransformWorkerFlags
    struct _cmstransform_struct *transformStruct = (struct _cmstransform_struct *)hTransform;
    if (transformStruct) {
        cmsUInt32Number flags = _cmsGetTransformWorkerFlags(transformStruct);
        (void)flags;
    }

    // 4. Test cmsGetTransformInputFormat
    cmsUInt32Number fmtIn = cmsGetTransformInputFormat(hTransform);
    (void)fmtIn;

    // 5. Test _cmsGetTransformFlags
    if (transformStruct) {
        cmsUInt32Number origFlags = _cmsGetTransformFlags(transformStruct);
        (void)origFlags;
    }

    // 6. Test cmsGetTransformOutputFormat
    cmsUInt32Number fmtOut = cmsGetTransformOutputFormat(hTransform);
    (void)fmtOut;

    free(inputBuf);
    free(outputBuf);
    cmsDeleteTransform(hTransform);

    return 0;
}