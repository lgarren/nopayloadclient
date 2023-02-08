#pragma once

#include<iostream>

#include <nopayloadclient/moment.hpp>
#include <nopayloadclient/exception.hpp>

using ll = long long;

namespace npc {

struct IOV {
    Moment start;
    Moment end;
    bool is_open;
    IOV(npc::Moment start_);
    IOV(npc::Moment start_, npc::Moment end_);
    IOV(ll major_start, ll minor_start) :
        IOV(Moment(major_start, minor_start)) {};
    IOV(ll major_start, ll minor_start, ll major_end, ll minor_end) :
        IOV(Moment(major_start, minor_start), Moment(major_end, minor_end)) {};
    friend std::ostream& operator<< (std::ostream& os, const npc::IOV& i);
};

}
