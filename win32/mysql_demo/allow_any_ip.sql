use mysql ;
update user set host = '%' where user = 'root' and host <> 'localhost';
flush privileges;
/*ֱ�Ӹ�Ȩ��ʽ*/
grant all on *.* to root@'%' identified by 'password';

