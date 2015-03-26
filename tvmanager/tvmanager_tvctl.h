#ifndef	TVMANAGER_TVCTL_H
#define	TVMANAGER_TVCTL_H

struct g_tvctl_ctl{
	FILE *log;
}g_tvctl;


//初始化控制点
int init_tvctl(void);
//控制点收尾工作
int fini_tvctl(void);
#endif
