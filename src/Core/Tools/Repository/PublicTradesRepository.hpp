#pragma once
#include "BaseRepository.hpp"

namespace Core::Tools {

    class PublicTradesRepository : public BasePublicTradesRepository {
    public:

        PublicTradesRepository(const QString& dbPath, IDatabaseManager& manager);
        ~PublicTradesRepository() = default;

        bool init() override;
        bool open() override;
        void close() override;
        QString error() override;
        bool clear() override;

    };
}
