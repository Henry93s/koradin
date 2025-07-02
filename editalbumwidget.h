#ifndef EDITALBUMWIDGET_H
#define EDITALBUMWIDGET_H

#include <QWidget>

namespace Ui {
class EditAlbumWidget;
}

class EditAlbumWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditAlbumWidget(QWidget *parent = nullptr);
    ~EditAlbumWidget();

private:
    Ui::EditAlbumWidget *ui;
};

#endif // EDITALBUMWIDGET_H
