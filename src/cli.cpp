#include <nopayloadclient/cli.hpp>

namespace nopayloadclient {

CLI::CLI() {
    insertCommand("getSize", &CLI::getSize);
    insertCommand("createPayloadType", &CLI::createPayloadType);
    insertCommand("getConfDict", &CLI::getConfDict);
    insertCommand("getPayloadTypes", &CLI::getPayloadTypes);
    insertCommand("getGlobalTags", &CLI::getGlobalTags);
    insertCommand("checkConnection", &CLI::checkConnection);
    insertCommand("createGlobalTag", &CLI::createGlobalTag);
    insertCommand("deleteGlobalTag", &CLI::deleteGlobalTag);
    insertCommand("lockGlobalTag", &CLI::lockGlobalTag);
    insertCommand("unlockGlobalTag", &CLI::unlockGlobalTag);
    insertCommand("getUrlDict", &CLI::getUrlDict);
    insertCommand("insertPayload", &CLI::insertPayload);
}

json CLI::getSize(Client& c, int& argc, char* argv[]) {
    return c.getSize();
}

json CLI::createPayloadType(Client& c, int& argc, char* argv[]) {
    std::string name = argv[2];
    return c.createPayloadType(name);
}

json CLI::getConfDict(Client& c, int& argc, char* argv[]) {
    return c.getConfDict();
}

json CLI::getPayloadTypes(Client& c, int& argc, char* argv[]) {
    return c.getPayloadTypes();
}

json CLI::getGlobalTags(Client& c, int& argc, char* argv[]) {
    return c.getGlobalTags();
}

json CLI::checkConnection(Client& c, int& argc, char* argv[]) {
    return c.checkConnection();
}

json CLI::getUrlDict(Client& c, int& argc, char* argv[]) {
    std::string gt_name = argv[2];
    long long major_iov = std::atoi(argv[3]);
    long long minor_iov = std::atoi(argv[4]);
    c.setGlobalTag(gt_name);
    return c.getUrlDict(major_iov, minor_iov);
}

json CLI::createGlobalTag(Client& c, int& argc, char* argv[]){
    std::string name = argv[2];
    c.setGlobalTag(name);
    return c.createGlobalTag();
}

json CLI::deleteGlobalTag(Client& c, int& argc, char* argv[]){
    std::string name = argv[2];
    c.setGlobalTag(name);
    return c.deleteGlobalTag();
}

json CLI::lockGlobalTag(Client& c, int& argc, char* argv[]){
    std::string name = argv[2];
    c.setGlobalTag(name);
    return c.lockGlobalTag();
}

json CLI::unlockGlobalTag(Client& c, int& argc, char* argv[]){
    std::string name = argv[2];
    c.setGlobalTag(name);
    return c.unlockGlobalTag();
}

json CLI::insertPayload(Client& c, int& argc, char* argv[]){
    std::string gt = argv[2];
    std::string pt = argv[3];
    std::string file_url = argv[4];
    long long major_iov_start = std::atoi(argv[5]);
    long long minor_iov_start = std::atoi(argv[6]);
    c.setGlobalTag(gt);
    if (argc == 7) {
        return c.insertPayload(pt, file_url, major_iov_start, minor_iov_start);
    }
    else if (argc == 9) {
        long long major_iov_end = std::atoi(argv[7]);
        long long minor_iov_end = std::atoi(argv[8]);
        return c.insertPayload(pt, file_url, major_iov_start, minor_iov_start,
                               major_iov_end, minor_iov_end);
    }
    std::string t = "insertPayload takes 5 or 7 arguments (" + std::to_string(argc-2) + " were given).";
    return BaseException(t).jsonify();
}

}

int main(int argc, char *argv[])
{
    nopayloadclient::CLI cli;
    nopayloadclient::Client client;
    std::cout << cli.callCommand(argv[1], &client, argc, *argv) << std::endl;
    return 0;
}