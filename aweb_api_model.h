#ifndef AWEB_API_MODEL_H
#define AWEB_API_MODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class AWeb_api_model : public QAbstractListModel
{
  Q_OBJECT
public:
  AWeb_api_model(QObject *parent = nullptr);

  Q_INVOKABLE QString myText() {return "my text"; }
  Q_INVOKABLE void indexChanged(int currentIndex);
  Q_INVOKABLE void apply_hint(QString hint);

private:
  QNetworkAccessManager *m_manager = new QNetworkAccessManager(this);
  int m_curr_id {0};


void replyFinished(QNetworkReply *reply);

signals:
Q_INVOKABLE void curr_api_changed(QString content, QString hint);

  // QAbstractItemModel interface
public:
  Q_INVOKABLE virtual int rowCount(const QModelIndex &) const override;
  Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int) const override;
  Q_INVOKABLE virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // AWEB_API_MODEL_H
