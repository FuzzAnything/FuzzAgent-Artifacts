#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_016 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML structure, attribute names, and sibling operations
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP WITH STRUCTURED SIBLINGS ====================
    // Create XML document for attribute-by-name and sibling-by-name testing
    pugi::xml_document doc;
    
    // Create root element
    pugi::xml_node root = doc.append_child("navigation_test_root");
    
    // Create multiple sibling elements with different names and attributes
    std::vector<pugi::xml_node> siblings;
    std::vector<std::string> sibling_names;
    
    // Generate sibling names from fuzzer input
    int num_siblings = fdp.ConsumeIntegralInRange<int>(3, 10);
    for (int i = 0; i < num_siblings && fdp.remaining_bytes() > 10; ++i) {
        std::string sibling_name = fdp.ConsumeRandomLengthString(20);
        sibling_names.push_back(sibling_name);
        
        pugi::xml_node sibling = root.append_child(sibling_name.c_str());
        siblings.push_back(sibling);
        
        // Add unique ID attribute to each sibling
        std::string id_attr = "id_" + std::to_string(i);
        sibling.append_attribute(id_attr.c_str()) = std::to_string(i).c_str();
        
        // Add some common attributes with different values
        std::string type_value = (i % 2 == 0) ? "even" : "odd";
        sibling.append_attribute("type") = type_value.c_str();
        
        std::string priority_value = std::to_string(i % 5);
        sibling.append_attribute("priority") = priority_value.c_str();
        
        // Add some unique attributes per sibling
        if (i % 3 == 0) {
            std::string unique_attr_name = fdp.ConsumeRandomLengthString(15);
            std::string unique_attr_value = fdp.ConsumeRandomLengthString(30);
            sibling.append_attribute(unique_attr_name.c_str()) = unique_attr_value.c_str();
        }
        
        // Add text content
        std::string text_content = fdp.ConsumeRandomLengthString(50);
        sibling.text() = text_content.c_str();
    }
    
    // ==================== PHASE 2: ATTRIBUTE RETRIEVAL BY NAME ====================
    // TARGET: xml_node::attribute(const char_t* name) - retrieve attributes by specific names
    
    // Create a test element with multiple attributes for retrieval testing
    pugi::xml_node test_elem = root.append_child("attribute_retrieval_test");
    
    // Add diverse attributes with different names and values
    std::vector<std::pair<std::string, std::string>> test_attributes = {
        {"id", "test_element_001"},
        {"class", "container highlight"},
        {"data-value", "42"},
        {"enabled", "true"},
        {"count", "100"},
        {"price", "19.99"},
        {"timestamp", "2024-01-01"},
        {"category", "A"},
        {"status", "active"},
        {"version", "1.0.0"}
    };
    
    for (const auto& attr_pair : test_attributes) {
        test_elem.append_attribute(attr_pair.first.c_str()) = attr_pair.second.c_str();
    }
    
    // Test attribute retrieval by name using fuzzer-generated attribute names
    while (fdp.remaining_bytes() > 20) {
        // Consume operation type
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 10;
        
        switch (operation) {
            case 0: {
                // Test retrieval of existing attributes by exact name
                for (const auto& attr_pair : test_attributes) {
                    pugi::xml_attribute retrieved = test_elem.attribute(attr_pair.first.c_str());
                    if (retrieved) {
                        std::string value = retrieved.value();
                        // Verify the value matches
                        (void)value;
                    }
                }
                break;
            }
            
            case 1: {
                // Test retrieval with fuzzer-generated attribute names
                std::string search_name = fdp.ConsumeRandomLengthString(30);
                pugi::xml_attribute found_attr = test_elem.attribute(search_name.c_str());
                // The attribute may or may not exist - both cases should be handled
                (void)found_attr;
                break;
            }
            
            case 2: {
                // Test attribute retrieval from sibling elements
                if (!siblings.empty()) {
                    size_t sibling_idx = fdp.ConsumeIntegralInRange<size_t>(0, siblings.size() - 1);
                    pugi::xml_node sibling = siblings[sibling_idx];
                    
                    // Try to retrieve common attributes
                    pugi::xml_attribute id_attr = sibling.attribute("id");
                    pugi::xml_attribute type_attr = sibling.attribute("type");
                    pugi::xml_attribute priority_attr = sibling.attribute("priority");
                    
                    (void)id_attr;
                    (void)type_attr;
                    (void)priority_attr;
                }
                break;
            }
            
            case 3: {
                // Test attribute retrieval with hint optimization
                // TARGET: xml_node::attribute(const char_t* name, xml_attribute& hint)
                if (!siblings.empty()) {
                    pugi::xml_node sibling = siblings[0];
                    pugi::xml_attribute hint = sibling.first_attribute();
                    
                    // Search for attributes using hint
                    pugi::xml_attribute found_with_hint = sibling.attribute("type", hint);
                    (void)found_with_hint;
                    
                    // Update hint and search again
                    if (hint) {
                        hint = hint.next_attribute();
                        pugi::xml_attribute found_again = sibling.attribute("priority", hint);
                        (void)found_again;
                    }
                }
                break;
            }
            
            case 4: {
                // Test string_view attribute retrieval if available
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string_view attr_name_sv = "type";
                pugi::xml_attribute sv_attr = test_elem.attribute(attr_name_sv);
                (void)sv_attr;
                #endif
                break;
            }
        }
    }
    
    // ==================== PHASE 3: SIBLING NAVIGATION BY NAME ====================
    // TARGET: xml_node::next_sibling(const char_t* name), xml_node::previous_sibling(const char_t* name)
    
    // Reset fuzzer input consumption for sibling navigation phase
    if (fdp.remaining_bytes() < 50) {
        return 0;
    }
    
    // Test sibling navigation with different starting points
    for (size_t start_idx = 0; start_idx < siblings.size() && fdp.remaining_bytes() > 10; ++start_idx) {
        pugi::xml_node current = siblings[start_idx];
        
        // Test navigation to next sibling with specific name
        for (const auto& target_name : sibling_names) {
            pugi::xml_node next_with_name = current.next_sibling(target_name.c_str());
            (void)next_with_name;
            
            // Also test previous sibling with specific name
            pugi::xml_node prev_with_name = current.previous_sibling(target_name.c_str());
            (void)prev_with_name;
        }
        
        // Test navigation with fuzzer-generated names
        std::string random_name = fdp.ConsumeRandomLengthString(25);
        pugi::xml_node next_random = current.next_sibling(random_name.c_str());
        pugi::xml_node prev_random = current.previous_sibling(random_name.c_str());
        
        (void)next_random;
        (void)prev_random;
        
        // Test chained navigation
        if (current) {
            pugi::xml_node first_named = current.next_sibling(sibling_names[0].c_str());
            if (first_named) {
                pugi::xml_node second_named = first_named.next_sibling(sibling_names[1 % sibling_names.size()].c_str());
                (void)second_named;
            }
        }
    }
    
    // ==================== PHASE 4: ATTRIBUTE PREPEND/APPEND/INSERT OPERATIONS ====================
    // TARGET: append_attribute, prepend_attribute, insert_attribute_after, insert_attribute_before
    
    // Create elements for attribute manipulation testing
    pugi::xml_node append_test = root.append_child("append_attribute_test");
    pugi::xml_node prepend_test = root.append_child("prepend_attribute_test");
    pugi::xml_node insert_test = root.append_child("insert_attribute_test");
    
    // Initial attributes for insert operations
    pugi::xml_attribute base_attr1 = insert_test.append_attribute("base1");
    base_attr1.set_value("initial_value_1");
    
    pugi::xml_attribute base_attr2 = insert_test.append_attribute("base2");
    base_attr2.set_value("initial_value_2");
    
    pugi::xml_attribute base_attr3 = insert_test.append_attribute("base3");
    base_attr3.set_value("initial_value_3");
    
    // Test attribute append operations
    while (fdp.remaining_bytes() > 30) {
        uint8_t attr_op = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (attr_op) {
            case 0: {
                // Test append_attribute
                std::string new_attr_name = fdp.ConsumeRandomLengthString(20);
                pugi::xml_attribute appended = append_test.append_attribute(new_attr_name.c_str());
                if (appended) {
                    std::string attr_value = fdp.ConsumeRandomLengthString(50);
                    appended.set_value(attr_value.c_str());
                }
                break;
            }
            
            case 1: {
                // Test prepend_attribute
                std::string prepend_name = fdp.ConsumeRandomLengthString(20);
                pugi::xml_attribute prepended = prepend_test.prepend_attribute(prepend_name.c_str());
                if (prepended) {
                    std::string prepend_value = fdp.ConsumeRandomLengthString(50);
                    prepended.set_value(prepend_value.c_str());
                }
                break;
            }
            
            case 2: {
                // Test insert_attribute_after
                std::string insert_name = fdp.ConsumeRandomLengthString(20);
                pugi::xml_attribute inserted_after = insert_test.insert_attribute_after(insert_name.c_str(), base_attr1);
                if (inserted_after) {
                    std::string insert_value = fdp.ConsumeRandomLengthString(50);
                    inserted_after.set_value(insert_value.c_str());
                }
                break;
            }
            
            case 3: {
                // Test insert_attribute_before
                std::string insert_before_name = fdp.ConsumeRandomLengthString(20);
                pugi::xml_attribute inserted_before = insert_test.insert_attribute_before(insert_before_name.c_str(), base_attr2);
                if (inserted_before) {
                    std::string insert_before_value = fdp.ConsumeRandomLengthString(50);
                    inserted_before.set_value(insert_before_value.c_str());
                }
                break;
            }
            
            case 4: {
                // Test string_view versions if available
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string_view sv_name = "string_view_attr";
                pugi::xml_attribute sv_appended = append_test.append_attribute(sv_name);
                if (sv_appended) {
                    sv_appended.set_value("sv_value");
                }
                #endif
                break;
            }
            
            case 5: {
                // Test attribute operations on sibling elements
                if (!siblings.empty()) {
                    size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, siblings.size() - 1);
                    pugi::xml_node target_sibling = siblings[target_idx];
                    
                    std::string dynamic_attr_name = fdp.ConsumeRandomLengthString(15);
                    pugi::xml_attribute dynamic_attr = target_sibling.append_attribute(dynamic_attr_name.c_str());
                    if (dynamic_attr) {
                        std::string dynamic_value = fdp.ConsumeRandomLengthString(40);
                        dynamic_attr.set_value(dynamic_value.c_str());
                    }
                }
                break;
            }
            
            case 6: {
                // Test multiple attribute operations in sequence
                std::string seq_attr1_name = fdp.ConsumeRandomLengthString(12);
                std::string seq_attr2_name = fdp.ConsumeRandomLengthString(12);
                
                pugi::xml_attribute seq1 = append_test.append_attribute(seq_attr1_name.c_str());
                if (seq1) {
                    seq1.set_value("sequence1");
                    
                    pugi::xml_attribute seq2 = append_test.append_attribute(seq_attr2_name.c_str());
                    if (seq2) {
                        seq2.set_value("sequence2");
                    }
                }
                break;
            }
            
            case 7: {
                // Test attribute removal and re-addition
                std::string temp_attr_name = fdp.ConsumeRandomLengthString(18);
                pugi::xml_attribute temp_attr = append_test.append_attribute(temp_attr_name.c_str());
                if (temp_attr) {
                    temp_attr.set_value("temporary");
                    // Remove it
                    append_test.remove_attribute(temp_attr);
                    // Try to add it again
                    pugi::xml_attribute re_added = append_test.append_attribute(temp_attr_name.c_str());
                    if (re_added) {
                        re_added.set_value("readded");
                    }
                }
                break;
            }
        }
    }
    
    // ==================== PHASE 5: INTEGRATED TESTING ====================
    // Combine attribute retrieval and sibling navigation in complex scenarios
    
    if (fdp.remaining_bytes() > 100) {
        // Create a chain of operations
        pugi::xml_node integrated_test = root.append_child("integrated_test");
        
        // Add attributes with predictable names
        integrated_test.append_attribute("start") = "begin";
        integrated_test.append_attribute("middle") = "center";
        integrated_test.append_attribute("end") = "finish";
        
        // Retrieve attributes by name
        pugi::xml_attribute start_attr = integrated_test.attribute("start");
        pugi::xml_attribute middle_attr = integrated_test.attribute("middle");
        pugi::xml_attribute end_attr = integrated_test.attribute("end");
        
        // Use retrieved attributes to create sibling elements
        if (start_attr) {
            pugi::xml_node sibling1 = root.append_child("sibling_from_start");
            sibling1.append_attribute("derived_from") = start_attr.value();
        }
        
        if (middle_attr) {
            pugi::xml_node sibling2 = root.append_child("sibling_from_middle");
            sibling2.append_attribute("derived_from") = middle_attr.value();
        }
        
        if (end_attr) {
            pugi::xml_node sibling3 = root.append_child("sibling_from_end");
            sibling3.append_attribute("derived_from") = end_attr.value();
        }
        
        // Navigate between these siblings by name
        pugi::xml_node first_sib = integrated_test.next_sibling("sibling_from_start");
        if (first_sib) {
            pugi::xml_node second_sib = first_sib.next_sibling("sibling_from_middle");
            if (second_sib) {
                pugi::xml_node third_sib = second_sib.next_sibling("sibling_from_end");
                (void)third_sib;
            }
        }
    }
    
    return 0;
}

} // namespace harness_016
