#include "maindialog.h"
#include "renamedialog.h"
#include <QApplication>
#include <QSettings>

std::string parser(const std::string &str)
{
    std::string str_ = str;
    std::string name = "";
    std::string address = "";
    for (int i=0; i<4; i++)
    {
        const size_t posBeg = str_.find("["), posEnd = str_.find("]");
        if (posBeg != std::string::npos && posEnd != std::string::npos)
        {
            if (i==0)
            {
                const size_t posBeg_ip = str_.find("//");
                if (posBeg_ip != std::string::npos)
                    address = str_.substr(posBeg_ip+2, posEnd - posBeg_ip - 2);
            }
            if (i==2)
                name = str_.substr(posBeg+1, posEnd - posBeg - 1);
            str_ = str_.substr(posEnd+1, str.size());
        }
    }
    if (address !="" && name!="")
        return name+ " [" + address + "]";
    else
        return "";

}

int main(int argc, char *argv[])
{
    //! Список доступных сканеров.

    std::vector<std::string> scannersList;

    //! Набор имен для теста диалога.

    scannersList.push_back("[escl:https://192.168.77.235:9096] [ESCL] [Kyocera ECOSYS M5526cdw111] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.66.235:9096] [ESCL] [Cannon ECOSYS M5526cdw222] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.77.21:9096] [ESCL] [Plustek OpticSlim 2610 Pro] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.66.23:9096] [ESCL] [Epson Perfection V19] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.77.25:9096] [ESCL] [HP ScanJet Pro 2000 s2] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.66.35:9096] [ESCL] [Canon CanoScan LiDE 400 2996C010] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.77.205:9096] [ESCL] [Epson WorkForce DS-7500] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.66.203:9096] [ESCL] [Canon/Duplex Color Image Reader Unit-M2] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.77.123:9096] [ESCL] [Kyocera ECOSYS M5526cdw333] [flatbed scanner]");

    scannersList.push_back("[escl:https://192.168.66.135:9096] [ESCL] [Cannon ECOSYS M5526cdw] [flatbed scanner]");

    QApplication a(argc, argv);
    mainDialog w;
    renameDialog r;

    QObject::connect (&w, &mainDialog::openRenameDialog, &r, &renameDialog::showName);
    QObject::connect (&r, &renameDialog::renameScan, &w, &mainDialog::updateScanList);

    QSettings settings( "settings.conf", QSettings::IniFormat );

    settings.beginGroup( "WidgetPosition" );
    int x = settings.value( "x", -1 ).toInt();
    int y = settings.value( "y", -1 ).toInt();
    int width = settings.value( "width", -1 ).toInt();
    int height = settings.value( "height", -1 ).toInt();
    settings.endGroup();

    if( x > 0 && y > 0 && width > 0 && height > 0 )
    {
        w.setGeometry( x, y, width, height );
    }

    std::vector<std::string> saveList;
    std::string saveScan;

    int size = settings.beginReadArray("FavoriteScanners");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        saveList.push_back(settings.value("scan").toString().toStdString());
    }
    settings.endArray();
    settings.beginGroup("MainFavorite");
    saveScan = settings.value("favorite").toString().toStdString();
    settings.endGroup();
    w.initFavoriteList(saveList, saveScan);

    std::vector<std::string> saveAvailableList;

    int size_ = settings.beginReadArray("AvailableScanners");
    for (int i = 0; i < size_; ++i)
    {
        settings.setArrayIndex(i);
        saveAvailableList.push_back(settings.value("comment").toString().toStdString());
    }
    settings.endArray();

    if (saveAvailableList.size()==0)
        for (uint i = 0; i < scannersList.size(); ++i)
        {
            saveAvailableList.push_back(parser(scannersList.at(i)));
        }

    w.initAvailableList(saveAvailableList);

    w.show();

    int res = a.exec();

    settings.beginGroup( "WidgetPosition" );
    settings.setValue( "x", w.x() );
    settings.setValue( "y", w.y() );
    settings.setValue( "width", w.width() );
    settings.setValue( "height", w.height() );
    settings.endGroup();

    std::vector<std::string> favList;
    std::string favScan;
    w.saveFavoriteList(favList, favScan);
    std::vector<std::string> availList;
    w.saveAvailableList(availList);

    settings.beginWriteArray("FavoriteScanners");
    for (uint i=0; i<favList.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("scan", QString::fromStdString(favList.at(i)) );
    }
    settings.endArray();

    settings.beginWriteArray("AvailableScanners");
    for (uint i=0; i<availList.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("comment", QString::fromStdString(availList.at(i)) );
    }
    settings.endArray();

    settings.beginGroup("MainFavorite");
    settings.setValue("favorite", QString::fromStdString(favScan));
    settings.endGroup();

    return res;
}
