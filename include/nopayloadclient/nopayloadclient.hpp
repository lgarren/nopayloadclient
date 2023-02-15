#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

#include <nopayloadclient/resthandler.hpp>
#include <nopayloadclient/payload.hpp>
#include <nopayloadclient/iov.hpp>
#include <nopayloadclient/plhandler.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/exception.hpp>

#define TRY(...) {              \
    try {                       \
        __VA_ARGS__             \
    }                           \
    catch (BaseException &e) {  \
        return e.jsonify();     \
    }                           \
}                               \

using json = nlohmann::json;
using ll = long long;

namespace nopayloadclient {

class Client {
public:
    Client();
    Client(std::string gt_name);

    // use return of dict, not old 'get' method
    // use virtual functions in interface

    // Configuration
    json setGlobalTag(std::string name);
    json getGlobalTag();

    // Reading
    json getUrlDict(ll major_iov, ll minor_iov);

    // Writing
    json createPayloadType(std::string pl_type);
    json createGlobalTag();
    json deleteGlobalTag();
    json lockGlobalTag();
    json unlockGlobalTag();
    json insertPayload(std::string pl_type, std::string file_url,
                       ll major_iov_start, ll minor_iov_start);
    json insertPayload(std::string pl_type, std::string file_url,
                       ll major_iov_start, ll minor_iov_start,
                       ll major_iov_end, ll minor_iov_end);

    // Helper
    json getSize();
    json getPayloadTypes();
    json getGlobalTags();
    json checkConnection();
    json getConfDict();
    json clearCache();
    friend std::ostream& operator<<(std::ostream& os, const Client& c);
    template<typename T>
    json makeResp(T msg);

private:
    json config_;
    RESTHandler rest_handler_;
    PLHandler pl_handler_;

    // Writing
    void prepareInsertIov(Payload &pl);
    void insertIov(Payload& pl, IOV& iov);
    void insertPayload(Payload &pl, IOV &iov);
    void createNewPll(std::string pl_type);
    void attachPayloadList(std::string pl_name);

    // Reading
    bool gtExists();
    bool gtStatusExists(std::string name);
    bool plTypeExists(std::string pl_type);
    bool gtHasPlType(std::string pl_type);
    void checkGtExists();
    void checkGtStatusExists(std::string name);
    void checkPlTypeExists(std::string name);
    json getUrlDict(Moment& mom);

    // Helper
    bool objWithNameExists(const json& j, std::string name);
};

}
