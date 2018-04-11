#include "newspages.h"

newsPages::newsPages(QWidget *parent) : QWidget(parent)
{
    pageLeft.setText('<');
    pageRight.setText('>');
    pageLayout.addWidget(pageLeft);
    pageLayout.addWidget(pageBox);
    pageLayout.addWidget(pageRight);
    this->setLayout(pageLayout)
}

newsPages::on_left_clicked(){

}
