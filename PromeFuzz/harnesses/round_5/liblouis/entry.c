// This is the entry of 17 fuzz drivers:
// 3, 5, 8, 14, 17, 19, 20, 21, 24, 25, 28, 29, 31, 32, 33, 35, 41
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);

// Entry function
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
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
    switch (driverIndex % 17) {
        case 0:
            return LLVMFuzzerTestOneInput_3(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_5(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_8(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_14(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_17(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_19(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_20(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_21(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_24(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_25(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_28(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_29(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_31(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_32(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_33(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_35(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_41(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
