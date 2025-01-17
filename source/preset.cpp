#include "preset.hpp"

#include <3ds.h>
#include <unistd.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <vector>

#include "category.hpp"
#include "settings.hpp"
#include "tinyxml2.h"

namespace fs = std::filesystem;

static std::string_view GetBasePath(OptionCategory category) {
  static constexpr std::array<std::string_view, 2> paths{
    "/3ds/presets/oot3dr/settings/",
    "/3ds/presets/oot3dr/cosmetics/",
  };

  switch(category) {
    case OptionCategory::Setting :
    case OptionCategory::Cosmetic :
      return paths[static_cast<size_t>(category)];
    case OptionCategory::Toggle :
      break;
  }
  return "";
}

//Creates preset directories if they don't exist
bool CreatePresetDirectories() {
  Result res;
  FS_Archive sdmcArchive;

  // Open SD archive
  if (!R_SUCCEEDED(res = FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, "")))) {
    return false;
  }

  //Create the 3ds directory if it doesn't exist
  FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/3ds"), FS_ATTRIBUTE_DIRECTORY);
  //Create the presets directory if it doesn't exist
  FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/3ds/presets"), FS_ATTRIBUTE_DIRECTORY);
  //Create the oot3d directory if it doesn't exist
  FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/3ds/presets/oot3dr"), FS_ATTRIBUTE_DIRECTORY);
  //Create the cosmetics directory if it doesn't exist
  FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/3ds/presets/oot3dr/cosmetics"), FS_ATTRIBUTE_DIRECTORY);
  //Create the settings directory if it doesn't exist
  FSUSER_CreateDirectory(sdmcArchive, fsMakePath(PATH_ASCII, "/3ds/presets/oot3dr/settings"), FS_ATTRIBUTE_DIRECTORY);

  // Close SD archive
  FSUSER_CloseArchive(sdmcArchive);
  return true;
}

//Gets the preset filenames
std::vector<std::string> GetSettingsPresets() {
  std::vector<std::string> presetEntries = {};
  for (const auto& entry : fs::directory_iterator(GetBasePath(OptionCategory::Setting))) {
    if(entry.path().stem().string() != "CACHED_SETTINGS") {
      presetEntries.push_back(entry.path().stem().string());
    }
  }
  return presetEntries;
}

static std::string PresetPath(std::string_view presetName, OptionCategory category) {
  return std::string(GetBasePath(category)).append(presetName).append(".xml");
}

// Presets are now saved as XML files using the tinyxml2 library.
// Documentation: https://leethomason.github.io/tinyxml2/index.html
bool SavePreset(std::string_view presetName, OptionCategory category) {
  using namespace tinyxml2;

  XMLDocument preset = XMLDocument();

  // Create and insert the XML declaration
  preset.InsertEndChild(preset.NewDeclaration());

  // Create the root node
  XMLElement* rootNode = preset.NewElement("settings");
  preset.InsertEndChild(rootNode);

  for (MenuItem* menu : Settings::mainMenu) {
    if (menu->mode != OPTION_SUB_MENU) {
      continue;
    }
    for (const Option* setting : *menu->settingsList) {
      if (!setting->IsCategory(category)) {
        continue;
      }

      // Create the <setting> element
      XMLElement* newSetting = preset.NewElement("setting");
      newSetting->SetAttribute("name", setting->GetName().c_str());
      newSetting->SetText(setting->GetSelectedOptionText().c_str());

      // Append it to the root node
      rootNode->InsertEndChild(newSetting);
    }
  }

  const std::string filepath = PresetPath(presetName, category);
  XMLError e = preset.SaveFile(filepath.c_str());
  if (e != XML_SUCCESS) {
    return false;
  }
  return true;
}

//Read the preset XML file
bool LoadPreset(std::string_view presetName, OptionCategory category) {
  using namespace tinyxml2;

  XMLDocument preset;
  XMLError e = preset.LoadFile(PresetPath(presetName, category).c_str());
  if (e != XML_SUCCESS) {
    return false;
  }

  XMLElement* rootNode = preset.RootElement();
  if (strcmp(rootNode->Name(), "settings") != 0) {
      // We do not have our <settings> root node, so it may be the old structure. We don't support that one anymore.
      return false;
  }

  XMLElement* curNode = rootNode->FirstChildElement();

  for (MenuItem* menu : Settings::mainMenu) {
    if (menu->mode != OPTION_SUB_MENU) {
      continue;
    }

    for (Option* setting : *menu->settingsList) {
      if (!setting->IsCategory(category)) {
        continue;
      }

      // Since presets are saved linearly, we can simply loop through the nodes as
      // we loop through the settings to find most of the matching elements.
      const std::string& settingToFind = setting->GetName();
      if (settingToFind == curNode->Attribute("name")) {
        setting->SetSelectedIndexByString(curNode->GetText());
        curNode = curNode->NextSiblingElement();
      } else {
        // If the current setting and element don't match, then search
        // linearly from the beginning. This will get us back on track if the
        // next setting and element line up with each other*/
        curNode = rootNode->FirstChildElement();
        bool settingFound = false;
        while (curNode != nullptr) {
          if (settingToFind == curNode->Attribute("name")) {
            setting->SetSelectedIndexByString(curNode->GetText());
            curNode = curNode->NextSiblingElement();
            settingFound = true;
            break;
          }
          curNode = curNode->NextSiblingElement();
        }
        //reset to the beginning if the setting wasn't found
        if (!settingFound) {
          curNode = rootNode->FirstChildElement();
        }
      }
    }
  }
  return true;
}

//Delete the selected preset
bool DeletePreset(std::string_view presetName, OptionCategory category) {
  Result res;
  FS_Archive sdmcArchive = 0;

  const std::string filepath = PresetPath(presetName, category);

  // Open SD archive
  if (!R_SUCCEEDED(res = FSUSER_OpenArchive(&sdmcArchive, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, "")))) {
    printf("\x1b[22;5HFailed to load SD Archive.");
    return false;
  }

  FSUSER_DeleteFile(sdmcArchive, fsMakePath(PATH_ASCII, filepath.c_str()));
  return true;
}

//Saves the new preset to a file
bool SaveSpecifiedPreset(std::string_view presetName, OptionCategory category) {
  //don't save if the user cancelled
  if (presetName.empty()) {
    return false;
  }
  return SavePreset(presetName, category);
}

void SaveCachedSettings() {
  SavePreset("CACHED_SETTINGS", OptionCategory::Setting);
}

void LoadCachedSettings() {
  //If cache file exists, load it
  for (const auto& entry : fs::directory_iterator(GetBasePath(OptionCategory::Setting))) {
    if(entry.path().stem().string() == "CACHED_SETTINGS") {
      //File exists, open
      LoadPreset("CACHED_SETTINGS", OptionCategory::Setting);
    }
  }
}

bool SaveCachedCosmetics() {
  return SavePreset("CACHED_COSMETICS", OptionCategory::Cosmetic);
}

bool LoadCachedCosmetics() {
  return LoadPreset("CACHED_COSMETICS", OptionCategory::Cosmetic);
}
