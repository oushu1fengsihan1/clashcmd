#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include <json/json.h>
#include <http.hpp>

std::vector<std::string> getProxies(httplib::Client& cli) {
  auto res = cli.Get("/proxies");
  Json::Value body;
  Json::Reader reader;
  reader.parse(res->body, body, false);
  
  std::vector<std::string> list;
  Json::Value& allProxies = body["proxies"]["GLOBAL"]["all"];
  size_t size = allProxies.size();
  for (int i = 0;i < allProxies.size(); ++i) {
    list.push_back(allProxies[i].asString());
  }
  return list;
}

void getProxyInfo(httplib::Client& cli, std::string& proxy) {
  std::string query = "/proxies/" + proxy;
  auto res = cli.Get(query.data());
  std::cout << res->body << "\n";
}


int getProxyDelay(httplib::Client& cli, std::string& proxy) {
  std::string query = "/proxies/" + proxy + "/delay?url=http://cp.cloudflare.com/generate_204&timeout=5000";
  auto res = cli.Get(query.data());
  std::cout << res->status << "\n";
  std::cout << res->body << "\n";
  std::cout << query << "\n";
}

int main() {
  httplib::Client cli("http://localhost:9090");
  std::vector<std::string> proxies = getProxies(cli);

  getProxyInfo(cli, proxies[0]);
  getProxyDelay(cli, proxies[0]);
}

