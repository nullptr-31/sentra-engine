//
// Created by Abdelbaki Boukerche on 18/7/2026.
//

#include "Sentra/Network/InterfaceManager.h"

#include "PcapLiveDeviceList.h"

namespace SCore {
    std::vector<pcpp::PcapLiveDevice *> InterfaceManager::GetSystemInterfaces() const {
        return pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    }

    pcpp::PcapLiveDevice* InterfaceManager::GetSystemInterface(const std::string& interfaceName) const {
        return pcpp::PcapLiveDeviceList::getInstance().getDeviceByName(interfaceName);
    }

    bool InterfaceManager::OpenInterface(const std::string &interfaceName) const {
        pcpp::PcapLiveDevice *interface = GetSystemInterface(interfaceName);

        return OpenInterface(interface);
    }

    bool InterfaceManager::OpenInterface(pcpp::PcapLiveDevice *interface) const {
        if (interface == nullptr) return false;

        if (interface->isOpened()) return true;

        if (!interface->open()) return false;

        return true;
    }

    void InterfaceManager::CloseInterface(const std::string &interfaceName) const {
        pcpp::PcapLiveDevice *interface = GetSystemInterface(interfaceName);

        CloseInterface(interface);
    }

    void InterfaceManager::CloseInterface(pcpp::PcapLiveDevice *interface) const {
        if (interface == nullptr) return;

        if (interface->isOpened()) {
            interface->close();
        }
    }
} // SCore