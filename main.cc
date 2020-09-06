#include "clashcmd.h"


int main() {
  ClashCmd clash("http://localhost:9090");
  std::cout << "Init finished. Please input command.\n";
  int cmd, opt;
  while (std::cin >> cmd) {
    switch (cmd) {
      case 0:
        std::cout << "0:" << "Print all commands option.\n";
        std::cout << "1:" << "Print all proxies.\n";
        std::cout << "2:" << "Print all proxies and delays.\n";
        std::cout << "3:" << "Print current proxy.\n";
        std::cout << "4:" << "Input an proxy index and output its information.\n";
        std::cout << "5:" << "Input an proxy index and output its delay.\n";
        std::cout << "6:" << "Input an proxy index and select it as current proxy.\n";
        std::cout << "8:" << "Refresh dalay test results.\n";
        std::cout << "9:" << "Exit.\n";
        break;
      case 1:
        clash.printAllProxies();
        break;
      case 2:
        std::cout << "Start testing delay\n";
        clash.printAllProxiesAnyDelays();
        break;
      case 3:
        clash.printCurrentProxy();
        break;
      case 4:
        std::cin >> opt;
        std::cout << clash.getProxyInfo(opt) << "\n";
        break;
      case 5:
        std::cin >> opt;
        std::cout << "Proxy:" << clash.proxyName(opt) << "\nDelay:";
        std::cout << clash.getProxyDelay(opt) << "\n";
        break;
      case 6:
        std::cin >> opt;
        clash.selectProxy(opt);
        break;
      case 8:
        clash.getAllProxiesDealy();
        std::cout << "Refresh finished\n";
        break;
      case 9:
        exit(0);
        break;
      default:
        std::cout << "Input 0 to get help information\n";
        break;
    }
    std::cout << "===============================\n";
  }
}
