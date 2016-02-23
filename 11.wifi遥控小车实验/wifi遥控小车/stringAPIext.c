//stringAPI
//扩展字符串功能
#include "stringAPIext.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

//查找sub 在str 的位置，没有返回-1
int StringIndex(const char *str,const char *sub)
{
	char *pos;
	pos = strstr(str, sub);
	if(NULL == pos)
	{
		return -1;
	}else
	{
		return pos - str;
	}
}

//get string length
int StringLenth(const char *str)
{
	return strlen(str);
}

//截取str2的子串到str1,注意溢出，截取失败返回-1
int StringSubstring(char *str1 ,const char *str2, int start , int end)
{
//	char *buf;
	if(start > end)
	{
		return -1;
	}
	if(end > strlen(str2))
	{
		return -1;
	}
	//buf = str2+start;
	strncpy(str1,str2+start,end - start+1);
	str1[end - start+1] = '\0';
	return 1;
}

//在字符串后面增加字符,注意溢出
void StringAddchar(char *str,const char add_data)
{
	int length = strlen(str);
	str[length] = add_data;
	str[length+1] = '\0';
}

//在str中查找sub第一次出现的位置，offset偏移位置
int StringIndexCharOffset(const char *str,const char sub, unsigned int offset)
{
//	char *buf;
	char *pos;
	if(offset > strlen(str)) return -1;
//	buf = str + offset;
	pos = strchr(str + offset,sub);
	if(pos == NULL) return -1;
	return pos - str;
}

