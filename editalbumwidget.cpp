#include "editalbumwidget.h"
#include "ui_editalbumwidget.h"

EditAlbumWidget::EditAlbumWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditAlbumWidget)
{
    ui->setupUi(this);
}

EditAlbumWidget::~EditAlbumWidget()
{
    delete ui;
}
