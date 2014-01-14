#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>

class QPaintEvent;
class QTimer;

class Clock : public QWidget
{
    Q_OBJECT
public:
    static const QColor kHourColor;
    static const QColor kMinuteColor;
    static const QColor kTextColor;
    
    explicit Clock(QWidget *parent = 0);
    ~Clock();
    void liveClock();
    void resize(int);
    void paintEvent(QPaintEvent *);
    void setTime(int h, int m, int s = 0);
    void setDisplayTextualTime(bool);
    void setTitle(const QString&);
    void select();
    void deselect();
signals:
    
public slots:
protected:
    int m_h;
    int m_m;
    int m_s;
    bool m_live;
    QTimer* m_liveTimer;
    bool m_displayTextualTime;
    QString m_title;
};

#endif // CLOCK_H
