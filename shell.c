#include <stdio.h>
int stringCompare(char*, char*);

int main()
{
  char* line[83];
  int i = 0, check;
  char command[6];
  
  syscall(0,"SHELL>: ");
  syscall(1, line);
  syscall(3, line+5,check);
  return 0;
}

int stringCompare(char* message1, char* message2)
{
  int count;
  for(count = 0; count <= 5;count++)
    {
      if(message1[count] != message2[count])
	{
	  return 0;
	}
      else if(message1[count] == '\0')
	{
	  break;
	}
      else if(message1[count] == '\r')
	{
	  break;
	}
    }
  return 1;
}
