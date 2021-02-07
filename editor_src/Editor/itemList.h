#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QListWidget>
#include <vector>
#include "item.h"

class ItemList : public QListWidget
{
    Q_OBJECT
public:
    explicit ItemList(QWidget *parent = nullptr);


    static QString editorMimeType() { return QStringLiteral("item"); }

protected:
    void startDrag(Qt::DropActions supportedActions) override;

private:
    void loadImages();
    void loadList();
    int itemSize;
    std::vector<Item> items;
};

#endif // ITEMLIST_H
