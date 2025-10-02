#include "FileLoader.hpp"
#include <iostream>
#include <filesystem>
#include <unordered_map>

FileLoader::FileLoader() : loaded(false) {}
FileLoader::FileLoader(const std::string& xmlFilePath) : loaded(LoadXML(xmlFilePath)) {}
FileLoader::~FileLoader() = default;

bool FileLoader::LoadXML(const std::string& filePath) {
    xmlFilePath = filePath;
    auto result = doc.load_file(filePath.c_str());
    if (result) {
        std::cout << "XML " << filePath << " loaded successfully" << std::endl;
        return true;
    }
    std::cerr << "Error loading XML " << filePath << ": " << result.description() << std::endl;
    return false;
}

bool FileLoader::IsLoaded() const { return loaded; }

pugi::xml_node FileLoader::FindCardNode(int cardId) const {
    if (!loaded) return {};
    for (pugi::xml_node card : doc.child("cards").children("card")) {
        if (card.attribute("id").as_int() == cardId) {
            return card;
        }
    }
    return {};
}

bool FileLoader::CardExists(int cardId) const {
    return FindCardNode(cardId) != nullptr;
}

std::string FileLoader::GetCardPath(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        return card.attribute("path").as_string();
    }
    return "";
}

EventType FileLoader::GetCardEventType(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        static const std::unordered_map<std::string, EventType> eventTypeMap{
            {"CREATE", EventType::CREATE},
            {"MODIFY", EventType::MODIFY},
            {"DELETE", EventType::DELETE}
        };
        std::string eventTypeStr = card.attribute("eventType").as_string();
        if (auto it = eventTypeMap.find(eventTypeStr); it != eventTypeMap.end()) {
            return it->second;
        }
    }
    return EventType::UNKNOWN;
}

std::map<ResourceType, int> FileLoader::GetCardResourcesYes(int cardId) const {
    std::map<ResourceType, int> resources;
    const pugi::xml_node card = FindCardNode(cardId);
    if (!card) return resources;

    // Mapeamento de intensidade string para valores numéricos
    auto intensityToValue = [](const std::string& intensity) -> int {
        if (intensity == "negativehigh") return -3*3;
        if (intensity == "negativemedium") return -2*3;
        if (intensity == "negativelow") return -1*3;
        if (intensity == "none") return 0;
        if (intensity == "low") return 1*3;
        if (intensity == "medium") return 2*3;
        if (intensity == "high") return 3*3;
        return 0; // default para intensidade desconhecida
    };

    // Ler recursos para resposta "Sim"
    for (pugi::xml_node res : card.child("resourcesYes").children("resource")) {
        std::string typeStr = res.attribute("type").as_string();
        std::string intensityStr = res.attribute("intensity").as_string();
        int value = intensityToValue(intensityStr);

        if (typeStr == "Economy") resources[ResourceType::ECONOMY] = value;
        else if (typeStr == "Population Awareness") resources[ResourceType::POPULATION_AWARENESS] = value;
        else if (typeStr == "Waste Collection") resources[ResourceType::WASTE_COLLECTION] = value;
        else if (typeStr == "Waste Accumulation") resources[ResourceType::WASTE_ACCUMULATION] = value;
    }

    return resources;
}

std::map<ResourceType, int> FileLoader::GetCardResourcesNo(int cardId) const {
    std::map<ResourceType, int> resources;
    const pugi::xml_node card = FindCardNode(cardId);
    if (!card) return resources;

    // Mapeamento de intensidade string para valores numéricos
    auto intensityToValue = [](const std::string& intensity) -> int {
        if (intensity == "negativehigh") return -3*3;
        if (intensity == "negativemedium") return -2*3;
        if (intensity == "negativelow") return -1*3;
        if (intensity == "none") return 0;
        if (intensity == "low") return 1*3;
        if (intensity == "medium") return 2*3;
        if (intensity == "high") return 3*3;
        return 0; // default para intensidade desconhecida
    };

    // Ler recursos para resposta "Não"
    for (pugi::xml_node res : card.child("resourcesNo").children("resource")) {
        std::string typeStr = res.attribute("type").as_string();
        std::string intensityStr = res.attribute("intensity").as_string();
        int value = intensityToValue(intensityStr);

        if (typeStr == "Economy") resources[ResourceType::ECONOMY] = value;
        else if (typeStr == "Population Awareness") resources[ResourceType::POPULATION_AWARENESS] = value;
        else if (typeStr == "Waste Collection") resources[ResourceType::WASTE_COLLECTION] = value;
        else if (typeStr == "Waste Accumulation") resources[ResourceType::WASTE_ACCUMULATION] = value;
    }

    return resources;
}

int FileLoader::GetCardQuantity(int cardId) const {
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

bool FileLoader::PathExists(const std::string& path) {
    return std::filesystem::exists(path);
}

std::string FileLoader::GetCardText(int cardId) const {
    if (pugi::xml_node card = FindCardNode(cardId)) {
        return card.child_value("text");
    }
    return "";
}
