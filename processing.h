/*!
  \file
  \brief Header file for special functions.

  These functions are used to strings process.
 */
#ifndef PROCESSING_H
#define PROCESSING_H

#include <QString>
#include <QStringList>
#include <QVariant>

typedef QVector<QString> Types;
typedef QVector<QVariant> DataRow;
typedef QVector<DataRow> Data;

/*!
 * \brief processingForCsvStr - converts a string to a csv view by adding quotes.
 * \param str - input string
 * \return - result
 */
QString processingForCsvStr(QString str);

/*!
 * \brief whatType - determines the string type: "TEXT","REAL" or "INTEGER"
 * \param str - word for definition
 * \return String with type.
 */
QString whatType(QString str);

/*!
 * \brief parseStr - splits a string from a csv file to column elements, removing extra quotes
 * \param str - input string
 * \return List of elements.
 */
QStringList parseStr(QString str);

/*!
 * \brief withoutQuotes - removes extra quotes
 * \param str - input string
 * \return String without quotes.
 */
QString withoutQuotes(QString str);

#endif // PROCESSING_H
