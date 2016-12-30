/*
  THIS PROGRAM PROMPTS THE USER TO SELECT A WORD TO DISPLAY ON THE 7 SEGMENT DISPLAY. THE WORD SCROLLS FROM RIGHT TO LEFT.
  SWITCH 0 IS USED TO FORWARD SCROLL OR PAUSE THE WORD.
  SWITCH 1 IS USED TO CONTROL THE SPEED OF THE SCROLLING.
  USER HAS A CHOICE OF A CUSTOM MSG*/

#include<Windows.h>
#include<stdlib.h>
#include<stdio.h>
#include<DAQlib.h>

#define ON 1 
#define TRUE 1
#define OFF 0
#define FALSE 0
#define NUM_OF_LED_DISPLAYS 8

void setuparray (char [], int);
void displaying (int[], int);
int countletters (char []);
void createstring (char, char, int);

int main (void) {

	int setupnum; //DAQModule 
	int choice; //Choise of word the user desires
	char word[] = ""; //The array that contains the word
	int numofletters; //# of letters in the word of the users choice 
	char message[9] = ""; //The array that contains the word + zeros

	printf(" Enter 4 for software, or 0 for hardware: ");
	scanf("%d", &setupnum);
	printf("\n");

	if (setupDAQ (setupnum) == ON) {

		do { printf("\n YOU MAY CHOOSE THE WORD YOU WOULD LIKE TO DISPLAY. \n\n ENTER 1 FOR HELLO \n ENTER 2 FOR CLOSE \n ENTER 3 FOR HELP \n ENTER 4 FOR PERSONALIZED MESSAGE \n\n ENTER HERE: ");
		scanf("%d", &choice);
	
			if (choice == 1) {
				sprintf(word, "HELLO"); 
			}
			if (choice == 2) {
				sprintf(word, "CLOSE"); 
			}
			if (choice == 3) {
				sprintf(word, "HELP"); 
			}
			if (choice == 4) {
				printf("\n Make a word up to 8 letters long, using letters A, O, P, C, H, E, L, S \n (ALL LETTERS THAT ARE USED IN THIS PROGRAM)(CAP SENSITIVE)(NO SPACES)");
				printf("\n\n Enter a Word: "); 
				scanf("%s", word); 
		  } 
		} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);       //Repeatedly asks the user for their choice until a valid choice is entered 

		printf("\n To REVERT Text, Turn ON Switch #0\n To FORWARD Text, Turn OFF Switch #0");
		printf("\n\n To INCREASE Speed, Turn ON Switch #1 \n To DECREASE Speed, Turn OF Switch #1\n");
		printf("\n\n Close program before closing DAQ module in order to avoid crashing the program\n");
		
		numofletters = countletters (word); //Counts the # of letters that the Word has. (ex. APSC has 4) 

		//Thinking of the LED's as an array, there are 8 spaces, the first few index's are occupied by the alphabets of the word, the remaining
		//are given value '0'. Counting how many zeros are needed. (For APSC choice, the numofzeros = 8 - 4 = 4); 
		//numofzeros = NUM_OF_LED_DISPLAYS - numofletters; 

		//This function creates a string of SIZE 9 (NUM_OF_LED_DISPLAYS + 1 for \0), that has the word followed by 0's. This string is called Message. 
		//For APSC Choice, message[] = "APSC0000";
		createstring (word, message, numofletters); 

		//This function creates a new Array that has the Data Values for DisplayWrite corresponding to the Alphabets & digits in Message String in the same order.  
		//The 0's are given 0 Data Value, indicating that nothing is displayed.
		//For APSC Choice, this would create a Array datavalues[] = {238, 206, 182, 156, 0, 0, 0, 0};  
		setuparray (message, numofletters); 
	}

	else {
		printf("Error occured in seting up DAQ module"); 
	}
	
	system("PAUSE");
	return 0;
}

void setuparray ( char message[], int numofalphas) {

	char letters[] = {'P', 'S', 'C', 'A', 'E', 'L', 'H', 'O', '0'}; //Both of these arrays are in the same order. First has symbols, the second has data values. 
	static int letters_table[]= {206, 182, 156, 238, 158, 28,  110, 252, 0 }; 

	int datavalues[8]; //Arrah that contains the datavalues of the message to be displayed in the right order
	int i; //random variables for a for loop
	int j;

	for (i = 0; i <= 7; i++) {  // Goes through the messgae array 
		for (j = 0; j <= 8; j++) { //Goes through the letters array 
			if (message[i] == letters[j]) {  //If the matching letters are found
				datavalues[i] = letters_table[j];  //Copy its data value to the index of the new array Datavalue that corresponds to where that letter is found in Message. 
			}
		}
	}  

	displaying(datavalues, numofalphas); //Function that preforms the displaying on LED screen. It Displays & Scrolls the DataValue Array
}

void displaying (int datavalues[], int numofalphas) {

	int pos; //describes the position of the First Letter on the DAQ Module (for APSC, it would be A)

	while (continueSuperLoop() == TRUE) {

		for (pos = 0; pos <= 7 + numofalphas; pos++) { //Run this until the first letter has gone to MAX position off the screen for the word to go completely off.

			if (digitalRead(0) == ON) { //This  makes the word stop
				pos = pos - 1; 
				displayWrite(0, pos + 1); //Clear the LED display where the number used to be
			}

			if (pos <= 7) {
				displayWrite(datavalues[0], pos); }     //if position is within the LED Screen, then display first letter at Position. 

			if (pos >= 1 && pos <= 8) {
				displayWrite(datavalues[1], (pos-1)); }      // If position of first letter is 1 or above&& 8 or below, then display the second letter at pos - 1 . 

			if (pos >= 2 && pos <= 9) {
				displayWrite(datavalues[2], (pos-2)); }

			if (pos >= 3 && pos <= 10) {
				displayWrite(datavalues[3], (pos-3)); }

			if (pos >= 4 && pos <= 11) {
				displayWrite(datavalues[4], (pos-4)); }

			if (pos >= 5 && pos <= 12) {
				displayWrite(datavalues[5], (pos-5)); }

			if (pos >= 6 && pos <= 13) {
				displayWrite(datavalues[6], (pos-6)); }

			if (pos >= 7 && pos <= 14) {
				displayWrite(datavalues[7], (pos-7)); }

			if (pos >= 8 && pos <= 15) {
				displayWrite(0, (pos-8)); }

			if (digitalRead(1) == TRUE) {  //if Switch is ON, the slow speed. 
				Sleep(500); }

			else if (digitalRead(1) == FALSE) { //If Switch is OFF, then fast speed. 
				Sleep(1000); }
		}
	 }
}

int countletters (char word []) { //COUNTS AMOUNT OF LETTERS IN THE WORD
	int i; 
	int count = 0;
	for (i = 0; word[i] != '\0'; i++) {     
		count++;
	}
	return count;
}

void createstring ( char word[], char message[], int numlets) {

	int i;
	for (i =0; i <numlets; i++) {
		message [i] = word[i];
	}
	for (i= numlets; i < 9; i++) {
		message[i] = '0';
	}
}