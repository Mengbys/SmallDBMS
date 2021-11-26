# SmallDBMS
## 介绍
SmallDBMS是一个极其简单的DBMS（数据库管理系统），或许可以把它理解成幼儿版的mysql？这个是《高级数据库技术》课程的大作业。老师推荐用java+javacc来实现，可我对java一窍不通。老师上课的时候提了一嘴说：“用C也可以，但要用到yacc和lex。”我实在不想从头再学一门语言（指java）了，所以一下课我就回去找相关的资料，看看能不能搞。查了之后发现新版本的yacc和lex变成了bison和flex，而且也有对C++的支持。那，试试看呗。试试就逝世，十天就这么没了。最终的exe文件可以从[这里](https://github.com/Mengbys/SmallDBMS/releases/tag/v1.0)下载。
## 平台
windows10
## 需要的工具
- g++
- make
- bison(>3.0.0)
- flex
- boost库 
## 编译
编译安装boost之后，将makefile中的COMPILEOPT_BOOST_INC变量改为你对应的目录。然后命令行中进入该文件夹，输入以下命令生成二进制文件:

`$ mingw32-make`

也可以输入以下命令来删除生成的文件：

`$ mingw32-make clean`
## 运行
直接双击运行或在命令行下运行smalldbms.exe即可。需要注意的是，首次运行会在当前工作目录（双击运行的当前工作目录就是exe文件所在的文件夹，命令行下运行的则是命令行中的当前目录）下创建名为“SmallDBMS-place-of-database”的文件夹。程序创建的文件都将放在这个文件夹中。进入程序之后，就可以输入sql语句了。
## SmallDBMS的局限性
这东西真的不行。它只支持下面这些sql语句（注意别忘了分号啊）。

__数据库相关操作__：
- create database DATABASENAME;
- use DATABASENAME;
- drop database DATABASENAME;
- show databases;

__表相关操作__：
- create table 只支持int和varchar两种数据类型；只支持主码约束这一种列级约束，不支持表级约束。

eg:
```sql
create table fish
(
id int primary key,
name varchar,
sex varchar,
...
);
```
- drop table TABLENAME;
- show tables;

__记录相关操作__：
- insert into

eg1:
```sql
insert into fish
values(3,lucy,female);
```
eg2:
```sql
insert into fish(id,name)
values(5,ball);
```
- update

eg:
```sql
update fish
set name=mike,sex=male
where id=6;
```
- delete

eg1:
```
delete * from fish;
```
eg2:
```sql
delete from fish
where name=jane;
```
- select 支持单表查询，也只支持等值过滤。

eg1:
```sql
select * from fish;
```
eg2:
```sql
select id,name from fish where sex=female;
```
## 相关文件说明
- ./include/FlexLexer.h

在linux中安装flex后，从linux平台拷过来的。没办法bison、flex本来就是linux下的工具。
- ./lib/libboost_serialization-mgw8-mt-x64-1_77.a

boost编译后从其lib文件夹下拷过来的，链接时用，需要它里面的序列化函数。
- ./win_flex_bison3-latest

windows下可用的bison和flex的二进制文件，从[这里](https://sourceforge.net/projects/winflexbison/files/)下载来的。
## 参考资料
- [mingw下载](https://sourceforge.net/projects/mingw-w64/files/)
- [boost下载](https://www.boost.org/)
- [boost序列化文档](http://zh.highscore.de/cpp/boost/serialization.html#serialization_archive)
- [bison文档](https://www.gnu.org/software/bison/manual/bison.html)
- [flex文档](https://westes.github.io/flex/manual/)
- <https://www.cnblogs.com/hdk1993/p/4922866.html>
- <https://github.com/jonathan-beard/simple_wc_example>
## ps
累了。为了写这个东西旷了4节课，有一次实验还没交，最后也只能达到这种效果。就这样交了吧。代码写的奇烂，主函数已经堆了500行了。如果运行出现了bug，再正常不过了，非常欢迎向我反馈，不过我可能没什么精力去改了。还有，如果你输入的sql语句无法解析，那是因为SmallDBMS还没实现相对应的功能，哈哈。代码仅供学习交流使用。欢迎有能大佬增加新的功能和特性。_(´ཀ`」 ∠)_
## 运行预览
![running](https://github.com/Mengbys/SmallDBMS/blob/main/screenshot/2.png)
