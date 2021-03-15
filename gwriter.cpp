#include <QtWidgets>

#include "gwriter.h"
#include "greader.h"

static inline QString yesValue() { return QStringLiteral("yes"); }
static inline QString noValue() { return QStringLiteral("no"); }
static inline QString titleElement() { return QStringLiteral("title"); }

GWriter::GWriter(const QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    xml.setAutoFormatting(true);
}

bool GWriter::writeFile(QIODevice *device)
{
    xml.setDevice(device);

    xml.writeStartDocument();
    xml.writeDTD(QStringLiteral("<!DOCTYPE G>"));
    xml.writeStartElement(QStringLiteral("G"));
    xml.writeAttribute(GReader::versionAttribute(), QStringLiteral("1.0"));
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
        writeItem(treeWidget->topLevelItem(i));

    xml.writeEndDocument();
    return true;
}

void GWriter::writeItem(const QTreeWidgetItem *item)
{
    QString tagName = item->data(0, Qt::UserRole).toString();
    if (tagName.contains(QLatin1String("LEVEL_")))
    {
        bool folded = !treeWidget->isItemExpanded(item);
        xml.writeStartElement(tagName);
        xml.writeAttribute(GReader::foldedAttribute(), folded ? yesValue() : noValue());
        if(item->text(0).contains("horizontal") || item->text(0).contains("vertical"))
        {
			xml.writeAttribute(GReader::sizeAttribute(), item->data(0, Qt::UserRole + 2).toString());
            xml.writeAttribute(GReader::groupAttribute(), item->data(0, Qt::UserRole + 3).toString());
			if (tagName == "LEVEL_1")
			{
				xml.writeAttribute(GReader::positionAttribute(), item->data(0, Qt::UserRole + 1).toString());
			}
        }
        else
        {
            xml.writeAttribute(GReader::groupAttribute(), item->data(0, Qt::UserRole + 3).toString());
        }
        xml.writeTextElement(titleElement(), item->text(0));
        for (int i = 0; i < item->childCount(); ++i)
        {


            writeItem(item->child(i));
        }
        xml.writeEndElement();
    }
//    else if (tagName == QLatin1String("LEVEL_1"))
//    {
//        xml.writeStartElement(tagName);
//        if (!item->text(1).isEmpty())
//            xml.writeAttribute(GReader::hrefAttribute(), item->text(1));
//        xml.writeTextElement(titleElement(), item->text(0));
//        xml.writeEndElement();
//    }
//    else if (tagName == QLatin1String("separator"))
//    {
//        xml.writeEmptyElement(tagName);
//    }
}
