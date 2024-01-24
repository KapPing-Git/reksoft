#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSql>

#include "aweb_api_model.h"
#include "constants.h"

int create_DB()
{
  // Создаём БД и подключаемся к ней
  QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
  sdb.setDatabaseName(DB_NAME);
  if (!sdb.open())
    {
      std::cerr << "Невозможно проверить наличие БД " << sdb.lastError().text().toStdString() << std::endl;
      return -1;
    }

  // Проверяем существование таблицы БД
  QSqlQuery query{QString{"SELECT COUNT(*) FROM sqlite_master WHERE type= 'table' AND name= '%1'"}.arg(TABLE_NAME)};
  if (query.lastError().type() != QSqlError::NoError)
    {
      std::cerr << "Невозможно проверить наличие БД " << query.lastError().text().toStdString() << std::endl;
      return -1;
    }
  if(query.next())
    {
      // Создаём таблицу БД если она ещё не создана
      if (query.value(0).toInt() == 0)
        {
          query.exec(QString{"CREATE TABLE IF NOT EXISTS %1 "
                    "(id integer primary key, "
                    "API text, "
                    "content text, "
                    "hint text)"}.arg(TABLE_NAME));
          if (query.lastError().type() != QSqlError::NoError)
            {
              std::cerr << "Невозможно создать таблицу БД " << query.lastError().text().toStdString() << std::endl;
              return -1;
            }
        }
//      query.exec("DROP TABLE webapi");
    }
  else
    {
      std::cerr << "При проверке существования БД получен пустой ответ, что то не так с sqlite" << std::endl;
      return -1;
    }
  return 0;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);

  int ret = create_DB();
  if (ret == -1) return -1;

  // регистрация типа
  qmlRegisterType<AWeb_api_model>("web_api_model", 1, 0, "AWeb_api_model");
  engine.load(url);

  return app.exec();
}
