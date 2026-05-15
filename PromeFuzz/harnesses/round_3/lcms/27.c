// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoUTF8 at cmsio1.c:1037:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsGetProfileInfoASCII at cmsio1.c:1027:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUsetWide at cmsnamed.c:413:20 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMLUtranslationsCodes at cmsnamed.c:696:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetWide at cmsnamed.c:629:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static void fuzz_cmsMLUtranslationsCodes(const cmsMLU* mlu, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return;
    
    cmsUInt32Number idx = *(const cmsUInt32Number*)data;
    char LanguageCode[3] = {0};
    char CountryCode[3] = {0};
    
    cmsMLUtranslationsCodes(mlu, idx, LanguageCode, CountryCode);
}

static void fuzz_cmsMLUgetWide(const cmsMLU* mlu, const uint8_t* data, size_t size) {
    if (size < 6) return;
    
    char LanguageCode[3] = {data[0], data[1], 0};
    char CountryCode[3] = {data[2], data[3], 0};
    cmsUInt32Number BufferSize = *(const cmsUInt32Number*)(data + 4);
    
    cmsUInt32Number needed = cmsMLUgetWide(mlu, LanguageCode, CountryCode, NULL, 0);
    if (needed > 0 && needed <= 1024) {
        wchar_t* Buffer = (wchar_t*)calloc(needed, sizeof(wchar_t));
        if (Buffer) {
            cmsMLUgetWide(mlu, LanguageCode, CountryCode, Buffer, needed);
            free(Buffer);
        }
    }
    
    if (BufferSize > 0 && BufferSize <= 1024) {
        wchar_t* Buffer = (wchar_t*)calloc(BufferSize, sizeof(wchar_t));
        if (Buffer) {
            cmsMLUgetWide(mlu, LanguageCode, CountryCode, Buffer, BufferSize);
            free(Buffer);
        }
    }
}

static void fuzz_cmsMLUgetASCII(const cmsMLU* mlu, const uint8_t* data, size_t size) {
    if (size < 6) return;
    
    char LanguageCode[3] = {data[0], data[1], 0};
    char CountryCode[3] = {data[2], data[3], 0};
    cmsUInt32Number BufferSize = *(const cmsUInt32Number*)(data + 4);
    
    cmsUInt32Number needed = cmsMLUgetASCII(mlu, LanguageCode, CountryCode, NULL, 0);
    if (needed > 0 && needed <= 1024) {
        char* Buffer = (char*)calloc(needed, sizeof(char));
        if (Buffer) {
            cmsMLUgetASCII(mlu, LanguageCode, CountryCode, Buffer, needed);
            free(Buffer);
        }
    }
    
    if (BufferSize > 0 && BufferSize <= 1024) {
        char* Buffer = (char*)calloc(BufferSize, sizeof(char));
        if (Buffer) {
            cmsMLUgetASCII(mlu, LanguageCode, CountryCode, Buffer, BufferSize);
            free(Buffer);
        }
    }
}

static void fuzz_cmsMLUgetUTF8(const cmsMLU* mlu, const uint8_t* data, size_t size) {
    if (size < 6) return;
    
    char LanguageCode[3] = {data[0], data[1], 0};
    char CountryCode[3] = {data[2], data[3], 0};
    cmsUInt32Number BufferSize = *(const cmsUInt32Number*)(data + 4);
    
    cmsUInt32Number needed = cmsMLUgetUTF8(mlu, LanguageCode, CountryCode, NULL, 0);
    if (needed > 0 && needed <= 1024) {
        char* Buffer = (char*)calloc(needed, sizeof(char));
        if (Buffer) {
            cmsMLUgetUTF8(mlu, LanguageCode, CountryCode, Buffer, needed);
            free(Buffer);
        }
    }
    
    if (BufferSize > 0 && BufferSize <= 1024) {
        char* Buffer = (char*)calloc(BufferSize, sizeof(char));
        if (Buffer) {
            cmsMLUgetUTF8(mlu, LanguageCode, CountryCode, Buffer, BufferSize);
            free(Buffer);
        }
    }
}

static void fuzz_cmsGetProfileInfoUTF8(cmsHPROFILE hProfile, const uint8_t* data, size_t size) {
    if (size < 8) return;
    
    cmsInfoType Info = *(const cmsInfoType*)(data);
    char LanguageCode[3] = {data[4], data[5], 0};
    char CountryCode[3] = {data[6], data[7], 0};
    cmsUInt32Number BufferSize = 0;
    
    if (size >= 12) {
        BufferSize = *(const cmsUInt32Number*)(data + 8);
    }
    
    cmsUInt32Number needed = cmsGetProfileInfoUTF8(hProfile, Info, LanguageCode, CountryCode, NULL, 0);
    if (needed > 0 && needed <= 1024) {
        char* Buffer = (char*)calloc(needed, sizeof(char));
        if (Buffer) {
            cmsGetProfileInfoUTF8(hProfile, Info, LanguageCode, CountryCode, Buffer, needed);
            free(Buffer);
        }
    }
    
    if (BufferSize > 0 && BufferSize <= 1024) {
        char* Buffer = (char*)calloc(BufferSize, sizeof(char));
        if (Buffer) {
            cmsGetProfileInfoUTF8(hProfile, Info, LanguageCode, CountryCode, Buffer, BufferSize);
            free(Buffer);
        }
    }
}

static void fuzz_cmsGetProfileInfoASCII(cmsHPROFILE hProfile, const uint8_t* data, size_t size) {
    if (size < 8) return;
    
    cmsInfoType Info = *(const cmsInfoType*)(data);
    char LanguageCode[3] = {data[4], data[5], 0};
    char CountryCode[3] = {data[6], data[7], 0};
    cmsUInt32Number BufferSize = 0;
    
    if (size >= 12) {
        BufferSize = *(const cmsUInt32Number*)(data + 8);
    }
    
    cmsUInt32Number needed = cmsGetProfileInfoASCII(hProfile, Info, LanguageCode, CountryCode, NULL, 0);
    if (needed > 0 && needed <= 1024) {
        char* Buffer = (char*)calloc(needed, sizeof(char));
        if (Buffer) {
            cmsGetProfileInfoASCII(hProfile, Info, LanguageCode, CountryCode, Buffer, needed);
            free(Buffer);
        }
    }
    
    if (BufferSize > 0 && BufferSize <= 1024) {
        char* Buffer = (char*)calloc(BufferSize, sizeof(char));
        if (Buffer) {
            cmsGetProfileInfoASCII(hProfile, Info, LanguageCode, CountryCode, Buffer, BufferSize);
            free(Buffer);
        }
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsMLU* mlu = cmsMLUalloc(ctx, 5);
    if (mlu) {
        if (Size >= 20) {
            char LanguageCode[3] = {Data[0], Data[1], 0};
            char CountryCode[3] = {Data[2], Data[3], 0};
            wchar_t wstr[64] = {0};
            for (size_t i = 0; i < 63 && i < Size - 4; i++) {
                wstr[i] = (wchar_t)Data[4 + i];
            }
            cmsMLUsetWide(mlu, LanguageCode, CountryCode, wstr);
        }
        
        fuzz_cmsMLUtranslationsCodes(mlu, Data, Size);
        fuzz_cmsMLUgetWide(mlu, Data, Size);
        fuzz_cmsMLUgetASCII(mlu, Data, Size);
        fuzz_cmsMLUgetUTF8(mlu, Data, Size);
        
        cmsMLUfree(mlu);
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        fclose(fp);
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    if (hProfile) {
        fuzz_cmsGetProfileInfoUTF8(hProfile, Data, Size);
        fuzz_cmsGetProfileInfoASCII(hProfile, Data, Size);
        cmsCloseProfile(hProfile);
    }
    
    remove("./dummy_file");
    cmsDeleteContext(ctx);
    
    return 0;
}