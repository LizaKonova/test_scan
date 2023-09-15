#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class renameDialog;
}

class renameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit renameDialog(QWidget *parent = 0);
    ~renameDialog();

public slots:
    void showName(const QString&name);

private slots:
    void rename();

private:
    Ui::renameDialog *ui;

signals:
    void renameScan(const QString&name);
};

#endif // RENAMEDIALOG_H
