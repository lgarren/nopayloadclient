#pragma once

#include <iostream>
#include <sys/stat.h>
#include <string>
#include <experimental/filesystem>
#include <fstream>
#include <unistd.h>

#include <nopayloadclient/exception.hpp>
#include <nopayloadclient/config.hpp>
#include <nopayloadclient/payload.hpp>

namespace plmover {

    void compareCheckSums(std::string firstFileUrl, std::string secondFileUrl);
    void prepareUploadFile(payload::Payload& pl);
    void uploadFile(payload::Payload& pl);

}


class PLMover {
public:
    PLMover(const nlohmann::json& config);
private:
    std::string write_dir_;
    std::vector<std::string> read_dir_list_;
};