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

        inline static InterfaceManager& Instance() { return s_Instance; }

        std::vector<pcpp::PcapLiveDevice*> GetSystemInterfaces() const;

        pcpp::PcapLiveDevice* GetSystemInterface(const std::string& interfaceName) const;

        bool OpenInterface(const std::string &interfaceName);
        bool OpenInterface(pcpp::PcapLiveDevice *interface);

        void CloseInterface(const std::string &interfaceName);
        void CloseInterface(pcpp::PcapLiveDevice *interface);

    private:
        static InterfaceManager s_Instance;
    };
} // SCore

#endif //SENTRA_ENGINE_INTERFACEMANAGER_H
