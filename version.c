#include <stdio.h>
#include <stdlib.h>

main(){
	FILE *fp;
	char str[256],ver[256];
	int a,i,size;

	fp = fopen("history.txt","r");
	if(fp==NULL)return 1;

	fgets(str,256,fp);
	fclose(fp);
	if(str[strlen(str)-1] == '\n'){
		str[strlen(str)-1] = '\0';
	}
	i = 0;
	while(str[i] != '\t' && str[i] != '\0')i++;
	strcpy(ver,&str[i+1]);

	fp = fopen("src/version.h","r");
	if(fp==NULL)return 1;

	fgets(str,256,fp);
	a = atoi(&str[16]);
	fclose(fp);

	fp = fopen("src/version.h","w");
	if(fp==NULL)return 1;
	fprintf(fp,"#define VERSION %d\n",a+1);
	fprintf(fp,"#define OPEN_VERSION L\"%s\"\n",ver);
	fclose(fp);
	return 0;
}

