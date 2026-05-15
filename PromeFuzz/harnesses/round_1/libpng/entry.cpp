// This is the entry of 34 fuzz drivers:
// 4, 8, 26, 31, 40, 52, 59, 64, 76, 113, 151, 174, 215, 226, 233, 235, 239, 249, 2
// 53, 255, 259, 260, 267, 287, 340, 343, 381, 389, 390, 399, 403, 404, 418, 444
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_249(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_255(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_287(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_340(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_343(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_381(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_389(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_390(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_399(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_403(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_404(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_418(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_444(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 34) {
        case 0:
            return LLVMFuzzerTestOneInput_4(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_8(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_26(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_31(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_40(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_52(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_59(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_64(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_76(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_113(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_151(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_174(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_215(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_226(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_233(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_235(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_239(remainData, remainSize);
        case 17:
            return LLVMFuzzerTestOneInput_249(remainData, remainSize);
        case 18:
            return LLVMFuzzerTestOneInput_253(remainData, remainSize);
        case 19:
            return LLVMFuzzerTestOneInput_255(remainData, remainSize);
        case 20:
            return LLVMFuzzerTestOneInput_259(remainData, remainSize);
        case 21:
            return LLVMFuzzerTestOneInput_260(remainData, remainSize);
        case 22:
            return LLVMFuzzerTestOneInput_267(remainData, remainSize);
        case 23:
            return LLVMFuzzerTestOneInput_287(remainData, remainSize);
        case 24:
            return LLVMFuzzerTestOneInput_340(remainData, remainSize);
        case 25:
            return LLVMFuzzerTestOneInput_343(remainData, remainSize);
        case 26:
            return LLVMFuzzerTestOneInput_381(remainData, remainSize);
        case 27:
            return LLVMFuzzerTestOneInput_389(remainData, remainSize);
        case 28:
            return LLVMFuzzerTestOneInput_390(remainData, remainSize);
        case 29:
            return LLVMFuzzerTestOneInput_399(remainData, remainSize);
        case 30:
            return LLVMFuzzerTestOneInput_403(remainData, remainSize);
        case 31:
            return LLVMFuzzerTestOneInput_404(remainData, remainSize);
        case 32:
            return LLVMFuzzerTestOneInput_418(remainData, remainSize);
        case 33:
            return LLVMFuzzerTestOneInput_444(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
