#include "maindialog.h"
#include "ui_maindialog.h"

#include <QRadioButton>
#include <QDebug>
#include <QCommonStyle>
#include <QMouseEvent>

mainDialog::mainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );

    QCommonStyle style;
    ui->clear_btn->setIcon(style.standardIcon(QStyle::SP_LineEditClearButton));

    ui->available_list->viewport()->installEventFilter(this);

    connect(ui->available_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(chooseScan(QListWidgetItem*)));
    connect(ui->clear_btn, SIGNAL(clicked()), this, SLOT(clearFavorite()));
}

mainDialog::~mainDialog()
{
    delete ui;
}

bool mainDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::RightButton)
        {
            emit openRenameDialog(ui->available_list->currentItem()->text());
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

int mainDialog::findScan(const QString&text)
{
    for (int i=0; i<ui->favorite_list->count(); i++)
    {
        QRadioButton* rb = static_cast<QRadioButton*> (ui->favorite_list->itemWidget(ui->favorite_list->item(i)));
        if (rb->text() == text)
            return i;
    }
    return -1;
}

void mainDialog::clearFavorite()
{
    ui->favorite_list->clear();
    for (int i=0; i<ui->available_list->count(); i++)
    {
        ui->available_list->item(i)->setIcon(QIcon(":/res/pennant.png"));
    }
}

void mainDialog::initAvailableList(const std::vector<std::string>&list)
{
    for (uint i=0; i<list.size(); i++)
    {
        std::string str=list.at(i);
        int row = findScan(QString::fromUtf8(str.c_str(), str.size()));
        if (row < 0)
            new QListWidgetItem(QIcon(":/res/pennant.png"), QString::fromUtf8(str.c_str(), str.size()), ui->available_list);
        else
            new QListWidgetItem(QIcon(":/res/pennant_on.png"), QString::fromUtf8(str.c_str(), str.size()), ui->available_list);
    }
}

void mainDialog::initFavoriteList(const std::vector<std::string>&list, const std::string &favScan)
{
    for (uint i=0; i<list.size(); i++)
    {
        std::string str=list.at(i);
        QListWidgetItem *it = new QListWidgetItem(ui->favorite_list);
        QRadioButton *btn = new QRadioButton(QString::fromUtf8(str.c_str(), str.size()));
        if (str==favScan)
            btn->setChecked(true);
        ui->favorite_list->setItemWidget(it, btn);
    }
}

void mainDialog::saveFavoriteList(std::vector<std::string>&list, std::string &favScan)
{
    for (int i=0; i<ui->favorite_list->count(); i++)
    {
        QRadioButton* rb = static_cast<QRadioButton*> (ui->favorite_list->itemWidget(ui->favorite_list->item(i)));
        list.push_back(rb->text().toStdString());
        if (rb->isChecked())
            favScan = rb->text().toStdString();
    }
}

void mainDialog::saveAvailableList(std::vector<std::string>&list)
{
    for (int i=0; i<ui->available_list->count(); i++)
    {
        list.push_back(ui->available_list->item(i)->text().toStdString());
    }
}
void mainDialog::chooseScan(QListWidgetItem* item)
{
    int row = findScan(item->text());
    if (row < 0)
    {
        QListWidgetItem *it;
        it = new QListWidgetItem(ui->favorite_list);
        ui->favorite_list->setItemWidget(it, new QRadioButton(item->text()));
        if (ui->favorite_list->count()==1)
        {
            QRadioButton *btn = static_cast<QRadioButton*> (ui->favorite_list->itemWidget(ui->favorite_list->item(0)));
            btn->setChecked(true);
        }
        ui->available_list->currentItem()->setIcon(QIcon(":/res/pennant_on.png"));
    }
    else
    {
        ui->favorite_list->takeItem(row);
        ui->available_list->currentItem()->setIcon(QIcon(":/res/pennant.png"));
    }
}

void mainDialog::updateScanList(const QString&name)
{
    for (int i=0; i<ui->favorite_list->count(); i++)
    {
        QRadioButton* rb = static_cast<QRadioButton*> (ui->favorite_list->itemWidget(ui->favorite_list->item(i)));
        if (rb->text()==ui->available_list->currentItem()->text())
            rb->setText(name);
    }
    ui->available_list->currentItem()->setText(name);
}
