/* Ian Calhoun
   COMP 350-003
   10/24/2019 */
 

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void readFile(char*,char*,int);

int main()
{
  char buffer[13312];
  int sectorsRead;
  makeInterrupt21();
  interrupt(0x21,3,"messag",buffer,&sectorsRead);
  if(sectorsRead>0)
    interrupt(0x21, 0, buffer, 0, 0);
  else
    interrupt(0x21,0,"messag not found\r\n",0,0);
  while(1);
}

void printString(char* chars)
{
  while(*chars != 0x0)
    {
      interrupt(0x10, 0xe*256+*chars, 0, 0, 0);
      chars++;
    }
}


void printChar(char c)
{
  interrupt(0x10, 0xe*256+c, 0, 0, 0);
}


void readString(char* line)
{
  int count = 0;
  char temp;

  while(1)
    {
      temp = interrupt(0x16, 0, 0, 0, 0);
      if((temp != 0xd)&&(temp != 0x8))
	{
	  interrupt(0x10, 0xe*256+temp, 0, 0, 0);
	  line[count] = temp;
	  count++;
	}
      else if(temp == 0x8)
	{
	  if(count > 0)
	    {
	      interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
	      count--;
	      line[count] = 0x0;
	      interrupt(0x10, 0xe*256+' ', 0, 0, 0);
	      interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
	    }
	}
      else
	{
	  line[count+1] = 0xa;
	  interrupt(0x10, 0xe*256+line[count+1], 0, 0 ,0);
	  line[count+2] = 0x0;
	  interrupt(0x10, 0xe*256+line[count+2], 0, 0, 0);
	  interrupt(0x10, 0xe*256+temp, 0, 0, 0);
	  return;
	}
    }
}


void readSector(char* buffer, int sector)
{
  int relative_sec = sector + 1, track= 0, head=0;
   
  interrupt(0x13, 2*256+1, buffer, track*256+relative_sec, head*256+0x80);
}


void handleInterrupt21(int ax, int bx, int cx, int dx)
{
  if(ax == 0)
    {
      printString(bx);
    }
  else if(ax == 1)
    {
      readString(bx);
    }
  else if(ax == 2)
    {
      readSector(bx,cx);
    }
  else if(ax == 3)
    {
      readFile(bx,cx,dx);
    }
     else
    {
      printString("that is an incorrect call");
    }
}


void readFile(char* filename, char buffer,int sectorsRead)
{
  char directory[512];
  int fileEntry;
  int count;
  int c;
  
  readSector(directory,2);

  for(fileEntry = 0; fileEntry < 512; fileEntry+32)
    {
      for(c = 0; c<5;c++)
	{
	  printChar(c);
	  if(filename[0] == directory[fileEntry+c])	    
	    {
	      count++;
	    }
	}
    }
}

