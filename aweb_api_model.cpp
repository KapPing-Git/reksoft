#include "aweb_api_model.h"

#include <iostream>

#include <QtSql>

#include "constants.h"
#include "functions.h"

AWeb_api_model::AWeb_api_model(QObject *parent) : QAbstractListModel(parent)
{
  // проверяем заполненность таблицы БД
  QSqlQuery query{QString{"SELECT COUNT(*) FROM %1"}.arg(TABLE_NAME)};
  if (query.lastError().type() == QSqlError::NoError)
    {
      query.next();
      if (query.value(0).toInt() == 0) // если БД пуста
        {
          // Скачиваем данные из сети
          connect(m_manager, &QNetworkAccessManager::finished, this, &AWeb_api_model::replyFinished);
          m_manager->get(QNetworkRequest(QUrl(API_SITE)));
        }
    }
  else
    {
      std::cerr << "" << query.lastError().text().toStdString() << std::endl;
    }
}

void AWeb_api_model::indexChanged(int currentIndex)
{
  m_curr_id = currentIndex;
  QSqlQuery query{QString{"SELECT * FROM %1 WHERE id=%2"}.arg(TABLE_NAME).arg(currentIndex+1)};
  query.next();
  QJsonDocument curr_document = QJsonDocument::fromJson(query.value(2).toByteArray());
  QString content = curr_document.toJson(QJsonDocument::Indented);
  curr_api_changed(content, query.value(3).toString());
}

void AWeb_api_model::apply_hint(QString hint)
{
  QSqlQuery query{QString{"UPDATE %1 SET hint='%3' WHERE id=%2"}.arg(TABLE_NAME).arg(m_curr_id+1).arg(hint)};
  qDebug() << query.lastError().text();
}

void AWeb_api_model::replyFinished(QNetworkReply *reply)
{
  if (reply->error() == QNetworkReply::NoError)
    {
      const QByteArray reply_data = reply->readAll();
      QJsonDocument curr_document = QJsonDocument::fromJson(reply_data);
      size_t count = curr_document.object()["count"].toInt();
      QJsonArray elements = curr_document.object()["entries"].toArray();


      QString queryString("INSERT INTO  %1 "
                          "(API,content,hint) "
                          "VALUES ");
      queryString = queryString.arg(TABLE_NAME);
      QString part{"('%1','%2',''),"};
      for (const auto elem : elements)
        {
          QJsonDocument doc{elem.toObject()};
          QString json_text{doc.toJson(QJsonDocument::Compact)};
          queryString.append(part.arg(text_to_sql(elem.toObject()["API"].toString()), text_to_sql(json_text)));
        }
      queryString.resize(queryString.size()-1);
      QSqlQuery query(queryString);
      if (query.lastError().type() != QSqlError::NoError)
        std::cerr << query.lastError().text().toStdString() << std::endl;

      beginInsertRows(QModelIndex(), 0, count);
      endInsertRows();
    }
  else
    std::cerr << reply->errorString().toStdString() << std::endl;

}


int AWeb_api_model::rowCount(const QModelIndex &/*parent*/) const
{
  QSqlQuery query{QString{"SELECT COUNT(*) FROM %1"}.arg(TABLE_NAME)};
  query.next();
  return query.value(0).toInt();
}

QVariant AWeb_api_model::data(const QModelIndex &index, int /*role*/) const
{
  QSqlQuery query{QString{"SELECT API FROM %1 WHERE id=%2"}.arg(TABLE_NAME).arg(index.row()+1)};
  if (query.next())
    {
      return query.value(0).toString();
    }
  else
    return "";
}


QHash<int, QByteArray> AWeb_api_model::roleNames() const
{
  QHash<int, QByteArray> roleNames;
  roleNames[0] = QByteArray{"only_role"};
  return roleNames;
}
