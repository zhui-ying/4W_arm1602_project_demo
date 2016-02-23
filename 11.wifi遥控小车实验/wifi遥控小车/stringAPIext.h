#ifndef __STRINGAPI_H__
#define __STRINGAPI_H__

//查找子串sub在str的位置并返回，没有返回-1
int StringIndex(const char *str,const char *sub);
//返回字符串长度
int StringLenth(const char *str);
//对字符串str2的start位置到end位置进行裁剪并将值赋给str1,裁剪失败返回-1
int StringSubstring(char *str1 ,const char *str2, int start , int end);
//将一个字符加到str的尾部，注意str的大小，防止溢出
void StringAddchar(char *str,const char add_data);
//在str向后偏移offset中查找一个字符
int StringIndexCharOffset(const char *str,const char sub, unsigned int offset);


#endif

