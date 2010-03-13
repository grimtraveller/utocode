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
INSERT INTO info(`name`, `info`) VALUES('����', 'wabgem-job@yahoo.coom����һ��������^_^');
INSERT INTO info(`name`, `info`) VALUES('����', '�ٵģ��������ֳ��Ĺ���,��ҹ�����ɸ�����>_<');
INSERT INTO info(`name`, `info`) VALUES('����', '��Ʒ�������Ǹ���ʿ...0_o');
INSERT INTO info(`name`, `info`) VALUES('������', '�ҽ�zhai xiao feng ���� di xiao feng Ҳ�ܲ鵽��...0_o');
INSERT INTO info(`name`, `info`) VALUES('����', '��ϵ�Ұɣ����Ҳ���������û������');
INSERT INTO info(`name`, `info`) VALUES('�����', '�ҵ���ѽ��̫�̼���>_<');
select * from wg_db.info;
