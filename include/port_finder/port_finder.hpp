#ifndef PORT_FINDER_HPP_
#define PORT_FINDER_HPP_

/**
 * @file port_finder.hpp
 * @brief Header file for the PortFinder class.
 * 
 * This class is responsible for finding ports via udevadm commands.
 */

#include <iostream>
#include <string>
#include <cstring>
#include <regex>

#define COMMAND_BUFFER_SIZE 128
#define SERIAL_PORT_PATTERN "\\n"
#define SERIAL_PORT_MONITORING_PATTERN "="

namespace port_finder
{
  class PortFinder
  {
    private:
      std::string commandExecute(const char* command);

      std::string findIDVendor(const std::string& serial_port);
      
      std::string findIDVendorID(const std::string& serial_port);
      
      std::string findIDModelID(const std::string& serial_port);
      
      std::string findIDPath(const std::string& serial_port);
      
      void listAllPorts(const std::string& serial_port_group);

      std::regex serial_port_pattern_;

      std::regex serial_port_matches_;

      std::regex serial_port_monitoring_pattern_;

      std::regex serial_port_monitoring_matches_;

      std::string::const_iterator search_start_;

      std::string::const_iterator search_end_;

      std::vector<std::string> serial_ports_;

      std::string find_device_name_command_;

      std::vector<std::string> all_ports_;

      std::string id_vendor_;
      
      std::string id_vendor_id_;

      std::string id_model_id_;

      std::string id_path_;

      std::string device_name_;

      char command_buffer_[COMMAND_BUFFER_SIZE];

    protected:

    public:
      PortFinder();

      ~PortFinder();

      const std::string findPort(const std::string& serial_port_group, 
        std::string id_vendor, std::string id_vendor_id, std::string id_model_id,
        std::string id_model);
  };
} // namespace port_finder

#endif // PORT_FINDER_HPP_