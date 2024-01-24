Для того чтобы работала загрузка из интернета необходима библиотека openssl-1.1.1w 
ссылка для скачивания https://www.openssl.org/source/openssl-3.2.0.tar.gz
Распакуйте её, зайдите в распакованную папку и выполните:
./config
make
в pro файле в строке LIBS += -L/home/kap/extern_sources/openssl-1.1.1w -lcrypto -L/home/kap/extern_sources/openssl-1.1.1w -lssl
замените home/kap/extern_sources на вашу папку
