#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "tablemodel.h"
#include "processing.h"
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QTranslator>

class Contractor
{
    //Q_OBJECT
public:
    Contractor();

    void openCSV();
    void openSQL();

    void convertToSQL();
    void convertToCSV();

    TableModel* getModel();

private:

    TableModel *model;
};

#endif // CONTRACTOR_H
