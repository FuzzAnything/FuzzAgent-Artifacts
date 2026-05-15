#include <stddef.h>
#include <stdint.h>

#include <FuzzedDataProvider.h>

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *data, size_t size);

using FuzzerEntry = int (*)(const uint8_t *data, size_t size);

static constexpr FuzzerEntry kEntries[] = {
	LLVMFuzzerTestOneInput_0,
	LLVMFuzzerTestOneInput_1,
	LLVMFuzzerTestOneInput_2,
	LLVMFuzzerTestOneInput_3,
	LLVMFuzzerTestOneInput_4,
	LLVMFuzzerTestOneInput_5,
	LLVMFuzzerTestOneInput_6,
	LLVMFuzzerTestOneInput_7,
	LLVMFuzzerTestOneInput_8,
	LLVMFuzzerTestOneInput_9,
	LLVMFuzzerTestOneInput_10,
	LLVMFuzzerTestOneInput_11,
	LLVMFuzzerTestOneInput_12,
	LLVMFuzzerTestOneInput_13,
	LLVMFuzzerTestOneInput_14,
	LLVMFuzzerTestOneInput_15,
	LLVMFuzzerTestOneInput_16,
	LLVMFuzzerTestOneInput_17,
	LLVMFuzzerTestOneInput_18,
	LLVMFuzzerTestOneInput_19,
	LLVMFuzzerTestOneInput_20,
	LLVMFuzzerTestOneInput_21,
	LLVMFuzzerTestOneInput_22,
	LLVMFuzzerTestOneInput_23,
	LLVMFuzzerTestOneInput_24,
	LLVMFuzzerTestOneInput_25,
	LLVMFuzzerTestOneInput_26,
	LLVMFuzzerTestOneInput_27,
	LLVMFuzzerTestOneInput_28,
	LLVMFuzzerTestOneInput_29,
	LLVMFuzzerTestOneInput_30,
	LLVMFuzzerTestOneInput_31,
	LLVMFuzzerTestOneInput_32,
	LLVMFuzzerTestOneInput_33,
	LLVMFuzzerTestOneInput_34,
	LLVMFuzzerTestOneInput_35,
	LLVMFuzzerTestOneInput_36,
	LLVMFuzzerTestOneInput_37,
	LLVMFuzzerTestOneInput_38,
	LLVMFuzzerTestOneInput_39,
	LLVMFuzzerTestOneInput_40,
	LLVMFuzzerTestOneInput_41,
	LLVMFuzzerTestOneInput_42,
	LLVMFuzzerTestOneInput_43,
	LLVMFuzzerTestOneInput_44,
	LLVMFuzzerTestOneInput_45,
	LLVMFuzzerTestOneInput_46,
	LLVMFuzzerTestOneInput_47,
	LLVMFuzzerTestOneInput_48,
	LLVMFuzzerTestOneInput_49,
	LLVMFuzzerTestOneInput_50,
	LLVMFuzzerTestOneInput_51,
	LLVMFuzzerTestOneInput_52,
	LLVMFuzzerTestOneInput_53,
	LLVMFuzzerTestOneInput_54,
	LLVMFuzzerTestOneInput_55,
	LLVMFuzzerTestOneInput_56,
	LLVMFuzzerTestOneInput_57,
	LLVMFuzzerTestOneInput_58,
	LLVMFuzzerTestOneInput_59,
	LLVMFuzzerTestOneInput_60,
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	if (size < sizeof(uint16_t)) {
		return 0;
	}

	FuzzedDataProvider fdp(data, size);
	const uint16_t switch_id = fdp.ConsumeIntegral<uint16_t>();
	const uint8_t *input = data + sizeof(uint16_t);
	const size_t input_size = size - sizeof(uint16_t);
	const size_t entry_index = switch_id % (sizeof(kEntries) / sizeof(kEntries[0]));

	return kEntries[entry_index](input, input_size);
}