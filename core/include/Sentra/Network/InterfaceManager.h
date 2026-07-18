//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#ifndef SENTRA_ENGINE_INTERFACEMANAGER_H
#define SENTRA_ENGINE_INTERFACEMANAGER_H

#include <vector>

#include "PcapLiveDevice.h"

namespace SCore {
    class InterfaceManager {
    private:
        InterfaceManager() = default;
    public:
        InterfaceManager(const InterfaceManager&) = delete;
        InterfaceManager(InterfaceManager &&) = delete;

        InterfaceManager &operator=(const InterfaceManager &) = delete;
        InterfaceManager &operator=(InterfaceManager &&) = delete;

        inline static InterfaceManager& Instance() {
            static InterfaceManager s_Instance;

            return s_Instance;
        }

        std::vector<pcpp::PcapLiveDevice*> GetSystemInterfaces() const;

        pcpp::PcapLiveDevice* GetSystemInterface(const std::string& interfaceName) const;

        bool OpenInterface(const std::string &interfaceName) const;
        bool OpenInterface(pcpp::PcapLiveDevice *interface) const;

        void CloseInterface(const std::string &interfaceName) const;
        void CloseInterface(pcpp::PcapLiveDevice *interface) const;
    };
} // SCore

#endif //SENTRA_ENGINE_INTERFACEMANAGER_H
