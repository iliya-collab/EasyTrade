#include <algorithm>
#include <limits>
#include "OrderbookSideModel.hpp"

namespace Core::Tools {

namespace {

const QList<int> kRawRoles { OrderbookSideModel::PriceRole,
                           OrderbookSideModel::VolumeRole,
                           OrderbookSideModel::TurnoverRole };
const QList<int> kCumRoles { OrderbookSideModel::TotalVolumeRole,
                           OrderbookSideModel::TotalTurnoverRole,
                           OrderbookSideModel::AvgPriceRole };
}

OrderbookSideModel::OrderbookSideModel(QObject *parent)
    : QAbstractListModel(parent), m_side(Bid) {}

OrderbookSideModel::OrderbookSideModel(Side side, QObject *parent)
    : QAbstractListModel(parent), m_side(side) {}

int OrderbookSideModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_orderbook.size();
}

QVariant OrderbookSideModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_orderbook.size())
        return QVariant();

    const auto &level = m_orderbook.at(index.row());

    switch (role) {
        case PriceRole:
            return level.price;
        case VolumeRole:
            return level.volume;
        case TurnoverRole:
            return level.turnover;
        case TotalVolumeRole:
            return level.totalVolume;
        case TotalTurnoverRole:
            return level.totalTurnover;
        case AvgPriceRole:
            return level.avgPrice;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> OrderbookSideModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[PriceRole]            = "price";
    roles[VolumeRole]           = "volume";
    roles[TurnoverRole]         = "turnover";
    roles[TotalVolumeRole]      = "totalVolume";
    roles[TotalTurnoverRole]    = "totalTurnover";
    roles[AvgPriceRole]         = "avgPrice";

    return roles;
}

void OrderbookSideModel::setSide(Side side)
{
    if (m_side == side)
        return;

    m_side = side;
    clear();

    emit sideChanged();
}

void OrderbookSideModel::snapshot(const QMap<QString, QString> &data)
{
    BookLevels levels;
    levels.reserve(data.size());

    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
        BookLevel l;
        l.price = it.key().toDouble();
        l.volume = it.value().toDouble();
        if (isVolumeLiquidation(l.volume))
            continue;
        l.turnover = l.price * l.volume;
        levels.push_back(l);
    }

    std::sort(levels.begin(), levels.end(),
              [this](const BookLevel& a, const BookLevel& b) { return before(a.price, b.price); });

    beginResetModel();
    m_orderbook = std::move(levels);
    recalcCumulative(0);
    endResetModel();

    updateStats();
}

void OrderbookSideModel::deltaUpdate(const QMap<QString, QString>& data)
{
    int minChanged = std::numeric_limits<int>::max();

    for (auto it = data.constBegin(); it != data.constEnd(); ++it)
    {
        const double price  = it.key().toDouble();
        const double volume = it.value().toDouble();

        const int pos = findBookLevel(price);
        const bool exists = pos < m_orderbook.size() && m_orderbook[pos].price == price;

        // уровень удалён
        if (exists && isVolumeLiquidation(volume))
        {
            beginRemoveRows({}, pos, pos);
            m_orderbook.removeAt(pos);
            endRemoveRows();
            minChanged = std::min(minChanged, pos);
        }
        // объём изменился
        else if (exists)
        {
            auto& l = m_orderbook[pos];
            if (l.volume == volume)
                continue;
            l.volume   = volume;
            l.turnover = price * volume;
            emit dataChanged(index(pos), index(pos), kRawRoles);
            minChanged = std::min(minChanged, pos);
        }
        // новый уровень
        else if (volume > 0)
        {
            BookLevel l;
            l.price    = price;
            l.volume   = volume;
            l.turnover = price * volume;
            beginInsertRows({}, pos, pos);
            m_orderbook.insert(pos, l);
            endInsertRows();
            minChanged = std::min(minChanged, pos);
        }
    }

    // ничего не изменилось
    if (minChanged == std::numeric_limits<int>::max())
        return;

    if (minChanged < m_orderbook.size())
    {
        recalcCumulative(minChanged);
        emit dataChanged(index(minChanged), index(m_orderbook.size() - 1), kCumRoles);
    }

    updateStats();
}

QVariantMap OrderbookSideModel::get(int index) const
{
    QVariantMap res;

    if (index < 0 || index >= m_orderbook.size())
        return res;

    auto item = m_orderbook[index];

    res["price"] = item.price;
    res["volume"] = item.volume;
    res["turnover"] = item.turnover;
    res["totalVolume"] = item.totalVolume;
    res["totalTurnover"] = item.totalTurnover;
    res["avgPrice"] = item.avgPrice;

    return res;
}

bool OrderbookSideModel::before(double a, double b) const
{
    return m_side == Bid ? a > b : a < b;
}

bool OrderbookSideModel::isVolumeLiquidation(double v) const
{
    return qFuzzyIsNull(v) && v <= 0;
}

int OrderbookSideModel::findBookLevel(double p) const
{
    auto it = std::lower_bound(m_orderbook.cbegin(), m_orderbook.cend(), p,
                               [this](const BookLevel& lvl, double price) { return before(lvl.price, price); });
    return (it - m_orderbook.cbegin());
}

void OrderbookSideModel::recalcCumulative(int from)
{
    double tv = 0, tt = 0;
    if (from > 0)
    {
        tv = m_orderbook[from - 1].totalVolume;
        tt = m_orderbook[from - 1].totalTurnover;
    }
    for (int i = from; i < m_orderbook.size(); i++)
    {
        auto& l = m_orderbook[i];
        tv += l.volume;
        tt += l.turnover;

        l.totalVolume = tv;
        l.totalTurnover = tt;
        l.avgPrice = tv > 0 ? tt / tv : 0;
    }
}

void OrderbookSideModel::updateStats()
{
    double maxVol = 0;
    for (const auto& l : std::as_const(m_orderbook))
        maxVol = std::max(maxVol, l.volume);

    const double tv = m_orderbook.isEmpty() ? 0.0 : m_orderbook.last().totalVolume;
    const double tt = m_orderbook.isEmpty() ? 0.0 : m_orderbook.last().totalTurnover;
    const int    n  = m_orderbook.size();

    if (m_count != n)
    {
        m_count = n;
        emit countChanged();
    }
    if (m_maxVolume != maxVol)
    {
        m_maxVolume = maxVol;
        emit maxVolumeChanged();
    }
    if (m_totalVolume != tv)
    {
        m_totalVolume = tv;
        emit totalVolumeChanged();
    }
    if (m_totalTurnover != tt)
    {
        m_totalTurnover = tt;
        emit totalTurnoverChanged();
    }
}

void OrderbookSideModel::clear()
{
    beginResetModel();
    m_orderbook.clear();
    m_count = 0;
    m_maxVolume = 0;
    m_totalVolume = 0;
    m_totalTurnover = 0;
    endResetModel();
    emit countChanged();
    emit maxVolumeChanged();
    emit totalVolumeChanged();
    emit totalTurnoverChanged();
}

}