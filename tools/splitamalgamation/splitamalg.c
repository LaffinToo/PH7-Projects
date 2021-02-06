#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char **argv)
{
	char *buf,fn[41],fp[46],*t,*x;
	int len,rd;
	long pos=0;
	FILE *ifh,*ofh=NULL,*flh;
	
	buf=malloc(8193);
	
	if(!(ifh=fopen("../../ph7.c","r")) || !buf)
	{
		puts("Fatal error");
		exit(1);
	}
	ofh=fopen("src/ph7.c","w");
	flh=fopen("ph7.lst","w");
	fprintf(flh,"ph7.c\n");
	
	while(rd=fread(buf,1,8192,ifh))
	{
		if(t=strstr(buf,"\n/*\n * -"))
			t[1]=0;
		len=strlen(buf);
		pos+=len;
		fseek(ifh,pos,SEEK_SET);
		if(buf==strstr(buf,"/*\n * -"))
		{
			if(t=strstr(buf,"File: "))
			{
				t+=6;
				x=strchr(t,'\n');
				*x=0;
				fprintf(flh,"%s\n",t);
				sprintf(fp,"src/%s",t);
				*x='\n';
				fclose(ofh);
				fopen(fp,"w");
				
			}
		}
		fwrite(buf,1,len,ofh);

	}
	fclose(ifh);
}