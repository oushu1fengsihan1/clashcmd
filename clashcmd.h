#include <algorithm>
#include <iostream>
#include <utility>

#include <http.hpp>

class ClashCmd {
 public:
  ClashCmd(const std::string& apiUrl);
  std::string getProxyInfo(int index);
  std::string proxyName(int index);
  int getProxyDelay(int index);
  void getAllProxiesDealy();

  void printAllProxies();
  void printAllProxiesAnyDelays();
  void printCurrentProxy();

  void selectProxy(int index);

 private:
  void getProxies();

  httplib::Client cli_;
  std::vector<std::pair<std::string, int>> allProxies_;
  bool hasDetechedDelay_;
  int proxyNameMaxLen_;
  int currentIndex_;
};