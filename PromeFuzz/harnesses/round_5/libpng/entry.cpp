// This is the entry of 33 fuzz drivers:
// 8, 14, 35, 48, 65, 66, 77, 82, 91, 92, 108, 132, 142, 161, 163, 190, 193, 198, 2
// 07, 210, 216, 217, 221, 225, 235, 239, 241, 247, 260, 298, 322, 364, 365
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_190(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_207(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_217(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_225(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_298(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_322(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_364(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_365(const uint8_t *Data, size_t Size);

// Entry function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Driver selector
    if (Size < 1) {
        return 0;
    }
    const uint8_t *selector = Data;
    unsigned int driverIndex = 0;
    memcpy(&driverIndex, selector, 1);

    // Remain data and size
    const uint8_t *remainData = Data + 1;
    size_t remainSize = Size - 1;
    if (remainSize == 0) {
        return 0;
    }

    // Select driver
    switch (driverIndex % 33) {
        case 0:
            return LLVMFuzzerTestOneInput_8(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_14(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_35(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_48(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_65(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_66(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_77(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_82(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_91(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_92(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_108(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_132(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_142(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_161(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_163(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_190(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_193(remainData, remainSize);
        case 17:
            return LLVMFuzzerTestOneInput_198(remainData, remainSize);
        case 18:
            return LLVMFuzzerTestOneInput_207(remainData, remainSize);
        case 19:
            return LLVMFuzzerTestOneInput_210(remainData, remainSize);
        case 20:
            return LLVMFuzzerTestOneInput_216(remainData, remainSize);
        case 21:
            return LLVMFuzzerTestOneInput_217(remainData, remainSize);
        case 22:
            return LLVMFuzzerTestOneInput_221(remainData, remainSize);
        case 23:
            return LLVMFuzzerTestOneInput_225(remainData, remainSize);
        case 24:
            return LLVMFuzzerTestOneInput_235(remainData, remainSize);
        case 25:
            return LLVMFuzzerTestOneInput_239(remainData, remainSize);
        case 26:
            return LLVMFuzzerTestOneInput_241(remainData, remainSize);
        case 27:
            return LLVMFuzzerTestOneInput_247(remainData, remainSize);
        case 28:
            return LLVMFuzzerTestOneInput_260(remainData, remainSize);
        case 29:
            return LLVMFuzzerTestOneInput_298(remainData, remainSize);
        case 30:
            return LLVMFuzzerTestOneInput_322(remainData, remainSize);
        case 31:
            return LLVMFuzzerTestOneInput_364(remainData, remainSize);
        case 32:
            return LLVMFuzzerTestOneInput_365(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
