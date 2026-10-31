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
        case QuoteRole:
            return trade.m_quoteCoin;
        case BaseRole:
            return trade.m_baseCoin;
        case CategoryRole:
            return QVariant::fromValue(trade.m_category);
        default:
            return QVariant();
        };

    }

    QHash<int, QByteArray> InstrumentModel::roleNames() const
    {
        return {
            {SymbolRole, "symbol"},
            {QuoteRole, "quote"},
            {BaseRole, "base"},
            {CategoryRole, "category"}
        };
    }

    void InstrumentModel::updateInstruments(const Tools::Instruments& instruments)
    {
        beginResetModel();
        m_instruments = instruments;
        endResetModel();
    }

}
