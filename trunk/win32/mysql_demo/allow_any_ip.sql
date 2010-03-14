use mysql ;
update user set host = '%' where user = 'root' and host <> 'localhost';
flush privileges;
/*直接赋权方式*/
grant all on *.* to root@'%' identified by 'password';

