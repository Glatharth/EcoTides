#pragma once
#ifndef FILELOADER_HPP
#define FILELOADER_HPP

#include "pugixml.hpp"
#include <string>
#include <vector>
#include <map>
#include "utils/enum.hpp"

class FileLoader {
public:
    FileLoader();
    explicit FileLoader(const std::string& xmlFilePath);
    ~FileLoader();

    bool LoadXML(const std::string& filePath);
    [[nodiscard]] bool IsLoaded() const;
    [[nodiscard]] bool CardExists(int cardId) const;
    [[nodiscard]] std::string GetCardPath(int cardId) const;
    [[nodiscard]] EventType GetCardEventType(int cardId) const;
    std::map<ResourceType, int> GetCardResourcesYes(int cardId) const;
    std::map<ResourceType, int> GetCardResourcesNo(int cardId) const;
    [[nodiscard]] std::string GetCardText(int cardId) const;
    [[nodiscard]] int GetCardQuantity(int cardId) const;
    [[nodiscard]] std::vector<std::pair<int, int>> GetCardParents(int cardId) const;
    static bool PathExists(const std::string& path);

private:
    [[nodiscard]] pugi::xml_node FindCardNode(int cardId) const;
    pugi::xml_document doc;
    std::string xmlFilePath;
    bool loaded;
};

#endif