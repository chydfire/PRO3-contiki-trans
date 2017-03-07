

说明：此工程文件可以成功用于基于51单片机SN8F5708的A7139收发测试实验。
注意：配置参数   CONFIG_DATA_RATE  设为  100

CONFIG_DATA_RATE  设为  5，接收到的数据为乱码；
CONFIG_DATA_RATE  设为 10，有时候可以接收到数据，有时候不能接收到数据，接收到的数据有时候正确，有时候为乱码；
CONFIG_DATA_RATE  设为 100，正确接收到数据。