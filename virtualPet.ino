
/*
  
  Project - Virtual Pet

  This is a DIY version of the popular Heads Up party game. To play, one person resets the RedBoard and holds the LCD
  facing away from them so that they cannot see it (usually on their forehead). The display will show a short countdown
  then display random words. The other player(s) who can see the screen must yell out clues until time runs out or the player
  guesses what word is on the screen. If they guess correctly, they can press the button on the breadboard and another word
  will be displayed.

*/

#include <LiquidCrystal.h> //the liquid crystal library contains commands for printing to the display
#include "Pet.h"

LiquidCrystal lcd(13, 12, 11, 10, 9, 8); // tell the RedBoard what pins are connected to the display

int buttonPin = 2; //pin that the button is connected to
int buzzerPin = 6; //pin for driving the buzzer

long timeLimit = 10000; //time limit for the player to guess each word
long startTime = 0;     //used to measure time that has passed for each word
int roundNumber = 0;    //keeps track of the roundNumber so that it can be displayed at the end of the game

Pet pet;

const char *menuOptions[] = {"Display Your Pet", "Feed your Pet", "Play Time"};
const int numberMenuOptions = 3;

void setup()
{

  pinMode(buttonPin, INPUT_PULLUP); //set the button pin as an input
  lcd.begin(16, 2);                 //tell the LCD library the size of the screen
  Serial.begin(9600);               //begin serial communication with the computer
  showStartSequence();              //print the start sequence text
}

void loop()
{

  int result = get_Menu_Options();
  const int DETAILS = 0;
  const int FEED = 1;
  const int PLAY = 2;

  switch (result)
  {
  case DETAILS:
    showPetDisplays();
    break;
  case FEED:
    feedOption();
    break;
  case PLAY:
    runGame();
    break;

  default:
    break;
  }
  delay(5000);
}

//--------------FUNCTIONS------------------------------

//
void showStartSequence()
{

  lcd.clear(); //clear the screen

  lcd.setCursor(0, 0);  //move the cursor to the top left corner
  lcd.print("Welcome to"); //print "Welcome to Virtual"

  lcd.setCursor(0, 1);      //move the cursor to the bottom left corner
  lcd.print("Virtual Pet"); //print "Pet World"
  delay(2000);              //Wait 2 seconds

  createPet();
}

//THIS function will display the menu options and get the userChoice and display it
int get_Menu_Options()
{

  bool isButtonPressed = false;  // boolean to check if the button is pressed
  int optionIndex = 0;           // the current option of the choices
  lcd.clear();                   //clear the screen
  lcd.print("Press the button"); //Display Choose one of options then delay for 2 seconds
  lcd.setCursor(0, 1);           // change cursor to be on next line
  lcd.print("for the option");   //Display for the option
  delay(2000);                   // display for 2 seconds
  while (!isButtonPressed)       // will run this code until the button is pressed
  {

    startTime = millis();                                             //record the time that this option started
    while (millis() - startTime < timeLimit and not(isButtonPressed)) // while there is time left
    {
      lcd.clear();                                                          //clear screen
      lcd.print(menuOptions[optionIndex]);                                  // print option
      int roundedTime = round((timeLimit - (millis() - startTime)) / 1000); //calculate the time left in the round (dividing by 1000 converts the number to seconds
      lcd.setCursor(14, 1);                                                 //set the cursor in the lower right corner of the screen
      lcd.print("  ");
      lcd.setCursor(14, 1);   //set the cursor in the lower right corner of the screen
      lcd.print(roundedTime); //print the time left in the time limit
      delay(1000);

      if (digitalRead(buttonPin) == LOW) // when the button is pressd
      {
        isButtonPressed = true;   // set the booolean that the button is pressed
        tone(buzzerPin, 272, 10); //emit a short beep when the button is pressed
        lcd.clear();
        lcd.print("You Selected"); // print out the selected option
        lcd.setCursor(0, 1);
        lcd.print(menuOptions[optionIndex]);
        delay(3000); // display for three seconds
      }
    }

    //change the option index to be next one and redo options if we reach the end of options
    if (!isButtonPressed)
    {
      if (optionIndex == (numberMenuOptions - 1))
      {
        optionIndex = 0;
      }
      else
      {
        optionIndex += 1;
      }
    }
  }
  return optionIndex; // return the option index that the user selected
}

//this function will allow the user to choose what food to eat
void chooseWhatToEat()
{
  const char *foodOptions[] = {"Chicken", "Apple", "Carrot"}; // create the food option
  const int numberOfFoodOption = 3;                           // number Of food option
  bool isButtonPressed = false;
  int optionIndex = 0;
  lcd.clear();                //clear the screen
  lcd.print("Pick the food"); //Display Choose one of options then delay for 2 seconds
  lcd.setCursor(0, 1);
  lcd.print("to feed the pet");
  int userChoice;
  delay(2000);
  while (!isButtonPressed) // will run this code until the button is pressed
  {

    startTime = millis();                                             //record the time that this option started
    while (millis() - startTime < timeLimit and not(isButtonPressed)) // while there is time left
    {
      lcd.clear(); //clear screen
      lcd.print("Option ");
      lcd.print(optionIndex + 1);
      lcd.setCursor(0, 1);
      lcd.print(foodOptions[optionIndex]);                                  // print option
      int roundedTime = round((timeLimit - (millis() - startTime)) / 1000); //calculate the time left in the round (dividing by 1000 converts the number to seconds
      lcd.setCursor(14, 1);                                                 //set the cursor in the lower right corner of the screen
      lcd.print("  ");
      lcd.setCursor(14, 1);   //set the cursor in the lower right corner of the screen
      lcd.print(roundedTime); //print the time left in the time limit
      delay(1000);

      if (digitalRead(buttonPin) == LOW) // when the button is pressd
      {
        isButtonPressed = true;   // set the booolean that the button is pressed
        tone(buzzerPin, 272, 10); //emit a short beep when the button is pressed
        lcd.clear();
        lcd.print("You Selected"); // print out the selected option
        lcd.setCursor(0, 1);
        lcd.print(foodOptions[optionIndex]);
        delay(3000); // display for three seconds
      }
    }

    //change the option index to be next one and redo options if we reach the end of options
    if (!isButtonPressed)
    {
      if (optionIndex == (numberOfFoodOption - 1))
      {
        optionIndex = 0;
      }
      else
      {
        optionIndex += 1;
      }
    }
  }

  lcd.clear();
  lcd.print("Pet is eating");
  lcd.setCursor(0, 1);
  lcd.print(foodOptions[optionIndex]);
}
void feedOption()
{
  chooseWhatToEat();
  pet.feedMe();
}

//this function will display the details of the Pet
void showPetDisplays()
{

  //Display the Pet details
  lcd.clear();
  lcd.print("Pet's Name is");
  lcd.setCursor(0, 1);
  lcd.print(pet.getName());
  delay(3000);

  //Display the Pet's age
  lcd.clear();
  lcd.print("Your pet age is");
  lcd.setCursor(0, 1);
  lcd.print(pet.getAge());
  lcd.print(" years old");
  delay(3000);

  //Dsplay the Pet weight
  lcd.clear();
  lcd.print("Pet weight's");
  lcd.setCursor(0, 1);
  lcd.print(pet.getWeight());
  lcd.print(" pounds");
  delay(3000);

  //Display the Pet image
  lcd.clear();
  lcd.print("Pet looks like");
  lcd.setCursor(0, 1);
  lcd.print(pet.getImage());
  delay(3000);

  //Display whether the Pet is hungry or not
  lcd.clear();
  if (pet.isHungry())
  {
    lcd.print("Pet is hungry");
  }
  else
  {
    lcd.print("Pet is not hungry");
  }
  delay(3000);

  //Display whether the Pet is happy or not
  lcd.clear();
  if (pet.isHappy())
  {
    lcd.print("Pet is happy");
  }
  else
  {
    lcd.print("Pet is not happy");
  }
  delay(3000);
}
//This method will  receive input from the computer and create a pet object with it
void createPet()
{
  Serial.println("Please enter the name for your pet");

  bool hasNotUserInput = true;
  while (hasNotUserInput)
  {
    if (Serial.available() > 0)
    {
      String name = Serial.readString();

      pet.setName(name);
      Serial.print("Yor pet's name is "); //print the pet name  to user
      Serial.println(name);
      hasNotUserInput = false;
      delay(3000);
    }
  }

  hasNotUserInput = true;
  Serial.println("Please enter the image that you would like your pet to look like. (Less then 16 chracters)");
  while (hasNotUserInput)
  {
    String image;
    if (Serial.available() > 0)
    {
      image = Serial.readString();
      Serial.print("Yor pet looks like this "); // print the image
      pet.setImage(image);
      Serial.println(image);
      hasNotUserInput = false;
      delay(3000);
    }
  }




  hasNotUserInput = true;
  Serial.println("Please enter a number for your pet's weight in pounds");
  while (hasNotUserInput)
  {
  
    if (Serial.available() > 0)
    {
      int weight = Serial.parseInt();
      pet.setWeight(weight);
      Serial.print("Yor Pet weight is ");
      Serial.print(weight);
      Serial.println(" pounds");
      hasNotUserInput = false;
      delay(3000);
    }
  }

  
  hasNotUserInput = true;
  Serial.end();
  Serial.begin(9600);
  Serial.println("Please enter a number for your pet's age in years");
  Serial.setTimeout(500);
  while (hasNotUserInput)
  { 
    if (Serial.available() > 0)
    {
      int age = Serial.parseInt();
      pet.setAge(age);
      Serial.print("Yor Pet age is ");
      Serial.print(age);
      Serial.println(" years");
      hasNotUserInput = false;
      delay(3000);
    }
  }

}





  //This method will run the game for our virtual pet
void runGame()
  {

    //   note  frequency
    //   c     262 Hz
    //   d     294 Hz
    //   e     330 Hz
    //   f     349 Hz
    //   g     392 Hz
    //   a     440 Hz
    //   b     494 Hz
    //   C     523 Hz

    // took notes from the digital trumpet  circuit
    const int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};
    const int numberOfNotes = 8;      // the number of different notes
    const int sizeOfSequence = 4;     // the number of notes we will run
    randomSeed(analogRead(0));        //reset the random seed
    int currentNoteIndex = 0;         //set the current note to intially be 0
    int noteSequence[sizeOfSequence]; // this array will contain the notes that will be played for each game

    // get 5 random notes that will be create a sequence
    for (int i = 0; i < sizeOfSequence; i++)
    {
      currentNoteIndex = random(0, numberOfNotes); // get a random value from 0 and 7 to be a index that represent a note
      noteSequence[i] = notes[currentNoteIndex];   //add the note in the note sequence
    }

    //This will play the song for a second a write the option on the screen
    int options[] = {1, 2, 3, 4, 5};
    bool isButtonPressed = false;
    long fourSecTimeLimit = 2000;
    lcd.clear();
    lcd.print("Playing Game");
    lcd.setCursor(0, 1);
    lcd.print("Now");
    delay(5000);
    for (int i = 0; i < sizeOfSequence; i++) // play each note in the seqeunece for two seconds
    {
      startTime = millis();                    //record the time that this option started
      while (millis() - startTime < timeLimit) // while there is time left
      {

        lcd.clear();
        lcd.print("Option "); // write in the lcd screen option number
        lcd.print(options[i]);
        lcd.print("  ");
        lcd.print(noteSequence[i]);
        tone(buzzerPin, noteSequence[i]);                                     // play the buzzer
        int roundedTime = round((timeLimit - (millis() - startTime)) / 1000); //calculate the time left in the round (dividing by 1000 converts the number to seconds
        lcd.print("  ");
        lcd.setCursor(14, 1);   //set the cursor in the lower right corner of the screen
        lcd.print(roundedTime); //print the time left in the time limit
        delay(1000);

        if (digitalRead(buttonPin) == LOW) // when the button is pressd
        {
          isButtonPressed = true;   // set the booolean that the button is pressed
          tone(buzzerPin, 272, 10); //emit a short beep when the button is pressed
          lcd.clear();
          lcd.print("You Selected"); // print out the selected option
          lcd.setCursor(0, 1);
          lcd.print("Option ");
          lcd.print(options[i]);
          delay(3000); // display for three seconds
        }
      }
      // turn off the buzzer for two seconds
      noTone(buzzerPin);
      delay(2000);
    }

    //generate question and print it out
    int mutipleChoiceOptions[] = {-1, -1, -1, -1, -1}; // create an array of mutiple choice options
    randomSeed(analogRead(0));                         //reset the random seed (Arduino needs this to generate truly random numbers
    int answerNoteIndex = random(0, sizeOfSequence);   // generate a number from 0-4
    int answerNote = noteSequence[answerNoteIndex];
    long fiveSecTimeOut = 5000;
    lcd.clear();
    lcd.print("Which note play"); // print on the lcd screen the question : "Which note played at A"
    lcd.setCursor(0, 1);
    lcd.print("at Option ");
    lcd.print(options[answerNoteIndex]);
    delay(fiveSecTimeOut);

    //Create 5 different mutiple choice options

    bool containAnswer = false;
    for (int i = 0; i < sizeOfSequence; i++)
    { //do this until all 5 postions are filled

      int currentNote = 0;      //variable to hold the current number
      boolean hasMatch = false; //does the currentNumber match any of the previous numbers?

      //generate random numbers until you've generated one that doesn't match any of the other numbers in the array
      do
      {
        currentNote = notes[random(0, numberOfNotes)]; //generate a random number from 0 to 7
        hasMatch = false;                              //we haven't checked for matches yet, so start by assuming that it doesn't match
        for (int i = 0; i < sizeOfSequence; i++)
        { //for all 5 numbers in the array
          if (currentNote == mutipleChoiceOptions[i])
          {                  //does the currentNote match any of the numbers?
            hasMatch = true; //if so, set the match variable to true
          }
          if (mutipleChoiceOptions[i] == answerNote)
          {
            containAnswer = true;
          }
        }
      } while (hasMatch == true);            //if the match variable is true, generate another random number and try again
      mutipleChoiceOptions[i] = currentNote; //if the match variable is false (the new number is unique) then add it to the sequence
    }

    //if the mutiple choice options does not contain the answer note then replace one of the options as the answer

    if (containAnswer)
    {
      int randomIndex = random(0, sizeOfSequence); // generate a number from 0-4
      mutipleChoiceOptions[randomIndex] = answerNote;
    }

    bool userHasNotAnswer = true;
    //This code is to display the mutiple choice options
    const char *choiceOptions[] = {"A", "B", "C", "D", "E"};
    int i = 0;
    int userChoice = 0;
    while (userHasNotAnswer)
    {
      lcd.clear();
      lcd.print("Option "); // write in the lcd screen option number
      lcd.print(choiceOptions[i]);
      lcd.print("  ");
      lcd.print(mutipleChoiceOptions[i]);
      tone(buzzerPin, mutipleChoiceOptions[i]); // play the buzzer

      delay(5000); // delay for 5 second

      if (digitalRead(buttonPin) == LOW)
      {                            // when a user has press the button
        userHasNotAnswer = false;  //set the boolean to false
        tone(buzzerPin, 272, 10);  //emit a short beep when the button is pressed
        lcd.clear();               // clear screen
        lcd.print("You Selected"); // print on the screen that You selected Option X
        lcd.setCursor(0, 1);
        lcd.print("Option ");
        lcd.print(choiceOptions[i]);
        delay(10000);
        userChoice = mutipleChoiceOptions[i];
      }
      if (userHasNotAnswer)
      {
        i = i + 1;
        if (i == sizeOfSequence)
        {
          i = 0;
        }
      }
    }

    //check To see if user is right
    lcd.clear();
    lcd.print("Results");
    delay(2000);

    lcd.clear();
    lcd.print("The note play"); // print on the lcd screen the question : "Which note played at A"
    lcd.setCursor(0, 1);
    lcd.print("at Option ");
    lcd.print(options[answerNoteIndex]);
    lcd.print("= ");
    lcd.print(answerNote);
    delay(3000);

    lcd.clear();
    lcd.print("You Selected"); // print on the lcd screen the question : "Which note played at A"
    lcd.setCursor(0, 1);
    lcd.print("at Option ");
    lcd.print(choiceOptions[answerNoteIndex]);
    lcd.print("= ");
    lcd.print(userChoice);
    delay(3000);

    lcd.clear();
    if (userChoice == answerNote)
    {
      lcd.print("You were correct");
    }
    else
    {
      lcd.print("You were wrong");
    }
    delay(5000);
  }
