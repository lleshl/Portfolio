#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Studentlist{
	char name[20];
	int age;
	int grade;
	int number;
	int phone;
	struct Studentlist* llink;
	struct Studentlist* rlink;
}Studentlist;

Studentlist *head;

void init(Studentlist* phead)
{
	phead->llink = phead;
	phead->rlink = phead;
}

void insert(Studentlist* before, char* name, int age, int grade, int number, int phone)
{
	Studentlist* p= (Studentlist *)malloc(sizeof(Studentlist));
	strcpy_s(p->name, name);
	p->age = age;
	p->grade = grade;
	p->number = number;
	p->phone = phone;
	p->llink = before;
	p->rlink = before->rlink;
	before->rlink->llink = p;
	before->rlink = p;
}


void print(Studentlist *phead)
{
	Studentlist* p;
	for(p=phead->rlink; p !=phead; p=p->rlink)
		{
			printf("name : %s, 나이 : %d, 학년 : %d, 학번 : %d, 전화번호 : %011d \n", p->name, p->age, p->grade, p->number, p->phone);

		}
}


void remove(Studentlist *head, int number) {
	Studentlist *p, *temp;

	if (head->llink == NULL) return;

	else {
		p = head;

		if (head->llink->llink == head->llink) { 

			p = p->llink;
			head->llink = NULL;
			free(p);

			return;
		}

		if (head->llink->number == number) { 
			do {
				p = p->llink;
			} while (p->llink != head->llink);

			temp = head->llink;
			head->llink = temp->llink;
			temp->llink->rlink = p;
			p->llink = temp->llink;
			free(temp);

			return;
		}

		do {
			p = p->llink;
			if (p->number == number) {
				p->rlink->llink = p->llink;
				p->llink ->rlink = p->rlink;
				return;
			}
		} while (p->llink != head->llink);
	}
}


int main(void)
{
	int o;
	int num;
	char name[20];
	int age;
	int number;
	int phone;
	int grade;
	Studentlist* head = (Studentlist *)malloc(sizeof(Studentlist));
	init(head);

	printf("학적관리시스템\n");

	while (1)
	{
		printf("실행리스트\n");
		printf(" 1. 데이터추가 \n");
		printf(" 2. 리스트출력 \n");
		printf(" 3. 리스트삭제 \n");;
		printf(" 4. 종료 \n\n");
		printf("할 행동의 번호를 입력하시오\n");

		scanf_s("%d", &num);
		switch (num)
		{
		case 1:
			printf("이름을 입력하시오\n");
			scanf_s("%s", &name, 20);
			printf("나이를 입력하시오\n");
			scanf_s("%d", &age);
			printf("학년을 입력하시오\n");
			scanf_s("%d", &grade);
			printf("학번을 입력하시오\n");
			scanf_s("%d", &number);
			printf("전화번호를 입력하시오\n");
			scanf_s("%011d", &phone);

			insert(head, name, age, grade, number, phone);
			break;
		case 2:
			print(head);
			break;
		case 3:
			printf("삭제할 학생의 학번을 입력하시오\n");
			scanf_s("%d", &o);
			remove(head, o);
			break;
		case 4:
			printf("종료합니다. \n");
			exit(1);
		default:
			printf("선택가능한 입력을 하시오\n");
		}
		printf("\n\n");
	}
	return 0;
}
