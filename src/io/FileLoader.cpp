#include "FileLoader.hpp"
#include <iostream>
#include <filesystem>
#include <unordered_map>

FileLoader::FileLoader() = default;

FileLoader::FileLoader(const std::string& xmlFilePath) {
    loaded = LoadXML(xmlFilePath);
}

FileLoader::~FileLoader() = default;

bool FileLoader::LoadXML(const std::string& filePath) {
    xmlFilePath = filePath;
    result = doc.load_file(filePath.c_str());

    if (result) {
        std::cout << "XML [" << filePath << "] loaded successfully" << std::endl;
        return true;
    }
    std::cerr << "Error loading XML [" << filePath << "]: "
              << result.description() << std::endl;
    return false;
}

bool FileLoader::IsLoaded() const {
    return loaded;
}

pugi::xml_node FileLoader::FindCardNode(int cardId) const {
    if (!loaded) return {};

    // Search for a card with the specified ID
    for (pugi::xml_node card : doc.child("cards").children("card")) {
        if (card.attribute("id").as_int() == cardId) {
            return card;
        }
    }

    return {}; // Return empty node if not found
}

std::string FileLoader::GetCardPath(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("path").as_string();
    }
    return "";
}

EventType FileLoader::GetCardEventType(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        static const std::unordered_map<std::string, EventType> eventTypeMap = {
            {"CREATE", EventType::CREATE},
            {"MODIFY", EventType::MODIFY},
            {"DELETE", EventType::DELETE}
        };

        const std::string eventTypeStr = card.attribute("eventType").as_string();
        if (const auto it = eventTypeMap.find(eventTypeStr); it != eventTypeMap.end()) {
            return it->second;
        }
    }
    return EventType::UNKNOWN;
}

std::map<ResourceType, int> FileLoader::GetCardResources(const int cardId) const {
    std::map<ResourceType, int> resources;
    const pugi::xml_node card = FindCardNode(cardId);
    if (!card) return resources;

    for (pugi::xml_node res : card.children("resource")) {
        std::string typeStr = res.attribute("type").as_string();
        int value = res.attribute("value").as_int();

        if (typeStr == "Economy") resources[ResourceType::ECONOMY] = value;
        else if (typeStr == "Population Awareness") resources[ResourceType::POPULATION_AWARENESS] = value;
        else if (typeStr == "Waste Collection") resources[ResourceType::WASTE_COLLECTION] = value;
        else if (typeStr == "Waste Accumulation") resources[ResourceType::WASTE_ACCUMULATION] = value;
    }
    return resources;
}

int FileLoader::GetCardResourceType(const int cardId, const ResourceType resource) const {
    const auto resources = GetCardResources(cardId);
    if (const auto it = resources.find(resource); it != resources.end()) {
        return it->second;
    }
    return 0;
}

int FileLoader::GetCardQuantity(const int cardId) const {
    if (const pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("quantity").as_int(1);
    }
    return 1;
}

std::vector<std::pair<int, int>> FileLoader::GetCardParents(int cardId) const {
    std::vector<std::pair<int, int>> parents;
    pugi::xml_node card = FindCardNode(cardId);
    if (!card) return parents;
    for (pugi::xml_node parent : card.child("parents").children("parent")) {
        int parentId = parent.attribute("id").as_int();
        int steps = parent.attribute("steps").as_int();
        parents.emplace_back(parentId, steps);
    }
    return parents;
}


bool FileLoader::CardExists(int cardId) const {
    return FindCardNode(cardId).type() != pugi::node_null;
}

bool FileLoader::PathExists(const std::string& path) {
    return std::filesystem::exists(path);
}

void FileLoader::LoadAllCards() const {
    if (!loaded) {
        std::cout << "XML was not loaded correctly" << std::endl;
        return;
    }

    pugi::xml_node cards = doc.child("cards");
    std::cout << "Cards found in XML:" << std::endl;

    for (pugi::xml_node card : cards.children("card")) {
        int id = card.attribute("id").as_int();
        std::string path = card.attribute("path").as_string();
        std::string eventType = card.attribute("eventType").as_string();

        std::cout << "Card ID: " << id
                  << ", Path: " << path
                  << ", EventType: " << eventType
                  << ", Path exists: " << (PathExists(path) ? "Yes" : "No")
                  << std::endl;
    }
}
