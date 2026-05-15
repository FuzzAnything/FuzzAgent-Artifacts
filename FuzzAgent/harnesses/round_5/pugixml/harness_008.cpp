#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <climits>
#include <string>
#include <vector>
#include <algorithm>
#include "pugixml.hpp"

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic XML and operations
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different tree manipulation modes
    bool test_node_operations = fdp.ConsumeBool();
    bool test_attribute_operations = fdp.ConsumeBool();
    bool test_move_vs_copy = fdp.ConsumeBool();
    bool test_before_vs_after = fdp.ConsumeBool();
    bool test_char_vs_stringview = fdp.ConsumeBool();
    bool test_complex_sequences = fdp.ConsumeBool();
    
    // Consume string data for node/attribute names and values
    std::string root_name = fdp.ConsumeRandomLengthString(32);
    std::string child1_name = fdp.ConsumeRandomLengthString(32);
    std::string child2_name = fdp.ConsumeRandomLengthString(32);
    std::string child3_name = fdp.ConsumeRandomLengthString(32);
    std::string attr1_name = fdp.ConsumeRandomLengthString(32);
    std::string attr2_name = fdp.ConsumeRandomLengthString(32);
    std::string attr3_name = fdp.ConsumeRandomLengthString(32);
    std::string attr4_name = fdp.ConsumeRandomLengthString(32);
    std::string text_value1 = fdp.ConsumeRandomLengthString(256);
    std::string text_value2 = fdp.ConsumeRandomLengthString(256);
    
    // Consume numeric data
    int int_val1 = fdp.ConsumeIntegral<int>();
    int int_val2 = fdp.ConsumeIntegral<int>();
    double double_val = fdp.ConsumeFloatingPoint<double>();
    bool bool_val = fdp.ConsumeBool();
    
    // Create XML document with complex structure
    pugi::xml_document doc;
    
    // Parse XML from remaining bytes
    std::string xml_data = fdp.ConsumeRemainingBytesAsString();
    if (xml_data.empty()) {
        // If no data left, create a simple default XML
        xml_data = "<root><parent id='1'><child1>text1</child1><child2>text2</child2></parent><parent id='2'><child3 attr='value'>text3</child3></parent></root>";
    }
    
    pugi::xml_parse_result result = doc.load_buffer(xml_data.c_str(), xml_data.size());
    if (!result) {
        // If parsing fails, create document from scratch using fuzzer-provided data
        pugi::xml_node root = doc.append_child(root_name.empty() ? "root" : root_name.c_str());
        
        // Create initial structure for testing using fuzzer-provided strings
        pugi::xml_node parent1 = root.append_child("parent1");
        // Use safe casting and absolute value to avoid overflow and negative modulus issues
        int64_t safe_int_val1 = static_cast<int64_t>(int_val1);
        int64_t safe_int_val2 = static_cast<int64_t>(int_val2);
        // Ensure non-negative for modulus operation
        int64_t mod_val1 = (safe_int_val1 < 0) ? -safe_int_val1 : safe_int_val1;
        int64_t mod_val2 = (safe_int_val2 < 0) ? -safe_int_val2 : safe_int_val2;
        std::string parent1_id = "id_" + std::to_string(mod_val1 % 1000);
        parent1.append_attribute("id") = parent1_id.c_str();
        
        pugi::xml_node child1 = parent1.append_child("child1");
        child1.append_attribute("attr1") = text_value1.empty() ? "default1" : text_value1.c_str();
        child1.text().set(text_value1.empty() ? "default_text1" : text_value1.c_str());
        
        pugi::xml_node child2 = parent1.append_child("child2");
        child2.append_attribute("attr2") = text_value2.empty() ? "default2" : text_value2.c_str();
        child2.text().set(text_value2.empty() ? "default_text2" : text_value2.c_str());
        
        pugi::xml_node parent2 = root.append_child("parent2");
        std::string parent2_id = "id_" + std::to_string(mod_val2 % 1000);
        parent2.append_attribute("id") = parent2_id.c_str();
        
        pugi::xml_node child3 = parent2.append_child("child3");
        std::string attr3_value = "val_" + std::to_string(double_val);
        child3.append_attribute("attr3") = attr3_value.c_str();
        child3.text().set(bool_val ? "true_text" : "false_text");
    }
    
    // Get reference nodes for manipulation
    pugi::xml_node root = doc.first_child();
    if (!root) return 0;
    
    pugi::xml_node parent1 = root.child("parent1");
    pugi::xml_node parent2 = root.child("parent2");
    
    // Ensure we have at least some nodes to work with
    if (!parent1) {
        parent1 = root.append_child("parent1");
        parent1.append_attribute("default") = "true";
    }
    if (!parent2) {
        parent2 = root.append_child("parent2");
        parent2.append_attribute("default") = "true";
    }
    
    // Get or create child nodes
    pugi::xml_node child1 = parent1.child("child1");
    pugi::xml_node child2 = parent1.child("child2");
    pugi::xml_node child3 = parent2.child("child3");
    
    if (!child1) child1 = parent1.append_child("child1");
    if (!child2) child2 = parent1.append_child("child2");
    if (!child3) child3 = parent2.append_child("child3");
    
    // Create attributes for testing
    pugi::xml_attribute attr1 = child1.attribute("attr1");
    if (!attr1) attr1 = child1.append_attribute("attr1");
    
    pugi::xml_attribute attr2 = child2.attribute("attr2");
    if (!attr2) attr2 = child2.append_attribute("attr2");
    
    pugi::xml_attribute attr3 = child3.attribute("attr3");
    if (!attr3) attr3 = child3.append_attribute("attr3");
    
    // Now test the target APIs based on operation mode
    
    // 1. Test xml_node::insert_move_after (TARGET API 1)
    if (test_node_operations && child1 && child2) {
        // Create a new node to move
        pugi::xml_node node_to_move = root.append_child(child1_name.empty() ? "node_to_move" : child1_name.c_str());
        node_to_move.text().set(text_value1.c_str());
        
        // Move node_to_move after child2
        pugi::xml_node moved_node = parent1.insert_move_after(node_to_move, child2);
        (void)moved_node; // Use moved_node to avoid unused variable warning
    }
    
    // 2. Test xml_node::insert_copy_after for attributes (TARGET API 2)
    if (test_attribute_operations && attr1 && attr2) {
        // Create a new attribute
        pugi::xml_attribute new_attr = child1.append_attribute(attr3_name.empty() ? "new_attr" : attr3_name.c_str());
        new_attr.set_value(int_val1);
        
        // Copy new_attr after attr2
        pugi::xml_attribute copied_attr = child1.insert_copy_after(new_attr, attr2);
        (void)copied_attr;
    }
    
    // 3. Test xml_node::insert_move_before for nodes (TARGET API 3)
    if (test_node_operations && child1 && child2 && test_before_vs_after) {
        // Create a node to move before
        pugi::xml_node node_to_move_before = root.append_child(child2_name.empty() ? "node_to_move_before" : child2_name.c_str());
        node_to_move_before.text().set(text_value2.c_str());
        
        // Move node_to_move_before before child1
        pugi::xml_node moved_before_node = parent1.insert_move_before(node_to_move_before, child1);
        (void)moved_before_node;
    }
    
    // 4. Test xml_node::insert_attribute_before (TARGET API 4)
    if (test_attribute_operations && attr1 && attr2 && test_before_vs_after) {
        // Insert attribute before attr1
        pugi::xml_attribute inserted_before = child1.insert_attribute_before(
            attr4_name.empty() ? "inserted_before" : attr4_name.c_str(),
            attr1
        );
        if (inserted_before) {
            inserted_before.set_value(int_val2);
        }
        (void)inserted_before;
    }
    
    // 5. Test xml_node::insert_attribute_after (TARGET API 5)
    if (test_attribute_operations && attr1 && attr2) {
        // Insert attribute after attr2
        pugi::xml_attribute inserted_after = child1.insert_attribute_after(
            "inserted_after",
            attr2
        );
        if (inserted_after) {
            inserted_after.set_value(double_val);
        }
        (void)inserted_after;
    }
    
    // 6. Test xml_node::insert_copy_before for attributes (TARGET API 6)
    if (test_attribute_operations && attr1 && attr2 && test_move_vs_copy) {
        // Copy attr1 before attr2
        pugi::xml_attribute copied_before_attr = child1.insert_copy_before(attr1, attr2);
        (void)copied_before_attr;
    }
    
    // 7. Test xml_attribute::insert_copy_before (TARGET API 7)
    if (test_attribute_operations && attr1 && attr2 && test_move_vs_copy) {
        // Copy attr2 before attr3 (on child3)
        pugi::xml_attribute attr2_copy = child3.insert_copy_before(attr2, attr3);
        (void)attr2_copy;
    }
    
    // 8. Test xml_node::insert_attribute_before with string_view (TARGET API 8)
    if (test_attribute_operations && attr2 && test_char_vs_stringview) {
#ifdef PUGIXML_HAS_STRING_VIEW
        std::string_view sv_name("inserted_before_sv");
        pugi::xml_attribute inserted_before_sv = child1.insert_attribute_before(
            sv_name, 
            attr2
        );
        if (inserted_before_sv) {
            inserted_before_sv.set_value(bool_val);
        }
        (void)inserted_before_sv;
#endif
    }
    
    // 9. Test xml_node::insert_attribute_after with string_view (TARGET API 9)
    if (test_attribute_operations && attr3 && test_char_vs_stringview) {
#ifdef PUGIXML_HAS_STRING_VIEW
        std::string_view sv_name2("inserted_after_sv");
        pugi::xml_attribute inserted_after_sv = child3.insert_attribute_after(
            sv_name2, 
            attr3
        );
        if (inserted_after_sv) {
            inserted_after_sv.set_value(double_val * 2);
        }
        (void)inserted_after_sv;
#endif
    }
    
    // 10. Test xml_node::insert_copy_after for nodes (TARGET API 10)
    if (test_node_operations && child1 && child2 && test_move_vs_copy) {
        // Copy child2 after child1
        pugi::xml_node copied_node = parent1.insert_copy_after(child2, child1);
        (void)copied_node;
    }
    
    // 11. Test xml_node::insert_copy_before for nodes (TARGET API 11)
    if (test_node_operations && child1 && child2 && test_move_vs_copy && test_before_vs_after) {
        // Copy child1 before child2
        pugi::xml_node copied_before_node = parent1.insert_copy_before(child1, child2);
        (void)copied_before_node;
    }
    
    // Complex sequences: Test multiple operations in sequence
    if (test_complex_sequences) {
        // Create a complex manipulation sequence
        pugi::xml_node complex_root = doc.append_child("complex_root");
        
        // Create initial structure using fuzzer-provided data
        pugi::xml_node base1 = complex_root.append_child("base1");
        pugi::xml_node base2 = complex_root.append_child("base2");
        pugi::xml_node base3 = complex_root.append_child("base3");
        
        // Use safe arithmetic to avoid integer overflow
        int64_t safe_int_val1 = static_cast<int64_t>(int_val1);
        int64_t safe_int_val2 = static_cast<int64_t>(int_val2);
        // Ensure non-negative for modulus operation
        int64_t mod_val1 = (safe_int_val1 < 0) ? -safe_int_val1 : safe_int_val1;
        int64_t mod_val2 = (safe_int_val2 < 0) ? -safe_int_val2 : safe_int_val2;
        // Safe addition using checked addition to avoid overflow
        int64_t sum = 0;
        if ((safe_int_val1 > 0 && safe_int_val2 > 0 && safe_int_val1 > INT64_MAX - safe_int_val2) ||
            (safe_int_val1 < 0 && safe_int_val2 < 0 && safe_int_val1 < INT64_MIN - safe_int_val2)) {
            // Overflow would occur, use a safe value
            sum = 0;
        } else {
            sum = safe_int_val1 + safe_int_val2;
        }
        int64_t mod_sum = (sum < 0) ? -sum : sum;
        std::string base_id1 = "b" + std::to_string(mod_val1 % 100);
        std::string base_id2 = "b" + std::to_string(mod_val2 % 100);
        std::string base_id3 = "b" + std::to_string(mod_sum % 100);
        base1.append_attribute("id") = base_id1.c_str();
        base2.append_attribute("id") = base_id2.c_str();
        base3.append_attribute("id") = base_id3.c_str();
        
        // Sequence 1: Move then copy using fuzzer data
        pugi::xml_node temp = complex_root.append_child("temp");
        temp.text().set(text_value1.empty() ? "temp_text" : text_value1.c_str());
        complex_root.insert_move_after(temp, base1);
        
        pugi::xml_node temp_copy = complex_root.insert_copy_after(temp, base2);
        (void)temp_copy;
        
        // Sequence 2: Attribute operations using fuzzer data
        pugi::xml_attribute base_attr1 = base1.append_attribute("base_attr1");
        base_attr1.set_value(text_value1.empty() ? "val1" : text_value1.c_str());
        
        pugi::xml_attribute base_attr2 = base1.append_attribute("base_attr2");
        base_attr2.set_value(text_value2.empty() ? "val2" : text_value2.c_str());
        
        // Insert attribute before using fuzzer data
        std::string inserted_attr_name = "inserted_" + std::to_string(double_val);
        base1.insert_attribute_before(inserted_attr_name.c_str(), base_attr2);
        
        // Copy attribute after
        base1.insert_copy_after(base_attr1, base_attr2);
        
        // Sequence 3: Mixed node/attribute operations using fuzzer data
        pugi::xml_node mixed = complex_root.append_child("mixed");
        mixed.text().set(bool_val ? "mixed_true" : "mixed_false");
        
        pugi::xml_attribute mixed_attr = mixed.append_attribute("mixed_attr");
        mixed_attr.set_value(std::to_string(double_val).c_str());
        
        // Move mixed node
        complex_root.insert_move_before(mixed, base3);
        
        // Set value with different types using fuzzer data
        mixed.set_value(text_value1.empty() ? "updated" : text_value1.c_str());
#ifdef PUGIXML_HAS_STRING_VIEW
        std::string mixed_sv_str = text_value2.empty() ? "sv_content" : text_value2;
        std::string_view mixed_sv(mixed_sv_str);
        mixed.set_value(mixed_sv);
#endif
    }
    // Test edge cases and error conditions
    switch (operation_mode) {
        case 0: {
            // Test with empty nodes
            pugi::xml_node empty1, empty2;
            // complex_root might not exist, check if test_complex_sequences created it
            if (test_complex_sequences) {
                // complex_root was created inside the if block, need to recreate or skip
                pugi::xml_node local_complex_root = doc.append_child("edge_case_root");
                local_complex_root.insert_move_after(empty1, empty2);
                local_complex_root.insert_copy_after(empty1, empty2);
            } else {
                // Use parent1 as target
                parent1.insert_move_after(empty1, empty2);
                parent1.insert_copy_after(empty1, empty2);
            }
            break;
        }
            
        case 1: {
            // Test self-insertion (should fail)
            if (child1) {
                parent1.insert_move_after(child1, child1);
                parent1.insert_copy_after(child1, child1);
            }
            break;
        }
            
        case 2: {
            // Test inserting node into itself (should fail)
            if (child1 && child1.first_child()) {
                child1.insert_move_after(child1, child1.first_child());
            }
            break;
        }
            
        case 3: {
            // Test with document node
            doc.insert_move_after(root, pugi::xml_node());
            doc.insert_copy_after(root, pugi::xml_node());
            break;
        }
            
        case 4: {
            // Test attribute operations with invalid nodes
            pugi::xml_node invalid_node;
            pugi::xml_attribute invalid_attr;
            invalid_node.insert_attribute_before("test", invalid_attr);
            invalid_node.insert_copy_after(invalid_attr, invalid_attr);
            break;
        }
            
        case 5: {
            // Test string operations with empty strings
            if (child1) {
                child1.set_value("");
                child1.set_value("", 0);
#ifdef PUGIXML_HAS_STRING_VIEW
                std::string_view empty_sv("");
                child1.set_value(empty_sv);
#endif
            }
            break;
        }
            
        case 6: {
            // Test with very long strings
            if (child2) {
                std::string long_str(1000, 'x');
                child2.set_value(long_str.c_str());
                child2.set_value(long_str.c_str(), 500);
            }
            break;
        }
            
        case 7: {
            // Test with special characters
            if (child3) {
                child3.set_value("value with <>&\"' special chars");
                child3.set_value("value with <>&\"' special chars", 20);
            }
            break;
        }
    }
    
    // Clean up
    doc.reset();
    
    return 0;
}

} // namespace harness_008
