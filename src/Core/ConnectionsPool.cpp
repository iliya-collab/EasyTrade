#include "ConnectionsPool.hpp"

Core::ConnectionsPool::ConnectionsPool() {}

size_t Core::ConnectionsPool::size() const
{
    return m_pool.size();
}

bool Core::ConnectionsPool::contains(Tools::Types::EnumHelpers::MarketType market) const
{
    const auto key = Eh::marketTypeToApiString(market).toStdString();
    return m_pool.find(key) != m_pool.end();
}

void Core::ConnectionsPool::addStream(std::unique_ptr<Streamer::IStreamer> stream)
{
    if (!stream)
        return;

    Streamer::IStreamer* raw = stream.get();
    const auto key = stream->id().toStdString();

    auto [it, inserted] = m_pool.emplace(key, std::move(stream));
    if (!inserted)
        return;

    m_hashPool.push_back(raw);
}

Core::Streamer::IStreamer* Core::ConnectionsPool::operator[] (Eh::MarketType market) const
{
    const auto key = Eh::marketTypeToApiString(market).toStdString();
    const auto it = m_pool.find(key);
    return (it != m_pool.end()) ? it->second.get() : nullptr;
}

std::vector<Core::Streamer::IStreamer *> Core::ConnectionsPool::streams() const
{
    return m_hashPool;
}
