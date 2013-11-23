#ifndef QURAN_READER_H
#define QURAN_READER_H

#include <QWidget>

#include "core/quran/chapter.h"

namespace Ui {
class QuranReader;
}

namespace data {
class DataHolder;
}

class QuranReader : public QWidget
{
    Q_OBJECT
    
public:
    QuranReader(data::DataHolder* dataHolder, QWidget *parent = 0);
    ~QuranReader();
    
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
private:
    Ui::QuranReader *ui;
    data::DataHolder* m_dataHolder;
};

#endif // QURAN_READER_H
