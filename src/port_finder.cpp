#include "port_finder/port_finder.hpp"

port_finder::PortFinder::PortFinder() {

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