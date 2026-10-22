#include <CLI/CLI.hpp>
#include <iostream>

int main(const int argc, char** argv) {
  CLI::App app{"Network Monitoring and Flow Analysis Engine"};

  app.set_version_flag("--version", "0.0.1");
  app.require_subcommand(1);

  auto* realtime =
      app.add_subcommand("realtime", "Process live network traffic");

  std::string interface_name;
  std::uint16_t dpdk_port_id = 0;

  auto* source = realtime->add_option_group(
      "Capture source", "Select a network interface or DPDK port");

  source->require_option(1, 1);

  source->add_option("-i,--interface", interface_name,
                     "Network interface name");

  source->add_option("--dpdk-port", dpdk_port_id, "DPDK port ID");

  auto* offline =
      app.add_subcommand("offline", "Analyze a saved packet capture");

  std::string input_path;
  offline->add_option("-i,--input", input_path, "Input capture file")
      ->required()
      ->check(CLI::ExistingFile);

  auto* simulation =
      app.add_subcommand("simulation", "Run simulated network traffic");

  CLI11_PARSE(app, argc, argv);

  std::cout << "Hello, World!";

  return 0;
}
