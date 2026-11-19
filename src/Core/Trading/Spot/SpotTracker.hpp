#ifndef SPOTTRACKER_HPP
#define SPOTTRACKER_HPP
#include "Trading/Common/IPositionTracker.hpp"

namespace Core::Trading
{

class SpotTracker final : public IPositionTracker
{

// private:
//     struct TrackedOrder
//     {
//         int64_t time = 0;       // время последнего обновления, для отсечения устаревших сообщений
//         double qty = 0;         // размер ордера
//         double cumQty = 0;      // исполнено всего
//         bool terminal = false;  // Filled / Cancelled / Rejected
//         bool buy = false;
//     };

public:

    explicit SpotTracker(double minQty);

    void onOrder(const OrderState& o) override;
    void onExec(const ExecutionState& e) override;
    void onBarClosed(const Bar& b) override;
    void setStop(double stop) override;
    void reset() override;
    AnyPosition snapshot() const override;

    void onWallet(const WalletState& w);

    bool entryKnown() const;    // false => стратегии торговать нельзя (entry неизвестен)

private:

    void applyBasis(const ExecutionState& e);
    void refreshEntry();
    bool isDust(double qty) const;
    void resetPosition();

    int64_t m_walletTime{};
    double m_minQty{};

    SpotPosition m_pos{};

    double m_heldQty = 0;   // количество по исполнениям, с комиссией
    double m_costQuote = 0; // стоимость базы в котируемой монете, с комиссией
    std::unordered_map<std::string, int64_t> m_seenExec;    // execId -> time
};

}

#endif // SPOTTRACKER_HPP
