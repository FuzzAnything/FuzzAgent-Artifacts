#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/unknown_field_set.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

namespace harness_006 {
// Fuzzing harness for Google Protocol Buffers Unknown Field Set operations
// Targets UnknownFieldSet and UnknownField APIs: AddVarint, AddFixed32, AddFixed64, 
// AddLengthDelimited, MergeFrom, ParseFromArray, SerializeToString, field operations
// Focuses on unknown field handling and arena allocation - semantically different from existing harnesses
// Differentiates from existing harnesses:
// - harness_000: message operations (binary parsing/serialization)
// - harness_001: Tokenizer parsing (low-level text parsing)
// - harness_002: CordInputStream/CordOutputStream (I/O streams with Cord)
// - harness_003: CodedInputStream/CodedOutputStream (binary I/O with Cord)
// - harness_004: JSON format conversion
// - harness_005: service/method descriptor reflection
// This harness targets unknown field handling and arena allocation APIs



// Unknown field headers

using google::protobuf::UnknownFieldSet;
using google::protobuf::UnknownField;
using google::protobuf::Arena;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::ArrayOutputStream;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful unknown field testing
    if (size < 32) {
        return 0;  // Insufficient input
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool use_arena = fdp.ConsumeBool();
    bool create_multiple_sets = fdp.ConsumeBool();
    bool use_cord_for_strings = fdp.ConsumeBool();
    
    // Consume parameters for field creation
    int max_field_count = fdp.ConsumeIntegralInRange<int>(1, 20);
    int max_field_number = fdp.ConsumeIntegralInRange<int>(1, 1000);
    size_t max_string_length = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Create arena if requested
    std::unique_ptr<Arena> arena;
    if (use_arena) {
        arena.reset(new Arena());
    }
    
    // Create primary unknown field set
    UnknownFieldSet* unknown_set;
    if (use_arena && arena) {
        unknown_set = Arena::Create<UnknownFieldSet>(arena.get());
    } else {
        unknown_set = new UnknownFieldSet();
    }
    
    // Add fields to the unknown field set based on fuzzed input
    int fields_added = 0;
    while (fields_added < max_field_count && fdp.remaining_bytes() > 16) {
        uint8_t field_type = fdp.ConsumeIntegral<uint8_t>() % 5;
        int field_number = fdp.ConsumeIntegralInRange<int>(1, max_field_number);
        
        switch (field_type) {
            case 0: {  // TYPE_VARINT
                uint64_t varint_value = fdp.ConsumeIntegral<uint64_t>();
                unknown_set->AddVarint(field_number, varint_value);
                fields_added++;
                break;
            }
            case 1: {  // TYPE_FIXED32
                uint32_t fixed32_value = fdp.ConsumeIntegral<uint32_t>();
                unknown_set->AddFixed32(field_number, fixed32_value);
                fields_added++;
                break;
            }
            case 2: {  // TYPE_FIXED64
                uint64_t fixed64_value = fdp.ConsumeIntegral<uint64_t>();
                unknown_set->AddFixed64(field_number, fixed64_value);
                fields_added++;
                break;
            }
            case 3: {  // TYPE_LENGTH_DELIMITED
                size_t string_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(max_string_length, fdp.remaining_bytes()));
                std::string string_value = fdp.ConsumeBytesAsString(string_len);
                
                if (use_cord_for_strings && fdp.ConsumeBool()) {
                    // Use Cord for length-delimited fields
                    absl::Cord cord_value(string_value);
                    unknown_set->AddLengthDelimited(field_number, cord_value);
                } else {
                    unknown_set->AddLengthDelimited(field_number, string_value);
                }
                fields_added++;
                break;
            }
            case 4: {  // TYPE_GROUP
                // Create a nested group
                if (fdp.remaining_bytes() > 32) {
                    UnknownFieldSet* group = unknown_set->AddGroup(field_number);
                    if (group) {
                        // Add a simple varint field to the group
                        uint64_t group_varint = fdp.ConsumeIntegral<uint64_t>();
                        group->AddVarint(field_number + 1, group_varint);
                        fields_added++;
                    }
                }
                break;
            }
        }
    }
    
    // Test different operations based on test_mode
    switch (test_mode) {
        case 0: {
            // Test 1: Field access and manipulation
            int field_count = unknown_set->field_count();
            if (field_count > 0) {
                // Access some fields
                for (int i = 0; i < std::min(5, field_count); i++) {
                    const UnknownField& field = unknown_set->field(i);
                    int number = field.number();
                    UnknownField::Type type = field.type();
                    
                    // Access field value based on type
                    switch (type) {
                        case UnknownField::TYPE_VARINT:
                            field.varint();
                            break;
                        case UnknownField::TYPE_FIXED32:
                            field.fixed32();
                            break;
                        case UnknownField::TYPE_FIXED64:
                            field.fixed64();
                            break;
                        case UnknownField::TYPE_LENGTH_DELIMITED:
                            field.length_delimited();
                            break;
                        case UnknownField::TYPE_GROUP:
                            field.group();
                            break;
                    }
                }
                
                // Test mutable field access
                if (field_count > 0) {
                    UnknownField* mutable_field = unknown_set->mutable_field(0);
                    if (mutable_field) {
                        UnknownField::Type type = mutable_field->type();
                        if (type == UnknownField::TYPE_VARINT) {
                            mutable_field->set_varint(mutable_field->varint() + 1);
                        }
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test 2: Serialization and parsing
            // Serialize to string
            std::string serialized;
            bool serialize_success = unknown_set->SerializeToString(&serialized);
            
            if (serialize_success && !serialized.empty()) {
                // Create a new set and parse from the serialized data
                UnknownFieldSet parsed_set;
                bool parse_success = parsed_set.ParseFromArray(serialized.data(), serialized.size());
                
                if (parse_success) {
                    // Merge parsed set back into original
                    unknown_set->MergeFrom(parsed_set);
                    
                    // Test space usage calculations
                    unknown_set->SpaceUsedLong();
                    unknown_set->SpaceUsedExcludingSelfLong();
                }
            }
            break;
        }
        
        case 2: {
            // Test 3: Multiple set operations
            if (create_multiple_sets && fdp.remaining_bytes() > 32) {
                // Create a second unknown field set
                UnknownFieldSet other_set;
                
                // Add some fields to the other set
                int other_fields = fdp.ConsumeIntegralInRange<int>(1, 5);
                for (int i = 0; i < other_fields && fdp.remaining_bytes() > 8; i++) {
                    int field_num = fdp.ConsumeIntegralInRange<int>(1000, 2000);
                    uint64_t value = fdp.ConsumeIntegral<uint64_t>();
                    other_set.AddVarint(field_num, value);
                }
                
                // Test merge operations
                unknown_set->MergeFrom(other_set);
                
                // Test swap operation (if arenas are compatible)
                if (!use_arena) {
                    UnknownFieldSet swap_set;
                    swap_set.AddVarint(9999, fdp.ConsumeIntegral<uint64_t>());
                    unknown_set->Swap(&swap_set);
                }
                
                // Test deletion operations
                if (unknown_set->field_count() > 0) {
                    // Delete a range of fields
                    int start = fdp.ConsumeIntegralInRange<int>(0, unknown_set->field_count() - 1);
                    int num_to_delete = fdp.ConsumeIntegralInRange<int>(1, unknown_set->field_count() - start);
                    unknown_set->DeleteSubrange(start, num_to_delete);
                    
                    // Delete by number if any fields remain
                    if (unknown_set->field_count() > 0) {
                        const UnknownField& first_field = unknown_set->field(0);
                        unknown_set->DeleteByNumber(first_field.number());
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test 4: Stream-based parsing and serialization
            if (fdp.remaining_bytes() > 64) {
                // Create test data for parsing
                size_t parse_data_size = fdp.ConsumeIntegralInRange<size_t>(32, std::min<size_t>(1024, fdp.remaining_bytes()));
                std::vector<uint8_t> parse_data = fdp.ConsumeBytes<uint8_t>(parse_data_size);
                
                // Try to parse from array
                UnknownFieldSet parsed_from_array;
                parsed_from_array.ParseFromArray(parse_data.data(), parse_data.size());
                
                // Try to parse from string view
                std::string parse_string(reinterpret_cast<const char*>(parse_data.data()), parse_data.size());
                parsed_from_array.ParseFromString(parse_string);
                
                // Test with CodedInputStream
                if (parse_data.size() > 16) {
                    ArrayInputStream array_stream(parse_data.data(), parse_data.size());
                    CodedInputStream coded_stream(&array_stream);
                    
                    UnknownFieldSet parsed_from_stream;
                    parsed_from_stream.ParseFromCodedStream(&coded_stream);
                }
                
                // Test serialization to CodedOutputStream
                if (unknown_set->field_count() > 0) {
                    std::vector<uint8_t> output_buffer(4096);
                    ArrayOutputStream array_output(output_buffer.data(), output_buffer.size());
                    CodedOutputStream coded_output(&array_output);
                    
                    unknown_set->SerializeToCodedStream(&coded_output);
                }
            }
            break;
        }
    }
    
    // Clean up
    if (!use_arena || !arena) {
        delete unknown_set;
    }
    
    return 0;
}

} // namespace harness_006
