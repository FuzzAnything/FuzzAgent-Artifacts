#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Parse XML from first part of input
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(0, size/2));
    pugi::xml_parse_result result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, try to create a simple XML structure
    pugi::xml_node root;
    if (!result || !doc.document_element()) {
        // Create a simple document with root element
        root = doc.append_child("root");
        
        // Add some initial children and attributes for testing
        pugi::xml_node child1 = root.append_child("child1");
        child1.append_attribute("attr1") = "value1";
        
        pugi::xml_node child2 = root.append_child("child2");
        child2.append_attribute("attr2") = "value2";
        
        pugi::xml_node child3 = root.append_child("child3");
        child3.append_attribute("attr3") = "value3";
    } else {
        root = doc.document_element();
    }
    
    if (!root) {
        return 0;
    }
    
    // Check if string_view is available in this build
#ifdef PUGIXML_HAS_STRING_VIEW
    // Create string_view objects from fuzzer input for testing
    // Consume strings for various operations
    std::string child_name1 = fdp.ConsumeRandomLengthString(50);
    std::string child_name2 = fdp.ConsumeRandomLengthString(50);
    std::string child_name3 = fdp.ConsumeRandomLengthString(50);
    std::string child_name4 = fdp.ConsumeRandomLengthString(50);
    std::string attr_name1 = fdp.ConsumeRandomLengthString(30);
    std::string attr_name2 = fdp.ConsumeRandomLengthString(30);
    std::string attr_name3 = fdp.ConsumeRandomLengthString(30);
    
    // Convert to string_view
    pugi::string_view_t sv_child1(child_name1.data(), child_name1.size());
    pugi::string_view_t sv_child2(child_name2.data(), child_name2.size());
    pugi::string_view_t sv_child3(child_name3.data(), child_name3.size());
    pugi::string_view_t sv_child4(child_name4.data(), child_name4.size());
    pugi::string_view_t sv_attr1(attr_name1.data(), attr_name1.size());
    pugi::string_view_t sv_attr2(attr_name2.data(), attr_name2.size());
    pugi::string_view_t sv_attr3(attr_name3.data(), attr_name3.size());
    
    // Target 1: xml_node::append_child(basic_string_view) - 19 branches
    pugi::xml_node appended_child = root.append_child(sv_child1);
    
    // Target 2: xml_node::prepend_child(basic_string_view) - 19 branches
    pugi::xml_node prepended_child = root.prepend_child(sv_child2);
    
    // Need sibling nodes for insert operations
    pugi::xml_node sibling1 = root.append_child("sibling1");
    pugi::xml_node sibling2 = root.append_child("sibling2");
    
    // Target 3: xml_node::insert_child_after(basic_string_view, xml_node) - 21 branches
    pugi::xml_node inserted_after = root.insert_child_after(sv_child3, sibling1);
    
    // Target 4: xml_node::insert_child_before(basic_string_view, xml_node) - 21 branches
    pugi::xml_node inserted_before = root.insert_child_before(sv_child4, sibling2);
    
    // Add some attributes for removal tests
    pugi::xml_attribute attr1 = root.append_attribute("test_attr1");
    attr1.set_value("test_value1");
    
    pugi::xml_attribute attr2 = root.append_attribute("test_attr2");
    attr2.set_value("test_value2");
    
    // Target 5: xml_node::remove_attribute(basic_string_view) - 16 branches
    // Create string_view for existing attribute name
    pugi::string_view_t sv_existing_attr("test_attr1", 10);
    bool removed_attr = root.remove_attribute(sv_existing_attr);
    
    // Target 6: xml_node::remove_child(basic_string_view) - 11 branches
    // Create a child to remove
    pugi::xml_node child_to_remove = root.append_child("child_to_remove");
    pugi::string_view_t sv_child_to_remove("child_to_remove", 15);
    bool removed_child = root.remove_child(sv_child_to_remove);
    
    // Target 7: xml_node::attribute(basic_string_view, xml_attribute&) - 24 branches
    pugi::xml_attribute found_attr;
    pugi::string_view_t sv_find_attr("test_attr2", 10);
    pugi::xml_attribute attr_result = root.attribute(sv_find_attr, found_attr);
    
    // Create attributes for insert operations
    pugi::xml_attribute base_attr1 = root.append_attribute("base_attr1");
    base_attr1.set_value("base_value1");
    
    pugi::xml_attribute base_attr2 = root.append_attribute("base_attr2");
    base_attr2.set_value("base_value2");
    
    // Target 8: xml_node::insert_attribute_before(basic_string_view, xml_attribute) - 13 branches
    pugi::xml_attribute inserted_attr_before = root.insert_attribute_before(sv_attr1, base_attr1);
    
    // Target 9: xml_node::insert_attribute_after(basic_string_view, xml_attribute) - 13 branches
    pugi::xml_attribute inserted_attr_after = root.insert_attribute_after(sv_attr2, base_attr2);
    
    // Also test other string_view APIs for completeness
    // Test xml_attribute::operator=(string_view_t)
    if (inserted_attr_before) {
        pugi::string_view_t sv_new_value("new_value", 9);
        inserted_attr_before = sv_new_value;
    }
    
    // Test xml_node::set_name(string_view_t)
    if (appended_child) {
        pugi::string_view_t sv_new_name("renamed_child", 13);
        appended_child.set_name(sv_new_name);
    }
    
    // Test xml_node::set_value(string_view_t)
    if (prepended_child) {
        pugi::string_view_t sv_new_value("new_node_value", 14);
        prepended_child.set_value(sv_new_value);
    }
    
    // Test xml_node::child(string_view_t)
    pugi::string_view_t sv_find_child("sibling1", 8);
    pugi::xml_node found_child = root.child(sv_find_child);
    
    // Test xml_node::next_sibling(string_view_t)
    if (found_child) {
        pugi::xml_node next_sib = found_child.next_sibling(sv_find_child);
    }
    
    // Test xml_node::previous_sibling(string_view_t)
    if (found_child) {
        pugi::xml_node prev_sib = found_child.previous_sibling(sv_find_child);
    }
    
    // Test xml_attribute::set_name(string_view_t)
    if (attr2) {
        pugi::string_view_t sv_new_attr_name("renamed_attr", 12);
        attr2.set_name(sv_new_attr_name);
    }
    
    // Test xml_attribute::set_value(string_view_t)
    if (attr2) {
        pugi::string_view_t sv_new_attr_value("new_attr_value", 14);
        attr2.set_value(sv_new_attr_value);
    }
    
    // Test xml_text::operator=(string_view_t)
    pugi::xml_node text_node = root.append_child("text_node");
    pugi::xml_text node_text = text_node.text();
    pugi::string_view_t sv_text_content("text content here", 17);
    node_text = sv_text_content;
    
    // Validate the tree structure after all operations
    // Simple traversal to ensure tree is still valid
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        const char* name = child.name();
        const char* value = child.value();
        
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
        }
    }
    
#endif // PUGIXML_HAS_STRING_VIEW
    
    // Cleanup happens automatically via RAII
    return 0;
}

} // namespace harness_005
