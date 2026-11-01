#include "InstrumentModel.hpp"

namespace Core::Tools {

    int InstrumentModel::rowCount(const QModelIndex &parent) const
    {
        return m_instruments.size();
    }

    QVariant InstrumentModel::data(const QModelIndex &index, int role) const
    {

        if (!index.isValid() || index.row() >= m_instruments.size())
            return QVariant();

        const auto& trade = m_instruments.at(index.row());

        switch (role)
        {
        case SymbolRole:
            return trade.m_symbol;
        case CategoryRole:
            return QVariant::fromValue(trade.m_category);
        case InstrumentRole:
            return QVariant::fromValue(trade);
        default:
            return QVariant();
        };

    }

    QHash<int, QByteArray> InstrumentModel::roleNames() const
    {
        return {
            {SymbolRole, "symbol"},
            {CategoryRole, "category"},
            {InstrumentRole, "instrument"}
        };
    }

    void InstrumentModel::updateInstruments(const Tools::Instruments& instruments)
    {
        beginResetModel();
        m_instruments = instruments;
        endResetModel();
    }

}
