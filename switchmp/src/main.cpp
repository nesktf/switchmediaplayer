#include <borealis.hpp>
#include <cstdlib>

#include "activity/main_activity.hpp"

int main(int argc, char* argv[]) {
  brls::Logger::setLogLevel(brls::LogLevel::LOG_INFO);

  if (!brls::Application::init()) {
    brls::Logger::error("Unable to init application");
    return EXIT_FAILURE;
  }

  brls::Application::createWindow("switchmp");
  brls::Logger::info("createWindow done");

  brls::Application::pushActivity(new MainActivity());

  while(brls::Application::mainLoop()) {}

  return 0;
}
