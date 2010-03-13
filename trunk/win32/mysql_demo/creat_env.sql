/*
 * Name:		creat_env.sql
 * Descr:		create simulate a real environment.
				there is a table of storing user name and information
 * usage:		NOTICE!!!!
				this sql script will delete wg_db database!!!!!!
				copy this file to mysql/bin directory
				mysql>source creat_env.sql
 * Author:		zuohaitao
 * Date:		2010-03-11
 */
SET NAMES gb2312;
DROP DATABASE IF EXISTS `wg_db`;
CREATE DATABASE `wg_db`;
USE `wg_db`;
DROP TABLE IF EXISTS `info`;
CREATE TABLE `info`
(
id INT NOT NULL AUTO_INCREMENT,
name VARCHAR(100) character set gb2312 NOT NULL default '',
info TEXT(2) character set gb2312 NOT NULL,
PRIMARY KEY (`id`)
);
INSERT INTO info(`name`, `info`) VALUES('王刚', 'wabgem-job@yahoo.coom这是一个真王刚^_^');
INSERT INTO info(`name`, `info`) VALUES('王钢', '假的！我是炼钢厂的工人,在夜会变身成钢铁侠>_<');
INSERT INTO info(`name`, `info`) VALUES('王岗', '赝品！！我是个烈士...0_o');
INSERT INTO info(`name`, `info`) VALUES('翟晓峰', '我叫zhai xiao feng 搜索 di xiao feng 也能查到我...0_o');
INSERT INTO info(`name`, `info`) VALUES('左海涛', '联系我吧，再找不到工作就没饭吃了');
INSERT INTO info(`name`, `info`) VALUES('猪火腿', '我地妈呀，太刺激了>_<');
select * from wg_db.info;
