AUTOMAKE_OPTIONS=foreign
#编译.cpp文件时的选项
#AM_CPPFLAGS=-fPIC -shared

#编译.c文件时的选项
#AM_CFLAGS=-std=c99 -Wall -fPIC
#AM_CFLAGS=-Wall -fPIC
#链接时选项
#AM_LDFLAGS=-lpthread

#当前目录全局链接库
LIBS=-lpthread

#可执行文件&动态库
#bin_PROGRAMS		编译&安装
#noinst_PROGRAMS	只编译不安装

#静态库
#noinst_LIBRARIES	只编译不安装
#lib_LIBRARIES		编译&安装

COMM_SRC=bclog.c  bctime.c  conf.c  csocket.c  linklist.c  sfline.c  taskdata.c  thpool.c  websocket.c convert_util.c

lib_LIBRARIES=libmauto.a
libmauto_a_SOURCES=$(COMM_SRC)
libmauto_a_CFLAGS=-std=gnu99 -Wall -fPIC

#动态库
libmautodir=$(libdir)
#libmauto_PROGRAMS=libmauto.so
#noinst_PROGRAMS=libmauto.so
libmauto_PROGRAMS=libmauto.so
libmauto_so_SOURCES=$(COMM_SRC)
libmauto_so_CFLAGS=-std=gnu99 -Wall -fPIC -shared
#libmauto_so_LDFLAGS=

#libcomm
noinst_PROGRAMS = libcomm
libcomm_SOURCES=libcomm.c 
libcomm_CFLAGS=-I$(top_srcdir)/libcommons -I/usr/include
libcomm_LDFLAGS= -static
#libcomm_LDFLAGS= -static -static-libgcc
#libcomm_LDFLAGS=-L$(top_srcdir)/libcommons
#libcom_LDFLAGS=-static -std=c99 -L$(top_srcdir)/libcommons
#libcomm_LDADD=$(top_srcdir)/libcommons/libmauto.a -lmauto
libcomm_LDADD=libmauto.a
#libcomm_LDADD=-lmauto

#安装头文件
include_HEADERS=*.h
EXTRA_DIST=*.h
