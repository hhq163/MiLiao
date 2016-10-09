[TOC]

###NewMessage v0.2

####新特性：
解决NM框架上一版本V0.1版本服务端架构以下问题：   
1.消息业务应该单独使用NoSql数据库进行存储，目前的分表策略会导致没法扩展；
2.很多非消息请求走TCP，导致内部模块交互增加；
3.注册响应时间太久（调环信接口注册）；
4.消息收发机制，可能导致重复收到消息，每一个消息都要发送ACK请求，表明客户端收到消息，增加系统负荷；
5.群的通知消息都未处理；
6.单聊、群聊都进行广播，造成通道拥堵；实际上都不需要广播;
7.业务逻辑不清晰，缓存未用好；


####安装配置

第三方类库安装：
1、yum install uuid uuid-dev libuuid-devel

2、安装hiredis客户端
文件在/doc中，执行./make_hiredis.sh即可；

3、安装Protobuf
文件在/doc中，执行./make_protobuf.sh即可；

4、安装mongoDB c drive:

1.下载  
wget https://github.com/mongodb/libbson/releases/download/1.3.5/libbson-1.3.5.tar.gz  
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.3.5/mongo-c-driver-1.3.5.tar.gz

2.安装libbson  
tar -xzvf libbson-1.3.5.tar.gz  
cd libbson-1.3.5/  
./configure --prefix=/usr --libdir=/usr/lib64  
make  
make install  
cd ../
    
3.安装mongodb c driver：   
yum install pkg-config openssl-devel cyrus-sasl-devel  
tar xzvf mongo-c-driver-1.3.5.tar.gz  
cd mongo-c-driver-1.3.5  
./configure  
make  
make install  
cd ../  
    
安装位置为/usr/local/lib/libmongoc-1.0.so
