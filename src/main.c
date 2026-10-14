#include <util/delay.h>
#include "std_types.h"
#include "keypad.h"
#include "LCD.h"

bool opNumber=false;

typedef struct number {
	float32 value;
} num;

typedef struct node{
	num data;
	struct node* next;
} node;

struct node* head = NULL;

void createNumber(float32 value){
	node* link = malloc(sizeof(struct node));
	node* curr = NULL;

	link->next = NULL;
	link->data.value = value;

	curr = head;
	if(head == NULL){
		head = link;
		return;
	}

	while(curr != NULL){
		if(curr->next == NULL) break;
		else curr = curr->next;
	}

	curr->next = link;
}

float32 Evaluate(uint8 operation, float32 num1, float32 num2){
	switch(operation){
	case '+':
		return num1 + num2;
	case '-':
		return num1 - num2;
	case '*':
		return num1 * num2;
	case '/':
		return num1 / num2;
	}
	return 0;
}

bool IsOperation(uint8 key){
	if(key == '+' || key == '-' || key == '*' || key == '/') return true;
	else return false;
}

void mergeNodes(uint8 opA, uint8 opB){
	node* left = head;
	node* curr = left->next;
	node* right = NULL;

	while(curr != NULL){
		right = curr->next;

		if(curr->data.value == opA || curr->data.value == opB){
			left->data.value = Evaluate(curr->data.value, left->data.value, right->data.value);
			left->next = right->next;
			free(curr);
			free(right);
			curr = left->next;
		}else{
			left = right;
			curr = left->next;
		}
	}
}

float32 operationPrecedence(void){
	mergeNodes('*', '/');
	mergeNodes('+', '-');
	return head->data.value;
}

void clearNodes(void){
	struct node* next = NULL;

	while(head != NULL){
		next = head->next;
		free(head);
		head = next;
	}
}

bool calculatable(bool flag){
	if(flag == true) return true;
	else return false;
}

int main(void){
	float32 number=0;
	uint8 op;
	uint8 key;
	bool flag=true;
	LCD_init();

	for(;;){
		key = KEYPAD_getPressedKey();

		if(key <= 9 && calculatable(flag)){
			LCD_integerToString(key);
			number = (number*10) + key;
			opNumber=true;
		}
		else if(key == '=' && opNumber==true && calculatable(flag)){
			LCD_moveCursor(1,0);
			LCD_displayCharacter(key);

			createNumber(number);
			number = 0;

			LCD_floatToString(operationPrecedence());
			clearNodes();
			opNumber=false;
			flag=false;
		}else if(IsOperation(key) && opNumber==true && calculatable(flag)){
			op = key;
			LCD_displayCharacter(key);

			createNumber(number);
			createNumber(op);
			number = 0;

			opNumber=false;
		}else if(key == 'C'){
			LCD_clearScreen();
			clearNodes();
			number = 0;
			opNumber = false;
			flag=true;
		}
	}
}
