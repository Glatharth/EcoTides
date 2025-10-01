#pragma once
#include <map>
#include <string>
#include <pugixml.hpp>
#include <vector>

#include "utils/enum.hpp"

class FileLoader {
public:
    FileLoader();
    explicit FileLoader(const std::string& xmlFilePath);
    ~FileLoader();

    bool IsLoaded() const;
    std::string GetCardPath(int cardId) const;
    EventType GetCardEventType(int cardId) const;
    std::map<ResourceType, int> GetCardResources(int cardId) const;
    int GetCardResourceType(int cardId, ResourceType resource) const;
    int GetCardQuantity(int cardId) const;
    std::vector<std::pair<int, int>> GetCardParents(int cardId) const;
    bool CardExists(int cardId) const;
    static bool PathExists(const std::string& path);
    void LoadAllCards() const;
private:
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    bool loaded = false;
    std::string xmlFilePath;

    pugi::xml_node FindCardNode(int cardId) const;
    bool LoadXML(const std::string& filePath);
};