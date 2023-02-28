#include <nopayloadclient/resthandler.hpp>

namespace nopayloadclient {

// Config
RESTHandler::RESTHandler(const json& config) {
    //curlwrapper_ = CurlWrapper(config);
    if (config["use_fake_backend"]) {
        curlwrapper_.reset(new CurlFaker(config));
    }
    else {
        curlwrapper_.reset(new CurlWrapper(config));
    }
    cache_ = Cache(config);
    use_cache_ = true;
}

void RESTHandler::clearCache() {
    cache_.trash();
}

void RESTHandler::setGlobalTag(std::string name) {
    global_tag_ = name;
}

std::string RESTHandler::getGlobalTag() {
    return global_tag_;
}

// Reading
json RESTHandler::getGlobalTags() {
    return get("globalTags");
}

json RESTHandler::getGlobalTagStatuses() {
    return get("gtstatus");
}

json RESTHandler::getPayloadTypes() {
    return get("pt");
}

json RESTHandler::getPayloadLists() {
    return get("gtPayloadLists/" + global_tag_);
}

json RESTHandler::getPayloadIOVs(Moment& mom){
    return get("payloadiovs/?gtName=" + global_tag_ + "&majorIOV=" +
               std::to_string(mom.major_) + "&minorIOV=" + std::to_string(mom.minor_));
}

// Writing
void RESTHandler::createGlobalTagStatus(std::string name){
    post("gtstatus", {{"name", name}});
}

void RESTHandler::createGlobalTagObject(std::string status) {
    json j = {
        {"name", global_tag_},
        {"status", status},
        {"author", std::getenv("USER")}
    };
    post("gt", j);
}

void RESTHandler::createPayloadType(std::string name){
    post("pt", {{"name", name}});
}

std::string RESTHandler::createPayloadList(std::string type){
    json res = post("pl", {{"payload_type", type}});
    return res["name"];
}

void RESTHandler::attachPayloadList(std::string pl_name){
    json j = {
        {"payload_list", pl_name},
        {"global_tag", global_tag_}
    };
    put("pl_attach", j);
}

void RESTHandler::lockGlobalTag(){
    put("gt_change_status/" + global_tag_ + "/locked");
}

void RESTHandler::unlockGlobalTag(){
    put("gt_change_status/" + global_tag_ + "/unlocked");
}

ll RESTHandler::createPayloadIOV(Payload& pl, IOV& iov){
    json j = {
        {"payload_url", pl.remote_url},
        {"major_iov", iov.start_.major_},
        {"minor_iov", iov.start_.minor_},
        {"checksum", pl.check_sum}
    };
    if (!iov.is_open_){
        j["major_iov_end"] = iov.end_.major_;
        j["minor_iov_end"] = iov.end_.minor_;
    }
    json res = post("piov", j);
    return res["id"];
}

void RESTHandler::attachPayloadIOV(std::string pll_name, ll piov_id){
    json j = {
        {"payload_list", pll_name},
        {"piov_id", piov_id}
    };
    put("piov_attach", j);
}

void RESTHandler::deleteGlobalTag() {
    del("deleteGlobalTag/" + global_tag_);
}

// Private
json RESTHandler::get(std::string url) {
    if (!use_cache_) return curlwrapper_->get(url);
    if (!cache_.contains(url)) {
        json resp = curlwrapper_->get(url);
        cache_.set(url, resp);
    }
    return cache_.get(url);
}

json RESTHandler::del(std::string url, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->del(url);
}

json RESTHandler::put(std::string url, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->put(url);
}

json RESTHandler::put(std::string url, json data, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->put(url, data);
}

json RESTHandler::post(std::string url, json data, bool trash_cache) {
    if (trash_cache) cache_.trash();
    return curlwrapper_->post(url, data);
}

}
