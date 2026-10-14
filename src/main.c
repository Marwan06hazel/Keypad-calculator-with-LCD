#include <util/delay.h>
#include "std_types.h"
#include "keypad.h"
#include "LCD.h"

/* Flag: true when a number has been typed and is waiting for an operator or '=' */
bool opNumber=false;

static uint8 digitsCount=0;

/* Holds one number (or one operator character) stored in the expression */
typedef struct number {
	float32 value;
} num;

/* Linked-list node: the expression is stored as number, operator, number, ... */
typedef struct node{
	num data;
	struct node* next;
} node;

/* Start of the expression list (NULL means the list is empty) */
struct node* head = NULL;

/*
 * Description:
 * Create a new node holding 'value' and add it at the end of the list.
 * Used for both numbers and operator characters.
 */
void createNumber(float64 value){
	node* link = malloc(sizeof(struct node));
	node* curr = NULL;

	link->next = NULL;
	link->data.value = value;

	/* Empty list: the new node becomes the head */
	curr = head;
	if(head == NULL){
		head = link;
		return;
	}

	/* Otherwise walk to the last node */
	while(curr != NULL){
		if(curr->next == NULL) break;
		else curr = curr->next;
	}

	/* Attach the new node after the last one */
	curr->next = link;
}

/*
 * Description:
 * Apply one operation (+ - * /) on two numbers and return the result.
 */
float64 Evaluate(uint8 operation, float64 num1, float64 num2){
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

/*
 * Description:
 * Return true if the key is one of the four operators.
 */
bool IsOperation(uint8 key){
	if(key == '+' || key == '-' || key == '*' || key == '/') return true;
	else return false;
}

/*
 * Description:
 * One pass over the list: find every operator equal to opA or opB and
 * replace "left operator right" with a single node holding the result.
 * left   -> left operand (keeps the result)
 * curr   -> operator node
 * right  -> right operand
 */
void mergeNodes(uint8 opA, uint8 opB){
	node* left = head;
	node* curr = left->next;
	node* right = NULL;

	while(curr != NULL){
		right = curr->next;

		if(curr->data.value == opA || curr->data.value == opB){
			/* Store the result in the left node, then unlink and free the operator and right nodes */
			left->data.value = Evaluate(curr->data.value, left->data.value, right->data.value);
			left->next = right->next;
			free(curr);
			free(right);
			curr = left->next;
		}else{
			/* Not a matching operator: move to the next number and operator */
			left = right;
			curr = left->next;
		}
	}
}

/*
 * Description:
 * Evaluate the whole expression using operator precedence:
 * first * and /, then + and -. The final result is the only node left.
 */
float32 operationPrecedence(void){
	mergeNodes('*', '/');
	mergeNodes('+', '-');
	return head->data.value;
}

/*
 * Description:
 * Free every node and empty the list.
 */
void clearNodes(void){
	struct node* next = NULL;

	while(head != NULL){
		next = head->next;
		free(head);
		head = next;
	}
}

/*
 * Description:
 * Return true when the calculator accepts input.
 * The flag becomes false after '=' and is set back to true by 'C'.
 */
bool calculatable(bool flag){
	if(flag == true) return true;
	else return false;
}

int main(void){
	float64 number=0;   /* Number currently being typed */
	uint8 op;           /* Last operator pressed */
	uint8 key;          /* Key returned by the keypad */
	bool flag=true;     /* true: calculator accepts input, false: result is shown */
	LCD_init();

	for(;;){
		key = KEYPAD_getPressedKey();

		if(digitsCount > 16) flag=false;

		/* Digit: show it and add it to the number being typed */
		if(key <= 9 && calculatable(flag)){
			LCD_integerToString(key);
			digitsCount++;
			number = (number*10) + key;
			opNumber=true;
		}
		/* '=': store the last number, calculate, and show the result on the second line */
		else if(key == '=' && opNumber==true && calculatable(flag)){
			LCD_moveCursor(1,0);
			LCD_displayCharacter(key);

			createNumber(number);
			number = 0;

			LCD_floatToString(operationPrecedence());
			clearNodes();
			opNumber=false;
			flag=false;
		/* Operator: show it and store the typed number, then the operator, in the list */
		}else if(IsOperation(key) && opNumber==true && calculatable(flag)){
			op = key;
			LCD_displayCharacter(key);
			digitsCount++;

			createNumber(number);
			createNumber(op);
			number = 0;

			opNumber=false;
		/* 'C': clear the screen, the list and the current number, and accept input again */
		}else if(key == 'C'){
			LCD_clearScreen();
			clearNodes();
			number = 0;
			opNumber = false;
			flag=true;
			digitsCount = 0;
		}
	}
}
