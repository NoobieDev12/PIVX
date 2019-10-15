// Copyright (c) 2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qt/pivx/coldstakingmodel.h"
#include "uint256.h"
#include "bitcoinunits.h"
#include "guiutil.h"
#include <iostream>
#include "addressbook.h"

ColdStakingModel::ColdStakingModel(WalletModel* _model,
                                   TransactionTableModel* _tableModel,
                                   AddressTableModel* _addressTableModel,
                                   QObject *parent) : QAbstractTableModel(parent), model(_model), tableModel(_tableModel), addressTableModel(_addressTableModel){
    updateCSList();
}

void ColdStakingModel::updateCSList(){
    emit dataChanged(index(0, 0, QModelIndex()), index(tableModel->csRowCount(), COLUMN_COUNT, QModelIndex()) );
}

int ColdStakingModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tableModel->csRowCount();
}

int ColdStakingModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}


QVariant ColdStakingModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
            return QVariant();

    int row = index.row();
    CSDelegation rec = tableModel->getDelegation(row);
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case DELEGATED_ADDRESS:
                return QString::fromStdString(rec.delegatedAddress);
            case DELEGATED_ADDRESS_LABEL:
                return addressTableModel->labelForAddress(QString::fromStdString(rec.delegatedAddress));
            case IS_WHITELISTED:
                return addressTableModel->purposeForAddress(rec.delegatedAddress).compare(AddressBook::AddressBookPurpose::DELEGATOR) == 0;
            case IS_WHITELISTED_STRING:
                return (addressTableModel->purposeForAddress(rec.delegatedAddress) == AddressBook::AddressBookPurpose::DELEGATOR ? "Staking" : "Not staking");
            case TOTAL_STACKEABLE_AMOUNT_STR:
                return GUIUtil::formatBalance(rec.cachedTotalAmount);
            case TOTAL_STACKEABLE_AMOUNT:
                return qint64(rec.cachedTotalAmount);
            case IS_RECEIVED_DELEGATION:
                return !rec.isSpendable;
        }
    }

    return QVariant();
}

bool ColdStakingModel::whitelist(const QModelIndex& modelIndex) {
    QString address = modelIndex.data(Qt::DisplayRole).toString();
    int idx = modelIndex.row();
    beginRemoveRows(QModelIndex(), idx, idx);
    bool ret = model->whitelistAddressFromColdStaking(address);
    endRemoveRows();
    emit dataChanged(index(idx, 0, QModelIndex()), index(idx, COLUMN_COUNT, QModelIndex()) );
    return ret;
}

bool ColdStakingModel::blacklist(const QModelIndex& modelIndex) {
    QString address = modelIndex.data(Qt::DisplayRole).toString();
    int idx = modelIndex.row();
    beginRemoveRows(QModelIndex(), idx, idx);
    bool ret = model->blacklistAddressFromColdStaking(address);
    endRemoveRows();
    emit dataChanged(index(idx, 0, QModelIndex()), index(idx, COLUMN_COUNT, QModelIndex()) );
    return ret;
}