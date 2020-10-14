/*read getcovs.origin0.sh ,then assembly to getcovs0.sh(v1)  getcovs0.sh(v2)  getcovs0.sh(v3)....*/
/*read getcovs.origin2.sh ,then assembly to getcovs2.sh(v1)  getcovs2.sh(v2)  getcovs2.sh(v3)....*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define CommandLen 2000

void ReplaceSerVer(char[],char[],int ,int );
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

//verTh=0 is vok
void ReplaceSerVer(char *fileOrigin,char *fileAllver,int specialLine,int verTh)
{
	int k=1;//line no.
	char strCommand[CommandLen];
	FILE *fdRead,*fdWrite; /*fdead:read file; fdwrite:write file*/
	fdRead = fopen(fileOrigin,"r");
	fdWrite = fopen(fileAllver,"w");
	if( fdRead==NULL || fdWrite==NULL )
	{
		fprintf(stdout, "Error: file open or file write.\n");
		exit(0);
	}

	while( !feof(fdRead) )
	{
		if( NULL==fgets(strCommand,CommandLen,fdRead) )
			break;
		//fgets(strCommand,CommandLen,fdRead);
		if( k==specialLine )
		{
			if( strCommand[0]!='s' || strCommand[1]!='t' || strCommand[2]!='r' || strCommand[3]!='V'
                 || strCommand[4]!='e' || strCommand[5]!='r' || strCommand[6]!='='|| strCommand[7]!='v' )
			{
				printf("%s setVer is error.strCommand=%s",fileOrigin,strCommand);
				exit(0);
			}
			else
			{
				if( 0==verTh )
					sprintf((char *)strCommand,"strVer=v%s\n","ok");
				else
					sprintf((char *)strCommand,"strVer=v%d\n",verTh);
			}
		}
		fputs(strCommand,fdWrite);
		k++;
	}
	//close file
	fclose(fdRead);
	fclose(fdWrite);
}

//read file,return the line number.
int readSeedshFile(char *filename)
{
	int total = 0;
	char strSeedno[100];
	FILE *fdRead;
	fdRead = fopen(filename,"r");
	while( !feof(fdRead) )
	{
		if( NULL==fgets(strSeedno,100,fdRead) )
			break;
		if( (strstr(strSeedno,"/*")!=NULL) && 
			(strstr(strSeedno,"*/")!=NULL) )
			total++;
	}
	fclose(fdRead);
	return total;
}

//change file.
//the verno's /**/ will delete , verno from 1 start.
void changeSeedshFile(int verno)
{
	int index = 0;
	char strSeedno[100];
	char testFilename[100];

	FILE *fdRead,*fdWrite; /*fdead:read file; fdwrite:write file*/
	fdRead = fopen("FaultSeeds.origin.h","r");
	//sprintf(testFilename,"FaultSeeds%d.h",verno);
	//fdWrite = fopen(testFilename,"w");
	fdWrite = fopen("FaultSeeds.h","w");
	if( fdRead==NULL || fdWrite==NULL )
	{
		fprintf(stdout, "Error: file open or file write.\n");
		exit(0);
	}
	while( !feof(fdRead) )
	{
		if( NULL==fgets(strSeedno,100,fdRead) )
			break;
		if( (strstr(strSeedno,"/*")!=NULL) && 
			(strstr(strSeedno,"*/")!=NULL) )
			index++;
		if( index==verno )
		{
			replaceFirst(strSeedno,"/*","");
			replaceFirst(strSeedno,"*/","");
		}
		fputs(strSeedno,fdWrite);
	}
	//close file
	fclose(fdRead);
	fclose(fdWrite);
}


/*************
      main
******************/
int main(int argc, char **argv)
{
	int k,verNum=-1;//version no.
	char seedFilename[] = "FaultSeeds.origin.h";
	char strObject[30];

	if( argc<2 )
	{
		fprintf(stdout, "Error: Command line arguments are (SIR object name).\n");
		exit(0);
	}
	strcpy(strObject,argv[1]);	
	fprintf(stdout, "%s\n", strObject);

	verNum = readSeedshFile(seedFilename);
	for( k=0;k<=verNum;k++ )
	{
		system("bash rm FaultSeeds.h");
		if( 0==k )
			system("cp FaultSeeds.origin.h FaultSeeds.h");
		else
			changeSeedshFile(k);
		//ReplaceSerVer("buildexe.origin.sh","buildexe.sh",1,k);
		system("bash buildexe.sh");
		if( 0==strcmp(strObject,"flex") )
		{
			ReplaceSerVer("getcovs.origin0.sh","getcovs0.sh",1,k);	
			system("bash getcovs0.sh");
			ReplaceSerVer("getcovs.origin2.sh","getcovs2.sh",1,k);	
			system("bash getcovs2.sh");
		}
	}
	return 0;
}
