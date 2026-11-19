#pragma once
#include <Tools/StdTypes.hpp>
#include <QAbstractListModel>

namespace Core::Tools
{

class InstrumentModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:

    enum Roles
    {
        SymbolRole = Qt::UserRole + 1,
        CategoryRole,
        InstrumentRole
    };

    explicit InstrumentModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void resetInstruments(const Instruments& instruments);
    void addBatchInstruments(const Instruments& instruments);
    void upsertInstruments(const Instruments& instruments);
    void addInstrument(const InstrumentInfo& inst);
    void removeInstrument(const InstrumentInfo& inst);
    size_t size();

private:

    void rebuildIndex();

    Instruments m_instruments;
    QHash<QString, int> m_indexById;

};

}
