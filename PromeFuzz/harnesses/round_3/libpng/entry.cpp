// This is the entry of 28 fuzz drivers:
// 13, 24, 48, 63, 67, 68, 74, 85, 94, 96, 105, 107, 129, 147, 156, 192, 195, 198, 
// 244, 253, 278, 307, 311, 314, 329, 392, 398, 405
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_278(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_311(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_314(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_329(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_392(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_398(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_405(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 28) {
        case 0:
            return LLVMFuzzerTestOneInput_13(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_24(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_48(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_63(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_67(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_68(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_74(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_85(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_94(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_96(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_105(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_107(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_129(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_147(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_156(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_192(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_195(remainData, remainSize);
        case 17:
            return LLVMFuzzerTestOneInput_198(remainData, remainSize);
        case 18:
            return LLVMFuzzerTestOneInput_244(remainData, remainSize);
        case 19:
            return LLVMFuzzerTestOneInput_253(remainData, remainSize);
        case 20:
            return LLVMFuzzerTestOneInput_278(remainData, remainSize);
        case 21:
            return LLVMFuzzerTestOneInput_307(remainData, remainSize);
        case 22:
            return LLVMFuzzerTestOneInput_311(remainData, remainSize);
        case 23:
            return LLVMFuzzerTestOneInput_314(remainData, remainSize);
        case 24:
            return LLVMFuzzerTestOneInput_329(remainData, remainSize);
        case 25:
            return LLVMFuzzerTestOneInput_392(remainData, remainSize);
        case 26:
            return LLVMFuzzerTestOneInput_398(remainData, remainSize);
        case 27:
            return LLVMFuzzerTestOneInput_405(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
