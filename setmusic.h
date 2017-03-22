#ifndef SETMUSIC_H
#define SETMUSIC_H

#include <QDialog>

namespace Ui {
class setmusic;
}

class setmusic : public QDialog
{
    Q_OBJECT

public:
    explicit setmusic(QWidget *parent = 0);
    ~setmusic();

private:
    Ui::setmusic *ui;
};

#endif // SETMUSIC_H
