#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QString>

QString text_to_sql(QString text)
{
  return text.replace("'","{{delim}}"); //TODO
}

QString sql_to_text(QString text)
{
  return text.replace("{{delim}}","'");;
}

#endif // FUNCTIONS_H
