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

    explicit InstrumentModel(QObject* parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void updateInstruments(const Tools::Instruments& instruments);
    auto size() { return m_instruments.size(); }

private:

    Tools::Instruments m_instruments;

};

}
