[需求]
见附一
[设计]
0 建立软件开发环境
 0.1 安装mysql数据库
	http://mysql.ntu.edu.tw/Downloads/MySQLGUITools/mysql-workbench-oss-5.2.16-beta-win32.msi
	http://mysql.ntu.edu.tw/Downloads/MySQL-5.1/mysql-essential-5.1.44-win32.msi
 0.2 配置开发工具环境
	增加mysql头文件目录到集成开发环境中
	VS2008中 工具->选项->项目和解决方案->VC++目录
	<alt>+<t>->o

1 建立数据库的模拟环境
 import mysql_demo/creat_evn.sql
	
2 建立拼音汉字对照表
 import mysql_demo/creat_dict.sql
3 程序流程
                   +----------------------+
                   |user input pinyin name|
                   +-----------.----------+
                               |
                      +----------------+
                      |connect database|
                      +--------.-------+
                               |
                               |
               +---------------'---------------+
               |get chinese from dict database |
               |combian chese to name          |
               |                               |
               +--------------+----------------+
                              |
               +--------------'---------------+
               |get user infomation from info |
               |print all infomations         |
               +--------------+---------------+
                              |
                       +------'-------+
                       |close database|
                       +--------------+

3.1 user input pinyin
e.g. wang gang
read the data from database
[select cn_char from wg_db.dict where py_char='wang']
[select py_char from wg_db.dict where cn_char='gang']
store data in vectors
auto make all names
[select * from wg_db.info where name='王刚' or name='王岗' or name='王钢']
printf infomations
4 todo
//TODO:栈上申请的数组过大，可适当缩小变为合适的大小
//TODO:关于char sql[1000]数组，动态申请更为合适
//TODO:对算法进行优化
//TODO:检查内存泄漏
//TODO:构造具有特点的不同的数据，对程序健壮性进行测试



-------------我是分割线--------------------------------------
附一
数据库中有姓名的信息，查询姓名时，有时只知道姓名的读音，不知道确切是哪几个字，所以就有同音字查询的需求，也就是输入拼音读法相同的，系统自动把读音相同的找出来。例如，用 “王刚”查询，应该把所有叫 “王刚”，“王钢”等的都查出来，如果不考虑音调，还有“王岗”也应该出来。
现在请你设计一个算法或者方案，解决这个问题。注意以下事项：
1.姓名是保存在数据库中的，最好能够方便使用SQL语句来进行查询。存储也要考虑方便性。最好不要保存多个音，因为字的个数是变化的，这样可能字段的个数不好定。
2.考虑多音字问题，有两种情况：
A，姓名本身中有多音字，例如名字 翟晓峰 中“翟”就是多音字
B，用来查询的字中有多音字 ，例如用 “翟晓峰” 去查询时，“翟”是多音字
 
又例如 用“西脏”应该能够查出“西藏”，用“西仓”应该也能够查出“西藏”，用“西藏”应该 能够查出所有音是 xicang 或者 xizang的
“藏行",应该是查询 zanghang ,zangxing,canghang,cangxing 四种。
 
请写出算法或者演示程序，数据库可以选择access ，mysql，sqllite，oracle等都可以，可以只准备测试数据即可，不用把整个字典都做了。
 
需求不清楚，可以和我联系。如果能完成，就可以过来面试了。
优化：
字段字节数都设置的比较大，可以根据具体需求重新修改字段字节数

