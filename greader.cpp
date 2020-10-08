#include <QtWidgets>

#include "greader.h"

GReader::GReader(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    QStyle *style = treeWidget->style();

    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style->standardPixmap(QStyle::SP_FileIcon));
}

bool GReader::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("G")
            && xml.attributes().value(versionAttribute()) == QLatin1String("1.0")) {
            readG();
        } else {
            xml.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
        }
    }

    return !xml.error();
}

QString GReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}

void GReader::readG()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("G"));

    while (xml.readNextStartElement()) {
		qDebug() << xml.name() << endl;
        if (xml.name().contains(QLatin1String("LEVEL_")))
            readFolder(0);
        else if (xml.name() == QLatin1String("bookmark"))
            readBookmark(0);
        else if (xml.name() == QLatin1String("separator"))
            readSeparator(0);
        else
            xml.skipCurrentElement();
    }
}

void GReader::readTitle(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("title"));

    QString title = xml.readElementText();
    item->setText(0, title);
}

void GReader::readSeparator(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("separator"));

    QTreeWidgetItem *separator = createChildItem(item);
    separator->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    separator->setText(0, QString(30, 0xB7));
    xml.skipCurrentElement();
}

void GReader::readFolder(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name().contains(QLatin1String("LEVEL_")));

    QTreeWidgetItem *folder = createChildItem(item);
	QString positionInfo = xml.attributes().value(positionAttribute()).toString();
    if(positionInfo != "")
    {
		folder->setData(0, Qt::UserRole + 1, xml.attributes().value(positionAttribute()).toString());
    }
	QString sizeInfo = xml.attributes().value(sizeAttribute()).toString();
	if (sizeInfo != "")
	{
		folder->setData(0, Qt::UserRole + 2, xml.attributes().value(sizeAttribute()).toString());
	}
    
    bool folded = (xml.attributes().value(foldedAttribute()) != QLatin1String("no"));
    treeWidget->setItemExpanded(folder, !folded);

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("title"))
            readTitle(folder);
        else if (xml.name().contains(QLatin1String("LEVEL_")))
            readFolder(folder);
        else if (xml.name() == QLatin1String("bookmark"))
            readBookmark(folder);
        else if (xml.name() == QLatin1String("separator"))
            readSeparator(folder);
        else
            xml.skipCurrentElement();
    }
}

void GReader::readBookmark(QTreeWidgetItem *item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("bookmark"));

    QTreeWidgetItem *bookmark = createChildItem(item);
    bookmark->setFlags(bookmark->flags() | Qt::ItemIsEditable);
    bookmark->setIcon(0, bookmarkIcon);
    bookmark->setText(0, QObject::tr("Unknown title"));
    bookmark->setText(1, xml.attributes().value(hrefAttribute()).toString());

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("title"))
            readTitle(bookmark);
        else
            xml.skipCurrentElement();
    }
}

QTreeWidgetItem *GReader::createChildItem(QTreeWidgetItem *item)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, xml.name().toString());
    return childItem;
}
