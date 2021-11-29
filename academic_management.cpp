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
			printf("name : %s, ���� : %d, �г� : %d, �й� : %d, ��ȭ��ȣ : %011d \n", p->name, p->age, p->grade, p->number, p->phone);

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

	printf("���������ý���\n");

	while (1)
	{
		printf("���ฮ��Ʈ\n");
		printf(" 1. �������߰� \n");
		printf(" 2. ����Ʈ��� \n");
		printf(" 3. ����Ʈ���� \n");;
		printf(" 4. ���� \n\n");
		printf("�� �ൿ�� ��ȣ�� �Է��Ͻÿ�\n");

		scanf_s("%d", &num);
		switch (num)
		{
		case 1:
			printf("�̸��� �Է��Ͻÿ�\n");
			scanf_s("%s", &name, 20);
			printf("���̸� �Է��Ͻÿ�\n");
			scanf_s("%d", &age);
			printf("�г��� �Է��Ͻÿ�\n");
			scanf_s("%d", &grade);
			printf("�й��� �Է��Ͻÿ�\n");
			scanf_s("%d", &number);
			printf("��ȭ��ȣ�� �Է��Ͻÿ�\n");
			scanf_s("%011d", &phone);

			insert(head, name, age, grade, number, phone);
			break;
		case 2:
			print(head);
			break;
		case 3:
			printf("������ �л��� �й��� �Է��Ͻÿ�\n");
			scanf_s("%d", &o);
			remove(head, o);
			break;
		case 4:
			printf("�����մϴ�. \n");
			exit(1);
		default:
			printf("���ð����� �Է��� �Ͻÿ�\n");
		}
		printf("\n\n");
	}
	return 0;
}
