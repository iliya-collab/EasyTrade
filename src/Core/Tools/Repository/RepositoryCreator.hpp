#pragma once

#include "BaseRepository.hpp"
#include <type_traits>
#include <memory>

namespace Core::Tools
{

class RepositoryCreator
{
public:

    static RepositoryCreator& instance();

    template <typename TRep>
    std::unique_ptr<TRep> create(const QString& dbPath, IDatabaseManager& dbManager) const
    {
        using TRow = typename TRep::RowType;

        static_assert(std::is_base_of<BaseRepository<TRow>, TRep>::value,
                      "TRep must inherit from BaseRepository!");

        auto rep = std::make_unique<TRep>(dbPath, dbManager);

        if (!rep->init())
            throw std::invalid_argument("Failed to initialize the repository");

        return rep;
    }

private:

    RepositoryCreator() = default;
    RepositoryCreator(const RepositoryCreator&) = delete;
    RepositoryCreator& operator=(const RepositoryCreator&) = delete;

};

}
