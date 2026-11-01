// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;

    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Ensure the input is null-terminated for safe string parsing
    // We create a temporary buffer with a null terminator
    std::string input_str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse as UTF-8
    pugi::xml_parse_result result = doc->load_string(input_str.c_str());
    
    if (!result) {
        // If parsing fails, create a minimal valid document to ensure we have something to test
        delete doc;
        doc = new pugi::xml_document();
        if (!doc) return nullptr;
        
        pugi::xml_node root = doc->append_child("root");
        if (!root) {
            delete doc;
            return nullptr;
        }
        // Add some initial attributes for testing
        root.append_attribute("attr1");
        root.append_attribute("attr2");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) return 0;
    
    pugi::xml_node root = doc->document_element();
    if (!root) {
        // If no root element, try to create one
        root = doc->append_child("root");
        if (!root) {
            delete doc;
            return 0;
        }
        // Add some initial attributes
        root.append_attribute("attr1");
        root.append_attribute("attr2");
    }
    
    // Get some attributes to use as references
    pugi::xml_attribute attr1 = root.attribute("attr1");
    pugi::xml_attribute attr2 = root.attribute("attr2");
    
    // If we don't have enough attributes, create them
    if (!attr1) attr1 = root.append_attribute("attr1");
    if (!attr2) attr2 = root.append_attribute("attr2");
    
    // Create a string view for testing
    std::string_view a_str = "test_attr";
    
    // Test insert_copy_before
    if (attr1 && attr2) {
        pugi::xml_attribute copied_before = root.insert_copy_before(attr1, attr2);
        (void)copied_before; // Suppress unused warning
    }
    
    // Test insert_attribute_before
    if (attr2) {
        pugi::xml_attribute new_before = root.insert_attribute_before("new_attr_before", attr2);
        (void)new_before;
    }
    
    // Test insert_attribute_after
    if (attr1) {
        pugi::xml_attribute new_after = root.insert_attribute_after(a_str, attr1);
        (void)new_after;
    }
    
    // Test attribute lookup
    pugi::xml_attribute found = root.attribute("attr1");
    (void)found;
    
    // Test prepend_attribute
    pugi::xml_attribute prepended = root.prepend_attribute("prepended_attr");
    (void)prepended;
    
    // Test insert_copy_after
    if (attr1 && attr2) {
        pugi::xml_attribute copied_after = root.insert_copy_after(attr1, attr2);
        (void)copied_after;
    }
    
    // Try with different nodes if available
    pugi::xml_node child = root.append_child("child");
    if (child) {
        child.append_attribute("child_attr");
        pugi::xml_attribute child_attr = child.attribute("child_attr");
        
        if (child_attr) {
            child.insert_attribute_before("child_new", child_attr);
            child.insert_attribute_after("child_new2", child_attr);
            child.prepend_attribute("child_prepended");
        }
    }
    
    // Test edge cases with invalid attributes
    pugi::xml_attribute invalid_attr;
    if (attr1) {
        root.insert_copy_before(invalid_attr, attr1);
        root.insert_copy_after(attr1, invalid_attr);
    }
    
    delete doc;
    return 0;
}