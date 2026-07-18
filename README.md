<img src="assets/sentra-engine.png" alt="sentra-engine logo" style="display: block; margin: 0 auto; width: 512px;">

**Sentra Engine** is a high-performance C++ packet-processing core of the Sentra intrusion detection platform.

It captures network traffic in real time, reconstructs bidirectional flows, extracts traffic features, executes detection modules, and forwards completed flow records to other Sentra components for storage, visualization, and AI-assisted classification.

> Sentra Engine is currently under active development and should be considered experimental.

---

## Overview
**Sentra Engine** is responsible for the low-level networking and runtime operations of the Sentra platform.

Its main responsibilities include:
- Capturing packets from one or more network interfaces
- Parsing Ethernet, IPv4, IPv6, TCP, UDP, ICMP, and other protocols
- Grouping packets into bidirectional network flows
- Tracking active flows and detecting flow expiration
- Extracting statistical and behavioral traffic features
- Executing pluggable detection modules
- Providing an interactive command-line interface
- Sending completed flows to Sentra Cortex
- Reporting runtime statistics, packet drops, and processing errors

The engine is designed to remain independent of the AI, backend, and dashboard components while exposing clear interfaces for integration.

---

## Sentra Platform
Sentra consists of four main components:

| Component     | Responsibility                                                                      |
|---------------|-------------------------------------------------------------------------------------|
| Sentra Engine | Packet capture, flow construction, feature extraction, CLI, and detection modules   |
| Sentra Cortex | AI-based network flow classification (Binary and Multiclass Classification)         |
| Sentra Nexus  | FastAPI backend, communication, storage, and platform coordination                  |
| Sentra Vision | Next.js dashboard for monitoring traffic, alerts, sensors, and system configuration |

---
## Features

### Real-Time Packet Capture
Sentra Engine captures live packets from selected network interfaces using a packet capture backend such as PcapPlusPlus or libpcap.

The capture layer is intended to support:
- Multiple network interfaces
- Promiscuous mode
- Packet capture statistics
- Configurable capture sessions
- Graceful session startup and shutdown

### Bidirectional Flow Construction
Packets are grouped into bidirectional flows using normalized flow keys.

A flow is typically identified using:
```
Source IP
Destination IP
Source port
Destination port
Transport protocol
```
Forward and backward traffic are stored in the same flow, regardless of which endpoint initiated the latest packet.

Example:
```
192.168.1.10:53000 -> 8.8.8.8:53 UDP
8.8.8.8:53 -> 192.168.1.10:53000 UDP
```
Both packet directions belong to the same bidirectional flow.

### Feature Extraction
Sentra Engine extracts flow-level features that can be consumed by rule-based modules or machine-learning models.

Planned and supported features include:
- Flow duration
- Transport protocol (only TCP for now)
- Source and destination ports
- Total packet count
- Total byte count
- Forward packet count
- Backward packet count
- Forward byte count
- Backward byte count
- Packets per second
- Bytes per second
- Minimum packet size
- Maximum packet size
- Mean packet size
- Packet-size variance
- Packet-size standard deviation
- Inter-arrival time minimum
- Inter-arrival time maximum
- Inter-arrival time mean
- Inter-arrival time variance
- TCP flag counters
- Flow start timestamp
- Flow end timestamp
- Flow termination reason

### Detection Modules
Sentra Engine supports modular detection logic that can inspect packets, flows, or protocol events.

Detection modules may be used for:
- Signature-based detection
- Protocol anomaly detection
- Threshold-based detection
- Traffic policy enforcement
- Custom experimental detectors
- Layer 2 and Layer 3 attack detection

Planned module targets include:
- ARP poisoning
- VLAN hopping
- Spanning Tree Protocol attacks
- Route poisoning
- TCP scanning
- Denial-of-service patterns
- Suspicious DNS behavior

The module system is intended to support dynamically loaded modules, including modules written in Lua.

### Interactive CLI
Sentra Engine includes an interactive command-line interface inspired by Cisco IOS.

Example session:
```
sentra> show version
Sentra Intrusion Detection System Software
Sentra Engine, Version 0.1.0 Experimental
Compiled Jul 18 2026 18:30:00
```

```
sentra> show interfaces
Index  Name  State  Capture
1      en0   up     stopped
2      lo0   up     stopped
```
```
sentra> interface en0
sentra-if-en0> capture start
Capture started on interface en0
sentra-if-en0> exit
sentra>
```

The CLI is designed to support:

- Command history
- Auto-completion
- Context-sensitive prompts
- Hierarchical commands
- Multiple configuration modes
- Unique-prefix command abbreviations

For example:

`show ip interface brief`

may be abbreviated as:

`sh ip int br`

Abbreviations are resolved dynamically only when the entered prefix identifies a unique command.

---

## Status

Sentra Engine is currently experimental.

Interfaces, configuration formats, command names, module APIs, and internal architecture may change before the first stable release.

The initial target is a functional prototype capable of:
- Capturing live traffic
- Building bidirectional flows
- Extracting network-flow features
- Running detection modules
- Exporting completed flows for AI classification
- Displaying engine state through the CLI

---

## License

This project is licensed under the terms specified in the `LICENSE` file.

---

### Related Repositories
- Sentra Cortex — AI-based traffic classification
- Sentra Nexus — FastAPI backend and platform coordinator
- Sentra Vision — Next.js monitoring dashboard

---

### Disclaimer
Sentra is an educational and experimental cybersecurity project.

It is not currently intended to replace a production-grade intrusion detection or network monitoring system. Use it only on networks and devices that you own or are explicitly authorized to monitor.