#include "generatexml.h"

#include "util.h"
using namespace boost;
GenerateXML::GenerateXML()
{
    boost::filesystem::path pathXml = GetDataDir() / "data.xml";
}



QString GenerateXML::ReadXml(QString inputTag, QString fileName)
{
    QFile *file = new QFile(fileName);

    if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return "";
    }

    QByteArray xmlData(file->readAll());
    QDomDocument doc("write");
    doc.setContent(xmlData);
    QDomElement root = doc.firstChildElement(inputTag);
    QString value = root.attribute(inputTag);
    return value;
}

void GenerateXML::WriteXML(QString inputTag, QString value, QString fileName)
{
    QFile *file = new QFile(fileName);

    if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        file->open(QIODevice::WriteOnly);
    }
    QByteArray xmlData(file->readAll());
    QDomDocument doc("write");
    doc.setContent(xmlData);
    QDomElement root = doc.firstChildElement(inputTag);
    if(root.isNull())
    {
        QDomNode autoUpdateNode = doc.createElement(inputTag);
        doc.appendChild(autoUpdateNode);
    }
    root = doc.firstChildElement(inputTag);
    root.setAttribute(inputTag, value);
    file->resize(0);
    QTextStream stream(file);
    stream.setDevice(file);
    doc.save(stream, 4);
    file->close();
}
