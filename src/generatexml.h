#ifndef GENERATEXML_H
#define GENERATEXML_H

#include <QFile>
#include <QMap>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QMapIterator>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QDomComment>

class GenerateXML
{
public:
    GenerateXML();
    //void CreateXMLFile(QString Root, QMap Part);
    QString ReadXml(QString inputTag, QString fileName);
    void WriteXML(QString inputTag, QString value, QString fileName);
private:
    QFile *file;
    QString Root;
};
#endif // GENERATEXML_H
