#include "port_finder/port_finder.hpp"

port_finder::PortFinder::PortFinder() {
  serial_port_pattern_ = std::regex(SERIAL_PORT_PATTERN);
  serial_port_monitoring_pattern_ = std::regex(SERIAL_PORT_MONITORING_PATTERN);
}

port_finder::PortFinder::~PortFinder() {
  
}

std::string port_finder::PortFinder::commandExecute(const char* command) {
  std::array<char, COMMAND_BUFFER_SIZE> buffer{};
  std::ostringstream result;

  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed! Please check your command.");
  }

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result << buffer.data();
  }

  return result.str();
}

std::string port_finder::PortFinder::findIDVendor(const std::string& serial_port) {
  const std::string& command = "udevadm info --name " + serial_port + 
    " | grep -i ID_VENDOR=";
  std::string result = commandExecute(command.c_str()).c_str();

  if (result.empty()) {
    throw std::runtime_error("Failed to find ID_VENDOR for " + serial_port);
    return "";
  } 

  std::sregex_token_iterator iterator{result.begin(), result.end(), 
    serial_port_monitoring_pattern_, -1};
  std::vector<std::string> matches{iterator, {}};
  result = matches[1];
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());  
  return result;
}

std::string port_finder::PortFinder::findIDVendorID(const std::string& serial_port) {
  const std::string& command = "udevadm info --name " + serial_port + 
    " | grep -i ID_VENDOR_ID=";
  std::string result = commandExecute(command.c_str()).c_str();
  if (result.empty()) {
    throw std::runtime_error("Failed to find ID_VENDOR_ID for " + serial_port);
    return "";
  }

  std::sregex_token_iterator iterator{result.begin(), result.end(), 
    serial_port_monitoring_pattern_, -1};
  std::vector<std::string> matches{iterator, {}};
  result = matches[1];
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  return result;
}

std::string port_finder::PortFinder::findIDModelID(const std::string& serial_port) {
  const std::string& command = "udevadm info --name " + serial_port + 
    " | grep -i ID_MODEL_ID=";
  std::string result = commandExecute(command.c_str()).c_str();
  if (result.empty()) {
    throw std::runtime_error("Failed to find ID_MODEL_ID for " + serial_port);
    return "";
  }

  std::sregex_token_iterator iterator{result.begin(), result.end(), 
    serial_port_monitoring_pattern_, -1};
  std::vector<std::string> matches{iterator, {}};
  result = matches[1];
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  return result;
}

std::string port_finder::PortFinder::findIDPath(const std::string& serial_port) {
  const std::string& command = "udevadm info --name " + serial_port + 
    " | grep -i ID_PATH=";
  std::string result = commandExecute(command.c_str()).c_str();
  if (result.empty()) {
    throw std::runtime_error("Failed to find ID_PATH for " + serial_port);
    return "";
  }
  std::sregex_token_iterator iterator{result.begin(), result.end(), 
    serial_port_monitoring_pattern_, -1};
  std::vector<std::string> matches{iterator, {}};
  result = matches[1];
  result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
  return result;
}

void port_finder::PortFinder::listAllPorts(const std::string& serial_port_group) {
  const std::string& command = "ls -p " + serial_port_group + " | grep -v /";
  const std::string& result = commandExecute(command.c_str()).c_str();
  if (result.empty()) {
    throw std::runtime_error("Failed to list all ports in " + serial_port_group);
  }

  std::sregex_token_iterator iterator_start{result.begin(), result.end(), 
    serial_port_pattern_, -1};
  std::sregex_token_iterator iterator_end{};
  
  for (auto it = iterator_start; it != iterator_end; ++it) {
    all_ports_.push_back(serial_port_group + "/" + it->str());
  }
}

const std::string port_finder::PortFinder::findPort(const std::string& serial_port_group, 
    std::string id_vendor, std::string id_vendor_id, std::string id_model_id,
    std::string id_model) {
  listAllPorts(serial_port_group);
  for (const auto& serial_port : all_ports_) {
    if (findIDVendor(serial_port) == id_vendor && findIDVendorID(serial_port) ==  id_vendor_id  && 
        findIDModelID(serial_port) == id_model_id && findIDPath(serial_port) == id_model) {
      device_name_ = serial_port;
      break;
    }
  }
  return device_name_;
}