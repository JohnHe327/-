#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100
typedef struct HNODE{
	int ele;
	int Scnt;
	struct HNODE *next;
}HNode;

int main()
{
	HNode HTable[MAXSIZE];
	int input[MAXSIZE];
	int total,p;
	float scnt=0,uscnt=0;
	
	scanf("%d",&total);
	for(int i=0;i<total;i++){
		scanf("%d",&input[i]);
	}
	
	scanf("%d",&p);
	
	//Creathash
	//initial
	for(int i=0;i<p;i++){
		HTable[i].ele=-1;
		HTable[i].Scnt=1;
		HTable[i].next=NULL;
	}
	
	for(int i=0;i<total;i++){
		int fkey=input[i]%p;
		HNode* pointer=&HTable[fkey];
		int success=1;
		
		while(pointer->next!=NULL){
			pointer=pointer->next;
			success++;
		}
		HNode* pnew=(HNode *)malloc(sizeof(HNode));
		pnew->ele=input[i];
		pnew->Scnt=success;
		pnew->next=NULL;
		pointer->next=pnew;
		HTable[fkey].Scnt=success+1;
	}
	
	//print hashtable
	for(int i=0;i<p;i++){
		printf("\n\n��ϣ��ĵ�ַ��\t%d",i);
		printf("\n���еĹؼ��֣�\t");
		if(HTable[i].next==NULL) printf("-");
		else{
			HNode* pointer=HTable[i].next;
			while(pointer!=NULL){
				printf("%d\t",pointer->ele);
				pointer=pointer->next;
			}
		}
		printf("\n�ɹ����Ҵ�����\t");
		if(HTable[i].next==NULL) printf("0");
		else{
			HNode* pointer=HTable[i].next;
			while(pointer!=NULL){
				printf("%d\t",pointer->Scnt);
				scnt+=pointer->Scnt;
				pointer=pointer->next;
			}
		}
		printf("\nʧ�ܲ��Ҵ�����\t%d",HTable[i].Scnt);
		uscnt+=HTable[i].Scnt;
	}
	
	printf("\nƽ���ɹ����ҳ��ȣ�%f",scnt/total);
	printf("\nƽ��ʧ�ܲ��ҳ��ȣ�%f",uscnt/p);
	
	//search 
	printf("\n\n�������key��");
	int key;
	scanf("%d",&key);
	int fkey=key%p;
	HNode* pointer=HTable[fkey].next;
	if(pointer==NULL) printf("\n����ʧ�ܣ��Ƚϴ�����1");
	else{
		while(pointer!=NULL && pointer->ele!=key)
		pointer=pointer->next;
	}
	if(pointer==NULL) printf("\n����ʧ�ܣ��Ƚϴ�����%d",HTable[fkey].Scnt);
	else printf("\n���ڱ�ţ�%d\n�Ƚϴ�����%d",fkey,pointer->Scnt);
	return 0;
}

