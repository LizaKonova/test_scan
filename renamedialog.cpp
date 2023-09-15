#include "renamedialog.h"
#include "ui_renamedialog.h"

renameDialog::renameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::renameDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );
}

renameDialog::~renameDialog()
{
    delete ui;
}

void renameDialog::showName(const QString&name)
{
    ui->name_edit->setText(name);
    show();
}

void renameDialog::rename()
{
    emit renameScan(ui->name_edit->text());
    hide();
}
