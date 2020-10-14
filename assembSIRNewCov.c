/*read scripts/runall0.sh ,then assembly to getcovs0.sh
read scripts/runall2.sh ,then assembly to getcovs2.sh
*/
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define CommandLen 5000

void replaceFirst(char *str1,char *str2,char *str3);
 
/*将str1字符串中第一次出现的str2字符串替换成str3*/  
void replaceFirst(char *str1,char *str2,char *str3)  
{  
    char str4[strlen(str1)+1];  
    char *p;  
    strcpy(str4,str1);  
    if((p=strstr(str1,str2))!=NULL)/*p指向str2在str1中第一次出现的位置*/  
    {  
        while(str1!=p&&str1!=NULL)/*将str1指针移动到p的位置*/  
        {  
            str1++;  
        }  
        str1[0]='\0';/*将str1指针指向的值变成/0,以此来截断str1,舍弃str2及以后的内容，只保留str2以前的内容*/  
        strcat(str1,str3);/*在str1后拼接上str3,组成新str1*/  
		/*strstr(str4,str2)是指向str2及以后的内容(包括str2),strstr(str4,str2)+strlen(str2)就是将指针向前移动strlen(str2)位，跳过str2*/  
        strcat(str1,strstr(str4,str2)+strlen(str2));
    }  
}  

//strObject:SIR object name [30]
//flag = 0 or 2.
void AssembleCoverageShellFile(char *strObject,int flag)
{
	int k=0;//testcase no.
	char strCommand[CommandLen];/*strCommand:shell command name*/
	char strExecObjectCommand[CommandLen];/*./object.int.exe*/
	char inputFile[100],outputFile[100];
	char  mvLine = 'N';//='Y',last row is mv ....;
	char strNewShell[CommandLen],strTmp[CommandLen];//new shell command.

	sprintf(strExecObjectCommand,"%s.int.exe",strObject);
	sprintf(inputFile,"../scripts/runall%d.sh",flag);
	sprintf(outputFile,"./getcovs.origin%d.sh",flag);

	FILE *fdRead,*fdWrite; /*fdead:read file; fdwrite:write file*/
	fdRead = fopen(inputFile,"r");
	fdWrite = fopen(outputFile,"w");
	if( fdRead==NULL || fdWrite==NULL )
	{
		fprintf(stdout, "Error: file open or file write.\n");
		exit(0);
	}
	
	fputs("strVer=v?\n",fdWrite);//define v1-v(xxx) and vok

	while( !feof(fdRead) )
	{
		if( NULL==fgets(strCommand,CommandLen,fdRead) )
			break;
		
		if( strCommand[0]=='#' )//comment infomation or #V0.X(testcase no)
		{
			fputs(strCommand,fdWrite);
			mvLine =  'N';
		}
		else if(  strCommand[0]=='e' && strCommand[1]=='c' && strCommand[2]=='h' && strCommand[3]=='o' )
		{	//read "echo ">>>>>>>>running test XXX" "
			fputs(strCommand,fdWrite);
			sprintf((char *)strNewShell,"rm %s.gcda\n",strObject); //del .gcda, otherwise execute times will add.
			fputs(strNewShell,fdWrite);
			if( 0==strcmp(strObject,"flex") )
			{
				if( 0==flag )
				{	//remove previous testing generate result file
					fputs("rm lex.yy.c\n",fdWrite);
					fputs("rm error\n",fdWrite);
				}
				else
				{ //2==falag
					fputs("rm error\n",fdWrite);
					sprintf((char *)strNewShell,"rm ../outputs/out2.n%d\n",k+1);	//notice:k+1 
					fputs(strNewShell,fdWrite); //check 
				}
			}
			mvLine = 'N';
		}
		else if(  strCommand[0]=='\n' || strCommand[1]=='\n'  )
		{ //blank line
			if( 'Y'==mvLine )
			{ //last row is mv, then add so....
				sprintf((char *)strNewShell,"gcov %s.c\n",strObject);
				fputs(strNewShell,fdWrite);
				sprintf((char *)strNewShell,"mv %s.c.gcov ../traces/${strVer}/t%d_%d.gcov\n",strObject,flag,k);
				fputs(strNewShell,fdWrite);
			}
			fputs(strCommand,fdWrite); //blank line.
			mvLine = 'N';
		}
		else if(  strCommand[0]=='m' && strCommand[1]=='v'&& strCommand[2]==' '  )
		{//mv ....
			replaceFirst(strCommand,"../outputs/","../outputs/${strVer}/");
			fputs(strCommand,fdWrite);
			mvLine = 'Y';
		}
		else
		{ //execute object.inst.exe
			if( strstr(strCommand,strExecObjectCommand)==NULL )
			{
				printf("%s can't find %s\n",strCommand,strExecObjectCommand);
				fclose(fdRead);
				fclose(fdWrite);
				return;
			}
			sprintf((char *)strTmp,"./%s.int.exe",strObject);
			replaceFirst(strCommand,strExecObjectCommand,strTmp);
			fputs(strCommand,fdWrite);
			k++;
			mvLine = 'N';
			if( (0==strcmp(strObject,"flex")) && (2==flag) )
			{ //patch 
				sprintf((char *)strNewShell,"mv ../outputs/out2.n%d ../outputs/${strVer}/out2.n%d \n",k,k);	
				fputs(strNewShell,fdWrite); //check 
			}
		}
		//if( k>=5 ) //for test.
		//	break;
	}
	fputs("echo ${strVer} is finished.\n",fdWrite);//prompt
	//close file
	fclose(fdRead);
	fclose(fdWrite);
}

/*************
      main
******************/
int main(int argc, char **argv)
{
	char strObject[30];/*strObject:SIR object name*/

	if( argc<2 )
	{
		fprintf(stdout, "Error: Command line arguments are (SIR object name).\n");
		exit(0);
	}
	strcpy(strObject,argv[1]);
	if( 0==strcmp(strObject,"flex" ) )
	{
		AssembleCoverageShellFile(strObject,0);
		AssembleCoverageShellFile(strObject,2);
	}
	else if( 0==strcmp(strObject,"grep" ) )
	{
	}
	else
	{
	}
	return 0;
}
