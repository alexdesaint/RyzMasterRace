#pragma once

#include <QtWidgets/QtWidgets>
#include <Register.hpp>

template<class T>
class QRegister : public QWidget {
private:
    QVBoxLayout *layout;
    QTreeView *table;
    QLabel *infoLabel;
    QStandardItemModel *standardModel;
    std::vector<T> &registers;

    unsigned int width;

    void selected(const QModelIndex &current, const QModelIndex &previous) {
        // TODO: use the internal id to identifie the data
        //if (index.parent().row() == -1)
        //    infoLabel->setText(MSRRegister::Registers[index.data(Qt::UserRole + 1).toInt()].description);
        //else if (index.child())
        //infoLabel->setText(std::to_string(index.data(Qt::UserRole + 1).toInt()).c_str());
        //else
        //    infoLabel->setText(MSRRegister::Registers[index.parent().data(Qt::UserRole + 1).toInt()].values[index.data(Qt::UserRole + 1).toInt()].description.c_str());
    }

    char * getName(const Register &r, const BitDescriptor &bitDescriptor, int offset, int cpu) {
        static char str[200];
        if (r.errors[offset][cpu]) {
            sprintf(str, "ERROR");
            return str;
        }
        auto data = (r.data[offset][cpu] >> bitDescriptor.end) & bitDescriptor.mask;
        switch (bitDescriptor.type) {
            case Type::INT:
            case Type::RESERVED:
                sprintf(str, "%lu", data);
                break;
            case Type::BOOL:
                sprintf(str, "%s", data ? "True" : "False");
                break;
            default:
                sprintf(str, "Unknown type (%i)", r.type);
                break;
        }

        return str;
    }

    bool sameValueOnAllCPU(const Register &reg, int offset) {
        uint64_t old = reg.data[offset][0];
        for (int i = 0; i < width; i++) {
            if (reg.data[offset][i] != old || reg.errors[offset][i]) {
                return false;
            }
            old = reg.data[offset][i];
        }

        return true;
    }

public:
    QRegister(std::vector<T> &registers, unsigned int width) : standardModel(new QStandardItemModel), table(new QTreeView), infoLabel(new QLabel), registers(registers), width(width) {
        infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        infoLabel->setAlignment(Qt::AlignLeft);
        createTable();
        table->setModel(standardModel);
        connect(table->selectionModel(), &QItemSelectionModel::currentChanged, this, &QRegister::selected);
        layout = new QVBoxLayout();
        layout->addWidget(table);
        layout->addWidget(infoLabel);
        this->setLayout(layout);
    }

    void createTable() {
        char str[200];

        for (int regId = 0; regId < registers.size(); regId++) {
            auto &reg = registers[regId];

            QList<QStandardItem *> registerList;

            if (reg.offsets.size() != 1) {
                auto item = new QStandardItem(reg.name);
                item->setEditable(false);
                item->setData(regId);
                registerList.append(item);
                for (int i = 0; i < width; i++) {
                    auto item = new QStandardItem();
                    item->setEditable(false);
                    item->setData(regId);
                    registerList.append(item);
                }
                standardModel->appendRow(registerList);
            }

            for (int offId = 0; offId < reg.offsets.size(); offId++) {
                QList<QStandardItem *> offlist;

                if (reg.offsets.size() == 1) {
                    auto item = new QStandardItem(reg.name);
                    item->setEditable(false);
                    item->setData(regId);
                    registerList.append(item);
                } else {
                    sprintf(str, "Address %i (0x%08lX)", offId + 1, reg.offsets[offId]);
                    auto item = new QStandardItem(str);
                    item->setEditable(false);
                    item->setData(offId);
                    offlist.append(item);
                }

                for (int i = 0; i < width; i++) {
                    auto item = new QStandardItem();
                    item->setEditable(false);
                    item->setData(regId);
                    if (reg.offsets.size() == 1)
                        registerList.append(item);
                    else
                        offlist.append(item);
                }

                if (reg.offsets.size() == 1)
                    standardModel->appendRow(registerList);
                else
                    registerList.first()->appendRow(offlist);

                for (int valId = 0; valId < reg.bitDescriptors.size(); valId++) {
                    auto &val = reg.bitDescriptors[valId];
                    //if (val.type != Type::RESERVED) {
                        QList<QStandardItem *> valuesList;

                        auto item = new QStandardItem(val.name.c_str());

                        item->setEditable(false);
                        item->setEnabled(val.type != Type::RESERVED);
                        item->setData(valId);
                        valuesList.append(item);
                        for (int i = 0; i < width; i++) {
                            item = new QStandardItem();
                            item->setEditable(false);
                            item->setEnabled(val.type != Type::RESERVED);
                            item->setData(valId);
                            valuesList.append(item);
                        }
                        if (reg.offsets.size() == 1)
                            registerList.first()->appendRow(valuesList);
                        else
                            offlist.first()->appendRow(valuesList);
                    //}
                }
            }
        }
    }

    void refresh() {
        char str[200];

        for (int regRow = 0; regRow < standardModel->rowCount(); regRow++) {
            auto regId = standardModel->item(regRow)->data().toInt();
            auto &reg = registers[regId];

            auto regItem = standardModel->item(regRow);

            if (reg.offsets.size() == 1) {
                unsigned int numOfCPU = 1;
                if (!sameValueOnAllCPU(reg, 0))
                    numOfCPU = width;

                for (int regCol = 0; regCol < numOfCPU; regCol++) {
                    if (reg.errors[0][regCol])
                        sprintf(str, "ERROR");
                    else {
                        if (reg.type == Type::REGISTER)
                            sprintf(str, "0x%lX", reg.data[0][regCol]);
                        else if (reg.type == Type::STRING_REGISTER)
                            sprintf(str, "%s", reg.toString().c_str());
                        else
                            sprintf(str, "Unknown type");
                    }
                    standardModel->item(regRow, regCol + 1)->setText(str);
                }

                for (int valRow = 0; valRow < regItem->rowCount(); valRow++) {
                    auto valId = regItem->child(valRow)->data().toInt();
                    auto &bitDescriptor = reg.bitDescriptors[valId];
                    for (int cpu = 0; cpu < numOfCPU; cpu++) {
                        regItem->child(valRow, cpu + 1)->setText(getName(reg, bitDescriptor,0, cpu));
                    }
                }
            } else {
                for (int addrRow = 0; addrRow < regItem->rowCount(); addrRow++) {
                    auto addrId = regItem->child(addrRow)->data().toInt();

                    unsigned int numOfCPU = 1;
                    if (!sameValueOnAllCPU(reg, addrId))
                        numOfCPU = width;

                    auto addrItem = regItem->child(addrRow);
                    for (int regCol = 0; regCol < numOfCPU; regCol++) {
                        if (reg.errors[addrId][regCol])
                            sprintf(str, "ERROR");
                        else
                            sprintf(str, "0x%lX", reg.data[addrId][regCol]);
                        regItem->child(addrRow, regCol + 1)->setText(str);
                    }

                    for (int valRow = 0; valRow < addrItem->rowCount(); valRow++) {
                        auto valId = addrItem->child(valRow)->data().toInt();
                        auto &bitDescriptor = reg.bitDescriptors[valId];
                        for (int cpu = 0; cpu < numOfCPU; cpu++) {
                            addrItem->child(valRow, cpu + 1)->setText(getName(reg, bitDescriptor, addrId, cpu));
                        }
                    }
                }
            }
        }
    }
};