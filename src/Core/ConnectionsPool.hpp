#ifndef CONNECTIONSPOOL_HPP
#define CONNECTIONSPOOL_HPP
#include <string>
#include <map>
#include <memory>
#include "Streamer/IStreamer.hpp"
#include "Tools/Types/EnumHelpers.hpp"

namespace Core
{

class ConnectionsPool
{
private:

    using Eh = Tools::Types::EnumHelpers;

    std::map<std::string, std::unique_ptr<Streamer::IStreamer>> m_pool;
    std::vector<Streamer::IStreamer*> m_hashPool;

public:

    explicit ConnectionsPool();

    size_t size() const;

    bool contains(Eh::MarketType market) const;

    void addStream(std::unique_ptr<Streamer::IStreamer> stream);

    Streamer::IStreamer* operator[] (Eh::MarketType market) const;

    std::vector<Streamer::IStreamer*> streams() const;

};

}

#endif // CONNECTIONSPOOL_HPP
