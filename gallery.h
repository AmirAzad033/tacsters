#ifndef GALLERY_H
#define GALLERY_H

#include <QWidget>

namespace Ui {
class Gallery;
}

class Gallery : public QWidget
{
    Q_OBJECT

public:
    explicit Gallery(QWidget *parent = nullptr);
    ~Gallery();

private:
    Ui::Gallery *ui;
};

#endif // GALLERY_H
