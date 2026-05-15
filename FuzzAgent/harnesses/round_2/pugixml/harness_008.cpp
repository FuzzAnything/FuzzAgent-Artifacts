#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <string_view>

namespace harness_008 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML document from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Use load_buffer for parsing
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(16, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    parse_result = doc.load_buffer(
        xml_buffer.data(), 
        xml_buffer.size(), 
        pugi::parse_default, 
        pugi::encoding_auto
    );
    
    // If parsing failed, create a simple document for testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name("root");
        
        // Add some initial structure
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
            pugi::xml_node child = root.append_child(pugi::node_element);
            child.set_name("initial_child");
            child.append_child(pugi::node_pcdata).set_value("initial_text");
        }
    }
    
    // Get the root node
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        // Create root if document is empty
        root = doc.append_child(pugi::node_element);
        root.set_name("root");
    }
    
    // Step 2: Store std::string objects from fuzzer data segments and create string_view objects
    // We store std::string objects to ensure proper lifetime for string_view objects
    
    // Check remaining bytes for string operations - we need enough for all strings
    // Approximate: 50+50+50+30+30+100+100+200+200+150 = ~960 bytes minimum
    if (fdp.remaining_bytes() < 1000) {
        return 0; // Not enough data for string operations
    }
    
    // Store std::string objects to ensure proper lifetime
    std::string node_name1_str = fdp.ConsumeRandomLengthString(50);
    std::string node_name2_str = fdp.ConsumeRandomLengthString(50);
    std::string node_name3_str = fdp.ConsumeRandomLengthString(50);
    std::string attr_name1_str = fdp.ConsumeRandomLengthString(30);
    std::string attr_name2_str = fdp.ConsumeRandomLengthString(30);
    std::string attr_value1_str = fdp.ConsumeRandomLengthString(100);
    std::string attr_value2_str = fdp.ConsumeRandomLengthString(100);
    std::string node_value1_str = fdp.ConsumeRandomLengthString(200);
    std::string node_value2_str = fdp.ConsumeRandomLengthString(200);
    std::string text_value_str = fdp.ConsumeRandomLengthString(150);
    
    // Create string_view objects from stored strings (valid for lifetime of std::string objects)
    std::string_view node_name1_sv = node_name1_str;
    std::string_view node_name2_sv = node_name2_str;
    std::string_view node_name3_sv = node_name3_str;
    std::string_view attr_name1_sv = attr_name1_str;
    std::string_view attr_name2_sv = attr_name2_str;
    std::string_view attr_value1_sv = attr_value1_str;
    std::string_view attr_value2_sv = attr_value2_str;
    std::string_view node_value1_sv = node_value1_str;
    std::string_view node_value2_sv = node_value2_str;
    std::string_view text_value_sv = text_value_str;
    
    // Step 3: Use string_view versions for tree manipulation
    
    // Primary Targets: xml_node string_view APIs
    
    // 1. append_child with string_view
    pugi::xml_node appended_child;
#ifdef PUGIXML_HAS_STRING_VIEW
    appended_child = root.append_child(node_name1_sv);
#else
    // Fallback to char* version if string_view not available
    appended_child = root.append_child(std::string(node_name1_sv).c_str());
#endif
    
    // 2. prepend_child with string_view
    pugi::xml_node prepended_child;
#ifdef PUGIXML_HAS_STRING_VIEW
    prepended_child = root.prepend_child(node_name2_sv);
#else
    prepended_child = root.prepend_child(std::string(node_name2_sv).c_str());
#endif
    
    // 3. insert_child_after with string_view
    pugi::xml_node inserted_child_after;
    if (!appended_child.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        inserted_child_after = root.insert_child_after(node_name3_sv, appended_child);
#else
        inserted_child_after = root.insert_child_after(std::string(node_name3_sv).c_str(), appended_child);
#endif
    }
    
    // 4. insert_child_before with string_view
    pugi::xml_node inserted_child_before;
    if (!prepended_child.empty() && !inserted_child_after.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        inserted_child_before = root.insert_child_before(node_name1_sv, inserted_child_after);
#else
        inserted_child_before = root.insert_child_before(std::string(node_name1_sv).c_str(), inserted_child_after);
#endif
    }
    
    // 5. append_attribute with string_view
    pugi::xml_attribute appended_attr;
    if (!appended_child.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        appended_attr = appended_child.append_attribute(attr_name1_sv);
#else
        appended_attr = appended_child.append_attribute(std::string(attr_name1_sv).c_str());
#endif
    }
    
    // 6. prepend_attribute with string_view
    pugi::xml_attribute prepended_attr;
    if (!prepended_child.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        prepended_attr = prepended_child.prepend_attribute(attr_name2_sv);
#else
        prepended_attr = prepended_child.prepend_attribute(std::string(attr_name2_sv).c_str());
#endif
    }
    
    // 7. set_name with string_view
    if (!appended_child.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        appended_child.set_name(node_name2_sv);
#else
        appended_child.set_name(std::string(node_name2_sv).c_str());
#endif
    }
    
    // 8. set_value with string_view for nodes
    if (!appended_child.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        appended_child.set_value(node_value1_sv);
#else
        appended_child.set_value(std::string(node_value1_sv).c_str());
#endif
    }
    
    // Secondary Targets: xml_attribute string_view APIs
    
    // 9. xml_attribute::set_value with string_view
    if (!appended_attr.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        appended_attr.set_value(attr_value1_sv);
#else
        appended_attr.set_value(std::string(attr_value1_sv).c_str());
#endif
    }
    
    // 10. xml_attribute::operator= with string_view
    if (!prepended_attr.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        prepended_attr = attr_value2_sv;
#else
        prepended_attr = std::string(attr_value2_sv).c_str();
#endif
    }
    
    // Secondary Targets: xml_text string_view APIs
    
    // Create a text node for xml_text testing
    pugi::xml_node text_node;
    if (!inserted_child_after.empty()) {
        text_node = inserted_child_after.append_child(pugi::node_pcdata);
        
        // Get xml_text object
        pugi::xml_text text_obj = text_node.text();
        
        // 11. xml_text::set with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
        text_obj.set(text_value_sv);
#else
        text_obj.set(std::string(text_value_sv).c_str());
#endif
        
        // 12. xml_text::operator= with string_view
        pugi::xml_text text_obj2 = text_node.text();
#ifdef PUGIXML_HAS_STRING_VIEW
        text_obj2 = node_value2_sv;
#else
        text_obj2 = std::string(node_value2_sv).c_str();
#endif
    }
    
    // Step 4: Validate operations
    
    // Validate node operations
    if (!appended_child.empty()) {
        const char* name = appended_child.name();
        const char* value = appended_child.value();
        (void)name;
        (void)value;
    }
    
    if (!prepended_child.empty()) {
        const char* name = prepended_child.name();
        const char* value = prepended_child.value();
        (void)name;
        (void)value;
    }
    
    // Validate attribute operations
    if (!appended_attr.empty()) {
        const char* attr_name = appended_attr.name();
        const char* attr_value = appended_attr.value();
        (void)attr_name;
        (void)attr_value;
    }
    
    if (!prepended_attr.empty()) {
        const char* attr_name = prepended_attr.name();
        const char* attr_value = prepended_attr.value();
        (void)attr_name;
        (void)attr_value;
    }
    
    // Validate tree structure by traversing
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
        pugi::xml_node_type type = node.type();
        const char* name = node.name();
        const char* value = node.value();
        (void)type;
        (void)name;
        (void)value;
        
        // Traverse attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
            (void)attr_name;
            (void)attr_value;
        }
    }
    
    // Additional string_view operations with remaining bytes
    
    // Test remove_attribute with string_view
    if (!appended_child.empty() && !appended_attr.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        appended_child.remove_attribute(attr_name1_sv);
#else
        appended_child.remove_attribute(std::string(attr_name1_sv).c_str());
#endif
    }
    
    // Test remove_child with string_view
    if (!root.empty() && !prepended_child.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        root.remove_child(node_name2_sv);
#else
        root.remove_child(std::string(node_name2_sv).c_str());
#endif
    }
    
    // Test find operations with string_view
    if (!root.empty()) {
#ifdef PUGIXML_HAS_STRING_VIEW
        pugi::xml_node found_child = root.child(node_name1_sv);
        pugi::xml_attribute found_attr = root.attribute(attr_name2_sv);
        (void)found_child;
        (void)found_attr;
#else
        pugi::xml_node found_child = root.child(std::string(node_name1_sv).c_str());
        pugi::xml_attribute found_attr = root.attribute(std::string(attr_name2_sv).c_str());
        (void)found_child;
        (void)found_attr;
#endif
    }
    
    // Step 5: Clean up
    doc.reset();
    
    return 0;
}

} // namespace harness_008
