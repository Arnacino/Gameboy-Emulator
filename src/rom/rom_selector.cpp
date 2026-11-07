#include "rom_selector.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

namespace {
using RomGroups = std::map<std::string, std::vector<std::filesystem::path>>;

std::filesystem::path findRomRoot() {
    std::error_code error;
    auto directory = std::filesystem::current_path(error);

    while (!error) {
        const auto romDirectory = directory / "roms";
        error.clear();
        if (std::filesystem::is_directory(romDirectory, error)) {
            return romDirectory;
        }

        error.clear();
        const auto parent = directory.parent_path();
        if (parent == directory) {
            break;
        }
        directory = parent;
    }

    return {};
}

RomGroups findRoms(const std::filesystem::path& root) {
    RomGroups groups;
    std::error_code error;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(
             root, std::filesystem::directory_options::skip_permission_denied, error)) {
        if (error) {
            error.clear();
            continue;
        }

        if (!entry.is_regular_file(error)) {
            error.clear();
            continue;
        }

        const auto extension = entry.path().extension().string();
        if (extension != ".gb" && extension != ".GB") {
            continue;
        }

        const auto relativePath = std::filesystem::relative(entry.path(), root, error);
        if (error) {
            error.clear();
            continue;
        }

        const auto folder = relativePath.parent_path().generic_string();
        groups[folder.empty() ? "roms" : folder].push_back(entry.path());
    }

    for (auto& [folder, files] : groups) {
        std::sort(files.begin(), files.end());
    }

    return groups;
}

bool readChoice(std::size_t& choice, std::size_t maximum) {
    if (std::cin >> choice && choice <= maximum) {
        return true;
    }

    if (std::cin.eof()) {
        return false;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid choice. Try again.\n";
    return false;
}
}

std::string RomSelector::selectRom() {
    const auto romRoot = findRomRoot();
    if (romRoot.empty()) {
        std::error_code error;
        std::cout << "Could not find the roms directory from "
                  << std::filesystem::current_path(error).string() << ".\n";
        return {};
    }

    const RomGroups groups = findRoms(romRoot);
    if (groups.empty()) {
        std::cout << "No Game Boy ROMs found in the roms directory.\n";
        return {};
    }

    while (true) {
        std::cout << "\nROM folders:\n";
        std::size_t index = 1;
        for (const auto& [folder, files] : groups) {
            std::cout << index++ << ". " << folder << " (" << files.size() << " ROMs)\n";
        }

        std::cout << "Folder: ";
        std::size_t choice = 0;
        if (!readChoice(choice, groups.size())) {
            if (std::cin.eof()) {
                return {};
            }
            continue;
        }
        if (choice == 0) {
            return {};
        }

        auto group = groups.begin();
        std::advance(group, static_cast<std::ptrdiff_t>(choice - 1));

        while (true) {
            std::cout << "\nROMs in " << group->first << ":\n";
            for (std::size_t romIndex = 0; romIndex < group->second.size(); ++romIndex) {
                std::cout << romIndex + 1 << ". "
                          << group->second[romIndex].filename().string() << '\n';
            }
            std::cout << "0. Back\nROM: ";

            std::size_t romChoice = 0;
            if (!readChoice(romChoice, group->second.size())) {
                if (std::cin.eof()) {
                    return {};
                }
                continue;
            }
            if (romChoice == 0) {
                break;
            }

            return std::filesystem::absolute(group->second[romChoice - 1]).string();
        }
    }
}
