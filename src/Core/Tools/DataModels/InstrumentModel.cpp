#include <algorithm>
#include "InstrumentModel.hpp"

namespace Core::Tools {

InstrumentModel::InstrumentModel(QObject *parent) : QAbstractListModel(parent) {}

int InstrumentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_instruments.size());
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

void InstrumentModel::rebuildIndex()
{
    m_indexById.clear();
    m_indexById.reserve(m_instruments.size());
    for (int i = 0; i < m_instruments.size(); ++i)
        m_indexById.insert(m_instruments.at(i).id(), i);
}

void InstrumentModel::resetInstruments(const Instruments& instruments)
{
    beginResetModel();
    m_instruments = instruments;
    rebuildIndex();
    endResetModel();
}

void InstrumentModel::addBatchInstruments(const Instruments& instruments)
{
    if (instruments.isEmpty())
        return;

    Instruments toAppend;
    toAppend.reserve(instruments.size());

    QSet<QString> seenInBatch;
    seenInBatch.reserve(instruments.size());

    for (const auto& inst : instruments)
    {
        const QString id = inst.id();

        if (m_indexById.contains(id))
            continue;

        if (seenInBatch.contains(id))
            continue;

        seenInBatch.insert(id);
        toAppend.push_back(inst);
    }

    if (toAppend.isEmpty())
        return;

    const int first = m_instruments.size();
    const int last  = first + toAppend.size() - 1;

    beginInsertRows(QModelIndex(), first, last);

    int row = m_instruments.size();
    for (const auto& inst : std::as_const(toAppend))
    {
        m_instruments.append(inst);
        m_indexById.insert(inst.id(), row++);
    }

    endInsertRows();
}

void InstrumentModel::upsertInstruments(const Instruments& instruments)
{
    if (instruments.isEmpty())
        return;

    QVector<int> changedRows;
    changedRows.reserve(instruments.size());

    Instruments toAppend;
    toAppend.reserve(instruments.size());

    for (const auto& inst : instruments)
    {
        auto it = m_indexById.constFind(inst.id());
        if (it != m_indexById.cend())
        {
            const int row = it.value();
            if (m_instruments.at(row).isSame(inst))
                continue;

            m_instruments[row] = inst;
            changedRows.push_back(row);
        }
        else
        {
            toAppend.push_back(inst);
        }
    }

    if (!changedRows.isEmpty())
    {
        for (int row : std::as_const(changedRows))
            emit dataChanged(index(row, 0), index(row, 0),
                             {SymbolRole, CategoryRole, InstrumentRole});
    }

    if (!toAppend.isEmpty())
    {
        const int first = m_instruments.size();
        const int last  = first + toAppend.size() - 1;

        beginInsertRows(QModelIndex(), first, last);

        int row = m_instruments.size();
        for (const auto& inst : std::as_const(toAppend))
        {
            m_instruments.append(inst);
            m_indexById.insert(inst.id(), row++);
        }

        endInsertRows();
    }
}

void InstrumentModel::addInstrument(const InstrumentInfo &inst)
{
    if (m_indexById.contains(inst.id()))
        return;

    const auto row = m_instruments.size();
    beginInsertRows(QModelIndex(), row, row);
    m_instruments.append(inst);
    m_indexById.insert(inst.id(), row);
    endInsertRows();
}

void InstrumentModel::removeInstrument(const InstrumentInfo &inst)
{
    auto it = m_indexById.constFind(inst.id());
    if (it == m_indexById.cend())
        return;

    const int row = it.value();

    beginRemoveRows(QModelIndex(), row, row);
    m_instruments.removeAt(row);
    endRemoveRows();

    m_indexById.remove(inst.id());
    for (auto i = m_indexById.begin(); i != m_indexById.end(); ++i)
    {
        if (i.value() > row)
            --i.value();
    }
}

size_t InstrumentModel::size() { return m_instruments.size(); }

}
