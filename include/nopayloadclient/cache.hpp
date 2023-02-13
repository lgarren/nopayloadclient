#pragma once

#include <vector>
#include <iostream>
#include <chrono>
#include <map>
#include <queue>
#include <nlohmann/json.hpp>

#include <nopayloadclient/exception.hpp>

using json = nlohmann::json;

namespace nopayloadclient {

class Cache {
public:
    Cache() {};
    Cache(const json& config);

    /// should check if it exists AND
    /// entry is less than life time old
    bool contains(std::string url);

    json get(std::string url);

    /// put in first, then removed oldest entry
    /// until size is under max size again
    void set(std::string url, json& response);
    void trash();
    friend std::ostream& operator<< (std::ostream& os, const Cache& c);

private:
    /// of the form {url_1: resp_1, url_2, resp_2, ...}
    json response_dict_;
    /// of the form {url_1: ts_1, url_2: ts_2, ...}
    json time_stamp_dict_;
    /// to find oldest element quickly
    std::queue<std::string> insertion_order_;
    void removeOldestEntry();
    bool isAboveMaxSize();
    double getSize();
    bool isEmpty();
    long long getNowTs();
    double life_time_;
    double max_mb_;
};

}
