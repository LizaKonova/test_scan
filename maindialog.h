#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class mainDialog;
}

class mainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mainDialog(QWidget *parent = 0);
    ~mainDialog();

    void initAvailableList(const std::vector<std::string>&list);
    void initFavoriteList(const std::vector<std::string>&list, const std::string &favScan);
    void saveFavoriteList(std::vector<std::string>&list, std::string &favScan);
    void saveAvailableList(std::vector<std::string>&list);

private:
    Ui::mainDialog *ui;

    bool eventFilter(QObject *obj, QEvent *event);
 //   std::string parser(const std::string &str);
    int findScan(const QString&text);

private slots:
    void chooseScan(QListWidgetItem* item);
    void clearFavorite();

public slots:
    void updateScanList(const QString&name);

signals:
    void openRenameDialog(const QString&name);

};

#endif // MAINDIALOG_H
