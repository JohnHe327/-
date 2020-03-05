#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAXLIST 100
typedef struct POLY
{
	struct POLY * next;
	int p;
	float a;
} Poly; //��a^p

Poly * GetPoly();
int PrintPoly(Poly *head);
int AddPoly(Poly *head1,Poly *head2);
int SubPoly(Poly *head1,Poly *head2);
int CalPoly(Poly *head,float x);
void DestroyPoly(Poly *head);
void ClearPoly(Poly *head); 
int InsertTerm(Poly *head);
int DeleteTerm(Poly *head);
int EditTerm(Poly *head);
int Diff(Poly *head,int n);
int coef(int p,int n);
int Intg(Poly *head);
int IntgAB(Poly *head,float a,float b);

int main()
{
	Poly *HeadList[MAXLIST+10];
	int Poly_count=0;
	int func=-1;
	int num=0,num1=0,num2=0;
	float x=0,a,b;
	while(1){
		printf("��ѡ����:\n");
		printf(" 1.��������ʽ\n");
		printf(" 2.�������ʽ\n");
		printf(" 3.����ʽ���\n");
		printf(" 4.����ʽ���\n");
		printf(" 5.����ʽ��ֵ\n");
		printf(" 6.����ʽ����\n");
		printf(" 7.����ʽ���\n");
		printf(" 8.����ʽ����\n");
		printf(" 9.����ʽɾ��\n");
		printf("10.����ʽ�޸�\n");
		printf("11.����ʽ΢��\n");
		printf("12.����ʽ��������\n");
		printf("13.����ʽ������\n");
		printf(" 0.�˳�����\n\n");
		
		if(scanf("%d",&func)!=1){
			printf("ѡ��ʧ�ܣ�������\n\n"); 
			while(getchar()!='\n');
			continue; 
		}
		if(!func) break;
		switch(func){
			case 1://���� 
				if(Poly_count<MAXLIST){
					printf("���������ʽ��\n") ;
					HeadList[Poly_count]=GetPoly();
					if(HeadList[Poly_count]==NULL) printf("����ʧ��\n\n");
					else {
						PrintPoly(HeadList[Poly_count]);
						printf("����ɹ����洢Ϊ��%d������ʽ\n\n",++Poly_count);
					}
				}
				else{
					printf("�б�����\n\n");
				}
				break;
			case 2://���
				printf("������Ҫ����Ķ���ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					if(PrintPoly(HeadList[num-1])) 
						printf("���\n��%d��\n\n",HeadList[num-1]->p);
				}
				num=0;
				break;
			case 3://���
				printf("����������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d%d",&num1,&num2)!=2)
					while(getchar()!='\n');
				if(num1<1 || num1>Poly_count || num2<1 || num2>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					AddPoly(HeadList[num1-1],HeadList[num2-1]);
				}
				num1=num2=0;
				break;
			case 4://���
				printf("�����뱻��ʽ���ʽ���(1~%d)��",Poly_count);
				if(scanf("%d%d",&num1,&num2)!=2)
					while(getchar()!='\n');
				if(num1<1 || num1>Poly_count || num2<1 || num2>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					SubPoly(HeadList[num1-1],HeadList[num2-1]);
				}
				num1=num2=0;
				break;
			case 5://��ֵ
				printf("���������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				else if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					PrintPoly(HeadList[num-1]);
					printf("�������Ա���ֵx��ֵ��");
					if(scanf("%f",&x)!=1)
						while(getchar()!='\n');
					else if(CalPoly(HeadList[num-1],x)) printf("���\n\n");
					else printf("����\n\n");
				}
				num=0; 
				break;
			case 6://���� 
				if(Poly_count==0){
					printf("�޶���ʽ\n\n");
					break;
				}
				printf("������Ҫ���ٵĶ���ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					DestroyPoly(HeadList[num-1]);
					for(int i=num-1;i<Poly_count-1;i++)
						HeadList[i]=HeadList[i+1];
					HeadList[--Poly_count]=NULL;
					printf("�����٣�ʣ��%d������ʽ\n\n",Poly_count);
				}
				num=0; 
				break;
			case 7://��� 
				printf("������Ҫ��յĶ���ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					ClearPoly(HeadList[num-1]);
					if(HeadList[num-1]->next == NULL && HeadList[num-1]->p == 0)
						printf("��%d������ʽ�����\n\n",num);
				}
				num=0;
				break;
			case 8://����
				printf("�������������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					num1=InsertTerm(HeadList[num-1]);
					if(num1!=-1)
						printf("����ɹ����¶���ʽ��%d��\n\n",num1);
					else printf("����ʧ��\n\n");
				}
				num=num1=0;
				break;
			case 9://ɾ�� 
				printf("�������������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					num1=DeleteTerm(HeadList[num-1]);
					if(num1==-1)
						printf("ɾ��ʧ��\n\n");
					else
						printf("ɾ���ɹ���ʣ��%d��\n\n",num1);
				}
				num=num1=0;
				break;
			case 10://�޸� 
				printf("�������������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					if(EditTerm(HeadList[num-1]))
						printf("���޸�\n\n");
					else
						printf("�޸�ʧ��\n\n");
				}
				num=0;
				break;
			case 11://΢�� 
				printf("�������������ʽ���(1~%d)��΢�ֽ�����",Poly_count);
				if(scanf("%d%d",&num,&num2)!=2)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else if(num2<0){
					printf("��Ч����\n\n");
				}
				else{
					if(Diff(HeadList[num-1],num2))
						printf("���\n\n");
				}
				num=num2=0;
				break;
			case 12://��������
				printf("�������������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					if(Intg(HeadList[num-1]))
						printf("���\n\n");
				}
				num=0;
				break;
			case 13://������ 
				printf("�������������ʽ���(1~%d)��",Poly_count);
				if(scanf("%d",&num)!=1)
					while(getchar()!='\n');
				if(num<1 || num>Poly_count){
					printf("��Ŵ���\n\n");
				}
				else{
					printf("��������ֽ磺");
					if(scanf("%f%f",&a,&b)!=2)
						while(getchar()!='\n');
					if(IntgAB(HeadList[num-1],a,b))
						printf("���\n\n");
				}
				num=0;
				break;
			default: printf("ѡ�����\n");
		}//switch
	}//while
	return 0;
}

Poly * GetPoly()
{
	Poly *head,*pnew,*tail;
	char c='+';
	int term=0;
	head=(Poly *)malloc(sizeof(Poly));
	if(head==NULL){
		printf("�ڴ治��\n");
		return NULL;
	}
	head->next=NULL;
	head->p=0;//head.p�洢����
	head->a=0;
	tail=head;
	
	do
	{
		pnew=(Poly *)malloc(sizeof(Poly));
		if(pnew==NULL){
			printf("�ڴ治�㣬�Ѵ���ǰ%d��\n",term);
			break;
		}
		if(scanf("%f*x^%d",&pnew->a,&pnew->p)!=2){
			printf("��ʽ����\n");
			DestroyPoly(head);
			while(getchar()!='\n');
			return NULL;
		}
		if(c=='-') pnew->a=-pnew->a;
		if(pnew->a!=0){
			pnew->next=NULL;
			tail->next=pnew;
			tail=pnew;
			term++;
		}
		else free(pnew);
	} while((c=getchar())!='\n');//�������ʽ
	
	if(term>1)
	for(int i=0;i<term;i++)
	{
		Poly *p=head;
		Poly *q=p->next;
		Poly *r=q->next;
		while(r!=NULL)
		{
			if(q->p > r->p)
			{
				q->next=r->next;
				r->next=q;
				p->next=r;
			}
			p=p->next;
			q=p->next;
			r=q->next;
		}
	}//ָ����С�������� 
	
	if(term>1){
		Poly *p=head;
		Poly *q=p->next;
		Poly *r=q->next;
		while(r!=NULL){
			if(q->p == r->p){
				if(q->a + r->a ==0){
					p->next=r->next;
					free(q);
					free(r);
					term-=2; 
				}//ϵ���෴
				else{
					q->a+=r->a;
					q->next=r->next; 
					free(r);
					term--;
				}//ϵ���Ͳ�Ϊ��
			}//��ָ�����
			else p=p->next;
			q=p->next;
			r=q->next;
		}//q��r����Чʱ
	}//�ϲ�ͬ����
	
	head->p=term;
	return head;
}

int PrintPoly(Poly *head)
{
	Poly *p=head->next;
	if(p!=NULL){
		printf("%.3f*x^%d",p->a,p->p);
		p=p->next;
	}
	else{
		printf("0\n");
		return 1;
	}
	while(p!=NULL){
		printf("%+.3f*x^%d",p->a,p->p);
		p=p->next;
	}
	printf("\n");
	return 1;
}//�������ʽ 

int AddPoly(Poly *head1,Poly *head2)
{
	Poly *p=head1->next,*q=head2->next;
	
	PrintPoly(head1);
	PrintPoly(head2);
	putchar('\n');
	 
	while(p!=NULL && q!=NULL && p->p == q->p && p->a + q->a == 0){
		p=p->next;
		q=q->next;
	}//Ѱ�ҵ�һ���ɲ����� 
	if(p==NULL){
		Poly *temp=head2;
		while(temp->next!=q) temp=temp->next;
		PrintPoly(temp);
		return 1;
	}//pΪNULL��ֱ�����q 
	else if(q==NULL){
		Poly *temp=head1;
		while(temp->next!=p) temp=temp->next;
		PrintPoly(temp);
		return 1;
	}//qΪNULL��ֱ�����p 
	else{
		if(p->p < q->p){
			printf("%.3f*x^%d",p->a,p->p);
			p=p->next;
		}
		else if(p->p > q->p){
			printf("%.3f*x^%d",q->a,q->p);
			q=q->next;
		}
		else{
			printf("%.3f*x^%d",p->a+q->a,p->p);
			p=p->next;
			q=q->next;
		}
	}//�����Ҫ������� 
	
	while(p!=NULL&&q!=NULL){
		if(p->p < q->p){
			printf("%+.3f*x^%d",p->a,p->p);
			p=p->next;
		}//pָ��С
		else if(p->p > q->p){
			printf("%+.3f*x^%d",q->a,q->p);
			q=q->next;
		}//qָ��С 
		else{
			if(p->a + q->a){
				printf("%+.3f*x^%d",p->a+q->a,p->p);
			}//ϵ���Ͳ�Ϊ�� 
			p=p->next;
			q=q->next;
		}//pqָ����� 
	}
	
	while(p!=NULL){
		printf("%+.3f*x^%d",p->a,p->p);
		p=p->next;
	}
	while(q!=NULL){
		printf("%+.3f*x^%d",q->a,q->p);
		q=q->next;
	}
	printf("\n");
	printf("���\n\n");
	return 1;
}

int SubPoly(Poly *head1,Poly *head2)
{
	Poly *head2Minus,*tail,*pnew;
	
	PrintPoly(head1);
	PrintPoly(head2);
	putchar('\n');
	
	head2Minus=(Poly *)malloc(sizeof(Poly));
	if(head2Minus==NULL){
		printf("�ڴ治��\n\n");
		return 0;
	}
	head2Minus->next=NULL;
	head2Minus->p=head2->p;
	head2Minus->a=0;
	tail=head2Minus;
	Poly *p=head2->next;
	while(p!=NULL){
		pnew=(Poly *)malloc(sizeof(Poly));
		if(pnew==NULL){
			printf("�ڴ治��\n\n");
			return 0;
		}
		pnew->a=-p->a;
		pnew->p=p->p;
		pnew->next=NULL;
		tail->next=pnew;
		tail=pnew;
		p=p->next;
	}//����������ʽ 
	
	AddPoly(head1,head2Minus);
	DestroyPoly(head2Minus);//�ͷ��ڴ� 
	return 1;
}

int CalPoly(Poly *head,float x)
{
	Poly *p=head->next;
	float Ans=0;
	printf("x=%.3f\n",x);
	while(p!=NULL){
		if(p->p<0 && x==0) return 0;
		Ans+=p->a*pow(x,p->p);
		p=p->next;
	}
	printf("���ʽ��ֵΪ%f\n",Ans);
	return 1;
}

void DestroyPoly(Poly *head)
{
	Poly *p=head->next;
	while(p!=NULL){
		head->next=p->next;
		free(p);
		p=head->next;
	}
	free(head);
}

void ClearPoly(Poly *head)
{
	Poly *p=head->next;
	while(p!=NULL){
		head->next=p->next;
		free(p);
		p=head->next;
	}
	head->p=0;//�������� 
}

int InsertTerm(Poly *head)
{
	Poly *p=head;
	Poly *q=p->next;
	float aIN;
	int pIN;
	printf("���������ڵ�ϵ����ָ����");
	if(scanf("%f%d",&aIN,&pIN)!=2){
		printf("��ʽ����\n");
		while(getchar()!='\n');
		return -1;
	}
	if(aIN==0)
		return head->p;//�޳���Ч���� 
	
	while(q->p < pIN){
		p=p->next;
		q=p->next;
	}//Ѱ�Ҳ���� 
	
	if(q->p > pIN){
		Poly *pnew=(Poly *)malloc(sizeof(Poly));
		if(pnew==NULL){
			printf("�ڴ治��\n");
			return -1;
		}
		pnew->a=aIN;
		pnew->p=pIN;
		pnew->next=q;
		p->next=pnew;
		head->p++;
	}//����ָͬ����
	else{
		if(aIN + q->a != 0)
			q->a+=aIN;
		else{
			p->next=q->next;
			free(q);
			head->p--;
		}//ϵ������ 
	} 
	return head->p;
}

int DeleteTerm(Poly *head)
{
	Poly *p=head;
	Poly *q=p->next;
	int pDEL;
	PrintPoly(head);
	printf("������ɾ���ڵ�ָ����");
	if(scanf("%d",&pDEL)!=1){
		printf("��ʽ����\n");
		while(getchar()!='\n');
		return -1;
	}
	while(q!=NULL && q->p!=pDEL){
		p=p->next;
		q=p->next;
	}
	if(q==NULL){
		printf("�޶�Ӧ��\n");
		return -1;
	}
	p->next=q->next;
	free(q);
	head->p--;
	return head->p;
}

int EditTerm(Poly *head)
{
	float aE;
	int pE;
	Poly *p=head,*q=head->next;
	
	PrintPoly(head);
	
	printf("������Ҫ�޸ĵ����ָ����");
	if(scanf("%d",&pE)!=1)
		while(getchar()!='\n');
	while(q!=NULL){
		if (q->p==pE)
			break;
		p=p->next;
		q=p->next;
	}//Ѱ���޸��� 
	if(q==NULL){
		printf("�޶�Ӧ��\n");
		return 0;
	}
	
	printf("�������޸ĺ�ڵ�ϵ����ָ����");
	if(scanf("%f%d",&aE,&pE)!=2 || getchar()!='\n'){
		printf("�������\n");
		while(getchar()!='\n');
		return 0; 
	}
	if(aE==0){
		p->next=q->next;
		free(q);
		head->p--;
		return 1;
	}//aE=0��Ч��ɾ���ڵ�
	if(q->p==pE){
		q->a=aE;
		return 1;
	}//ָ����ȿɼ��滻 
	
	q->a=aE;
	q->p=pE;
	p->next=q->next;
	Poly *R=head,*S=head->next;
	while(S!=NULL && S->p < pE){
		R=R->next;
		S=R->next;
	}
	if(S==NULL){
		R->next=q;
		q->next=NULL;
		return 1;
	}//����ָ����� 
	if(S->p > pE){
		R->next=q;
		q->next=S;
		return 1;
	}//����ָͬ����ֱ�Ӳ��� 
	
	if(S->a + aE ==0){
		R->next=S->next;
		free(S);
		free(q);
		head->p-=2;
		return 1;
	}
	S->a += q->a;
	free(q);
	head->p--;
	return 1;
}

int Diff(Poly *head,int n)
{
	Poly *p=head->next;
	int flag=0;//�����Ч���
	float aOUT;
	int pOUT; 
	while(p!=NULL){
		if(p->p > 0 && p->p < n);//΢�ֺ�Ϊ0
		else{
			aOUT=p->a*coef(p->p,n);
			pOUT=p->p-n;
			if(aOUT!=0 && flag==0){
				printf("%.3f*x^%d",aOUT,pOUT);
				flag++;
			}
			else if(aOUT!=0)
				printf("%+.3f*x^%d",aOUT,pOUT);
		}
		p=p->next;
	}
	if(flag==0) printf("0");
	printf("\n");
	return 1;
}

int coef(int p,int n)
{
	if(n==0) return 1;
	else return p*coef(p-1,n-1);
}

int Intg(Poly *head)
{
	Poly *p=head->next;
	int flag=0;//�����Ч���
	while(p!=NULL){
		if(p->p==-1){
			if(flag==0){
				printf("%.3f*ln(x)",p->a);
				flag++;
			}
			else
				printf("%+.3f*ln(x)",p->a);
		}//p=-1����Ϊ����
		else{
			if(flag==0){
				printf("%.3f*x^%d",p->a/(p->p+1),p->p+1);
				flag++;
			}
			else
				printf("%+.3f*x^%d",p->a/(p->p+1),p->p+1);
		}
		p=p->next;
	}
	
	if(flag!=0)
		putchar('+');
	printf("C\n");
	return 1;
}

int IntgAB(Poly *head,float a,float b)
{
	Poly *p=head->next;
	float Ans=0;
	while(p!=NULL){
		if(p->p == -1){
			if(a<=0 || b<=0){
				printf("���ֽ����\n\n");
				return 0;
			}
			Ans+=log(b/a);
		}//���� 
		else if(p->p >= 0){
			Ans+=(p->a/(p->p+1))*(pow(b,p->p+1)-pow(a,p->p+1));
		}//��ָ�� 
		else{
			if(a*b<=0){
				printf("���ֽ����\n\n");
				return 0;
			}
			Ans+=(p->a/(p->p+1))*(pow(b,p->p+1)-pow(a,p->p+1));
		}//��ָ�� 
		p=p->next;
	}
	printf("%.3f\n",Ans); 
	return 1;
}

