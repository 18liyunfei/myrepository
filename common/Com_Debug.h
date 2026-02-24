#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include"usart.h"
#include"stdio.h"
#include<string.h>
#include<stdarg.h>


void Com_Debug_Init(void);

#define DEBUG_ENABLE





#ifdef DEBUG_ENABLE

  #define FILE__NAME__  ((strrchr(__FILE__, '\\')) ? strrchr(__FILE__, '\\') + 1 : __FILE__)//用“”会出现类型报错
  #define __FILE__NAME__  ((strrchr(FILE__NAME__,'/'))?strrchr(FILE__NAME__,'/')+1:FILE__NAME__)
  /* Com_Debug.h  只改这一行 */
#define DEBUG_PRINTF(format,...) printf("[%s:%d] " format "\r\n", __FILE__NAME__, __LINE__,  ##__VA_ARGS__)



  #else
    #define DEBUG_PRINTF()
    
#endif  






#endif /* __COM_DEBUG_H__ */





