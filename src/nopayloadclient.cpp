#include <nopayloadclient/nopayloadclient.hpp>


namespace nopayloadclient {

Client::Client() {
    config_ = config::getDict();
    backend_ = new Backend(config_);
    plmover_ = new PLMover(config_);
}

// Reading
json Client::get(std::string gt_name, std::string pl_type, long long major_iov, long long minor_iov){
    TRY(
        return makeResp(backend_->getPayloadUrls(gt_name, pl_type, major_iov, minor_iov));
    )
}

// Writing
json Client::createGlobalTag(std::string gt_name) {
    TRY(
        backend_->createGlobalTag(gt_name);
        return makeResp("successfully created global tag");
    )
}

json Client::deleteGlobalTag(std::string name) {
    TRY(
        backend_->deleteGlobalTag(name);
        return makeResp("successfully deleted global tag");
    )
}

json Client::lockGlobalTag(std::string name) {
    TRY(
        backend_->lockGlobalTag(name);
        return makeResp("successfully locked global tag");
    )
}

json Client::unlockGlobalTag(std::string name) {
    TRY(
        backend_->unlockGlobalTag(name);
        return makeResp("successfully unlocked global tag");
    )
}

json Client::createPayloadType(std::string name) {
    TRY(
        backend_->createPayloadType(name);
        return makeResp("successfully created payload type");
    )
}

json Client::insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart){
    TRY(
        payload::Payload pl = payload::Payload(fileUrl, pl_type);
        plmover_->prepareUploadFile(pl);
        backend_->prepareInsertIov(gt_name, pl);
        plmover_->uploadFile(pl);
        backend_->insertIov(gt_name, pl, major_iovStart, minor_iovStart);
        return makeResp("successfully inserted payload");
    )
}

json Client::insertPayload(std::string gt_name, std::string pl_type, std::string fileUrl,
                             long long major_iovStart, long long minor_iovStart,
                             long long major_iovEnd, long long minor_iovEnd){
    TRY(
        payload::Payload pl = payload::Payload(fileUrl, pl_type);
        plmover_->prepareUploadFile(pl);
        backend_->prepareInsertIov(gt_name, pl);
        plmover_->uploadFile(pl);
        backend_->insertIov(gt_name, pl, major_iovStart, minor_iovStart, major_iovEnd, minor_iovEnd);
        return makeResp("successfully inserted payload");
    )
}

// Helper
json Client::getSize(){
    TRY(
        return makeResp(backend_->getSize());
    )
}

json Client::getPayloadTypes(){
    TRY(
        return makeResp(backend_->getPayloadTypes());
    )
}

json Client::getGlobalTags(){
    TRY(
        return makeResp(backend_->getGlobalTags());
    )
}

json Client::checkConnection(){
    TRY(
        return makeResp(backend_->checkConnection());
    )
}

json Client::getConfDict(){
    TRY(
        return makeResp(config::getDict());
    )
}

std::ostream& operator<<(std::ostream& os, const nopayloadclient::Client& c) {
    os << "Client instance with following attributes:" << std::endl;
    //os << "global tag = " << c.global_tag_ << std::endl;
    os << "config = " << c.config_ << std::endl;
    return os;
}

template<typename T>
json Client::makeResp(T msg) {
    return json::object({{"code", 0}, {"msg", msg}});
}

}
