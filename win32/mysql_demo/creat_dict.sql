/*
 * Name:		creat_dict.sql
 * Descr:		create dict table for pinyin to chinese.
				there is a table of storing user name and information
 * usage:		NOTICE!!!!
				this sql script will delete dict table in wg_db database!!!!!!
				copy this file to mysql/bin directory
				mysql>source creat_env.sql
 * Author:		zuohaitao
 * Date:		2010-03-11
 */
SET NAMES gb2312;
USE `wg_db`;
DROP TABLE IF EXISTS `dict`;
CREATE TABLE `dict`
(
id INT NOT NULL AUTO_INCREMENT,
py_char VARCHAR(100) character set gb2312 NOT NULL default '',
cn_char VARCHAR(100) character set gb2312 NOT NULL,
PRIMARY KEY (`id`)
);
INSERT INTO dict(`py_char`, `cn_char`) VALUES('wang', 'Íõ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('gang', '¸Õ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('gang', '¸Ú');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('gang', '¸Ö');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('zhai', 'µÔ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('di', 'µÔ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('xiao', 'Ïþ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('feng', '·å');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('zuo', '×ó');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('hai', 'º£');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('tao', 'ÌÎ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('zuo', '×ô');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('hai', 'º¢');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('tao', 'ÌÏ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('zhang', 'ÕÅ');
INSERT INTO dict(`py_char`, `cn_char`) VALUES('zhang', 'ÕÂ');
select * from wg_db.dict;
