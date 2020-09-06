#include "clashcmd.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <future>
#include <iomanip>
#include <iostream>
#include <thread>

#include <json/json.h>

ClashCmd::ClashCmd(const std::string& apiUrl)
    : cli_(apiUrl.data()), hasDetechedDelay_(false), proxyNameMaxLen_(0),
      currentIndex_(-1) {
  try {
    auto res = cli_.Get("/");
    if (res == nullptr || res->status != 200) {
      std::cout << "Please run clash first.\n";
      exit(2);
    }
  } catch (...) {
      std::cout << "Please run clash first.\n";
      exit(2);
  }
  getProxies();
  proxyNameMaxLen_ += 4 - (proxyNameMaxLen_% 4);
}

void ClashCmd::getProxies() {
  auto res = cli_.Get("/proxies");
  Json::Value body;
  Json::Reader reader;
  reader.parse(res->body, body, false);
  
  std::vector<std::string> list;
  Json::Value& proxies = body["proxies"]["GLOBAL"]["all"];
  size_t size = proxies.size();
  for (int i = 0; i < proxies.size(); ++i) {
    allProxies_.emplace_back(proxies[i].asString(), -1);
    if (proxyNameMaxLen_ < allProxies_[i].first.size()) {
      proxyNameMaxLen_ = allProxies_[i].first.size();
    }
  }
  if (proxies.size() == 0) {
    std::cout << "There is no proxy\n";
  }
}

std::string ClashCmd::getProxyInfo(int index) {
  if (index >= allProxies_.size()) {
    std::cout << "Index is out of range\n";
    return "";
  }
  std::string query = "/proxies/" + allProxies_[index].first;
  auto res = cli_.Get(query.data());
  if (res->status == 200) {
    return res->body;
  } else {
    std::cout << "Fail:" << res->status << "\n";
    return "";
  }
}

int ClashCmd::getProxyDelay(int index) {
  if (index >= allProxies_.size()) {
    std::cout << "Index is out of range\n";
    return -1;
  }
  std::string query =
      "/proxies/" + allProxies_[index].first +
      "/delay?url=http://cp.cloudflare.com/generate_204&timeout=5000";
  auto res = cli_.Get(query.data());
  if (res == nullptr || res->status != 200) {
    return -1;
  }
  
  size_t pos = res->body.find("delay\":");
  if (pos != std::string::npos) {
    return std::stoi(res->body.substr(pos + 7));
  }
  return -1;
}

void ClashCmd::getAllProxiesDealy() {
  hasDetechedDelay_ = true;
  int cores = std::thread::hardware_concurrency(), i;
  cores = cores > 8 ? cores : 8;
  std::vector<std::future<int>> tasks;
  // 留一些资源
  tasks.reserve(cores);
  for (i = 0; i + cores < allProxies_.size(); i += cores) {
    tasks.clear();
    for (int j = 0; j < cores; ++j) {
      
      tasks.push_back(
          std::async(
              std::launch::async | std::launch::deferred,
              std::bind(&ClashCmd::getProxyDelay, this, std::placeholders::_1),
              i + j));
    }
    for (int j = 0; j < cores; ++j) {
      allProxies_[i + j].second = tasks[j].get();
    }
  }

  while (i < allProxies_.size()) {
    allProxies_[i].second = getProxyDelay(i);
    ++i;
  }

  std::sort(allProxies_.begin(), allProxies_.end(),
      [](const std::pair<std::string, int>& a,
         const std::pair<std::string, int>& b) -> bool {
        if (a.second == -1) {
          return false;
        } else if (b.second == -1) {
          return true;
        }
        return a.second < b.second;
      });
}

void ClashCmd::printAllProxies() {
  int size = allProxies_.size();
  int width = int(std::log(size));
  for (int i = 0; i < size; ++i) {
    std::cout
        << std::setiosflags(std::ios::left)
        << std::setw(width) << i << ": "
        << std::setiosflags(std::ios::left) << std::setw(proxyNameMaxLen_)
        << allProxies_[i].first << "\n";
  }
}

void ClashCmd::printAllProxiesAnyDelays() {
  if (!hasDetechedDelay_) {
    getAllProxiesDealy();
  }
  int size = allProxies_.size();
  int width = int(std::log(size));
  width = width > 0 ? width : 1;
  for (int i = 0; i < size; ++i) {
    std::cout
        << std::setiosflags(std::ios::left)
        << std::setw(width) << i << ": "
        << std::setw(proxyNameMaxLen_ + 4)
        << allProxies_[i].first << allProxies_[i].second << "\n";
  }
}

void ClashCmd::selectProxy(int index) {
  if (index >= allProxies_.size()) {
    std::cout << "Index is out of range\n";
    return;
  }

  std::string query = "/proxies/🔰国外流量";
  const char* contentType = "application/json";
  auto res = cli_.Put(
      query.data(), 
      "{\"name\":\"" + allProxies_[index].first + "\"}", contentType);
  if (res->status == 204) {
    std::cout << "Proxy swtiches to " << allProxies_[index].first << "\n";
    currentIndex_ = index;
  } else {
    std::cout << "Failed:" << res->status << "\n" << res->body << "\n";
  }
}

void ClashCmd::printCurrentProxy() {
  if (currentIndex_ == -1) {
    std::cout << "Index:0\nProxy:DIRECT\n";
  } else {
    std::cout
        << "Index:" << currentIndex_ << "\nProxy:"
        << allProxies_[currentIndex_].first << "\n";
  }
}

std::string ClashCmd::proxyName(int index) {
  return allProxies_[index].first;
}