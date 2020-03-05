#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100
int main()
{
	int HTable[MAXSIZE];
	int Scnt[MAXSIZE]={0},UScnt[MAXSIZE];
	int input[MAXSIZE];
	int total,p;
	
	scanf("%d",&total);
	//input=(int*)malloc(sizeof(int)*total);
	for(int i=0;i<total;i++){
		scanf("%d",&input[i]);
	}
	
	scanf("%d",&p);
	//HTable=(int *)malloc(sizeof(int)*p);
	//Scnt=(int *)malloc(sizeof(int)*p);
	//UScnt=(int *)malloc(sizeof(int)*p);
	
	//Creathash
	//initial
	for(int i=0;i<p;i++)
	HTable[i]=-1;
	
	for(int i=0;i<total;i++){
		int point=input[i]%p;
		int success=0;
		while(HTable[point]!=-1){
			point=(point+1)%p;
			success++;
		}
		HTable[point]=input[i];
		Scnt[point]=success+1;
	}
	//unseccess count
	for(int i=0;i<p;i++){
		int j=i;
		while(HTable[j%p]!=-1 && j-i<p) j++;
		if(HTable[j%p]==-1) UScnt[i]=j-i+1;
		else UScnt[i]=p;
	}
	
	//print hashtable
	printf("\n��ϣ��ĵ�ַ��\t");
	for(int i=0;i<p;i++) printf("%d\t",i);
	printf("\n���еĹؼ��֣�\t");
	for(int i=0;i<p;i++){
		if(HTable[i]!=-1) printf("%d\t",HTable[i]);
		else printf("-\t");
	}
	printf("\n�ɹ����Ҵ�����\t");
	for(int i=0;i<p;i++) printf("%d\t",Scnt[i]);
	printf("\nʧ�ܲ��Ҵ�����\t");
	for(int i=0;i<p;i++) printf("%d\t",UScnt[i]);
	float scnt=0,uscnt=0;
	for(int i=0;i<p;i++){
		scnt+=Scnt[i];
		uscnt+=UScnt[i];
	}
	printf("\nƽ���ɹ����ҳ��ȣ�%f",scnt/total);
	printf("\nƽ��ʧ�ܲ��ҳ��ȣ�%f",uscnt/p);
	
	//search 
	printf("\n\n�������key��");
	int cnt=1,key;
	scanf("%d",&key);
	int point=key%p;
	while(HTable[point%p]!=-1 && HTable[point%p]!=key && cnt<=p ){
		point++;
		cnt++;
	}
	
	if(HTable[point%p]==key) printf("\n���б�ţ�%d\n�Ƚϴ�����%d",point%p,cnt);
	else if(cnt>p) printf("\n����ʧ�ܣ��Ƚϴ�����%d",p);
	else printf("\n����ʧ�ܣ��Ƚϴ�����%d",point-key%p+1);
	return 0;
}

