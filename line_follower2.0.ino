#include <SoftwareSerial.h>
SoftwareSerial bt (3,4);

/*moving functions*/
void Stop();
void Back();
void front();
void Right();
void Left();
void turn_r();
void turn_l();
void uturn();

/*motor defination*/
#define B_mot_1 6  //right motor (in4)
#define B_mot_2 7  //right motor (in3)
#define A_mot_1 9  //left motor (in1)
#define A_mot_2 8  //left motor (in2)
#define enA 10
#define enB 5
#define power 70   // power of motor

/*ir defination*/
#define ir_f 11     // front ir
#define ir_r 12     // right ir
#define ir_l 13     // left ir

bool togg = false; //decide controling via remote or ir
int rd_ir_f;
int rd_ir_r;
int rd_ir_l;
char last_move=0;
char in = 'w';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 bt.begin(9600);
 
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(B_mot_1,OUTPUT);
  pinMode(B_mot_2,OUTPUT);
  pinMode(A_mot_1,OUTPUT);
  pinMode(A_mot_2,OUTPUT);
  pinMode(ir_f,INPUT);
  pinMode(ir_r,INPUT);
  pinMode(ir_l,INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  //if(bt.available())
  //{
//    in = bt.read();    
//    
//      if (in == 'W') // front light on
//      {
//        togg = true; //control using the app
//      }
//    
//    else if (in == 'w')  //front light off
//    {
//      togg = false; // control using ir (maze solve)
//    }

    if (togg)
  {
    Serial.println("control using the app");
    if (in == 'F') front();
    else if (in == 'B') Back();
    else if (in == 'R') Right();
    else if (in =='L') Left();
    else Stop();
  }
  else  //solve the maze
  {
    rd_ir_f = digitalRead(ir_f);
    rd_ir_r = digitalRead(ir_r);
    rd_ir_l = digitalRead(ir_l);
    Serial.println("front");
    Serial.println(rd_ir_f);
    Serial.println("right");
    Serial.println(rd_ir_r);
    Serial.println("left");
    Serial.println(rd_ir_l);
    if (rd_ir_f)   //black line (nothing ahead)
    {
      front();
    }
    if (rd_ir_r || rd_ir_l)   // any intersection or correction to the path
    {
      Stop();
      delay(500);
      if(rd_ir_f)  //still there's a road
      {
        //v.push_back(f);     // add to vector
        front();  
       // delay(500); 
        Serial.println("front after intersection");     
      }
      else if (rd_ir_r)
      {
        //v.push_back(r);
        Serial.println("turning right");
       // delay(500);
        turn_r(); 
        
      }
      else if (rd_ir_l)
      {
        //v.push_back(l);
        Serial.println("turning left");
        // delay(500);
         turn_l();
         
      }
    }
    else if(rd_ir_f==0 && rd_ir_r==0 && rd_ir_l==0)    //deadend
    {
      Serial.println("turning uturn");
     // delay(500);
      uturn();
      
      last_move = 'f';
     // last_move = v.back();     //read last move
      //v.pop_back();    
    }
    while (last_move !=0)     //there's a last move
    {
//      in = bt.read();    //check if u want to control using the app
//    
//      if (in == 'W') // front light on
//      {
//        togg = true; //control using the app
//        break;
//      }
      /*take the reading*/
      rd_ir_f = digitalRead(ir_f);
      rd_ir_r = digitalRead(ir_r);
      rd_ir_l = digitalRead(ir_l);
      
      Serial.println("correcting");
      //delay(500);

      if(rd_ir_f)
       Serial.println("correcting front");
            //     delay(500);

      Back();

      if(rd_ir_r || rd_ir_l)    //intersection
      { Stop();
       Serial.println("stop in intersection");
     // delay(500);
      
//        if (last_move =='l')
//        {
//          v.push_back('f');
//          turn_l();
//          last_move=0;    // no last move
//        }
//        else if (last_move =='r')
//        {
//          v.push_back('f');
//          turn_r();
//          last_move=0;    // no last move
//        }
//        else        // last move was front
//        {
          if (rd_ir_r)
        {
          Serial.println("turning right from loop");
            //        delay(500);

         // v.push_back('l');
          turn_r();
          last_move=0;    // no last move
        }
        else if (rd_ir_l)
        {
         // v.push_back('r');
         Serial.println("turning left from loop");
        //  delay(500);
          turn_l();
          last_move=0;    // no last move
        }
        }
        
      }
    }
  }
//}
//}
    void Back()
    {
    analogWrite(enA,power);
    analogWrite(enB,power);
    digitalWrite(B_mot_1,HIGH);
    digitalWrite(B_mot_2,0);
    digitalWrite(A_mot_1,HIGH);
    digitalWrite(A_mot_2,0);
    }
    
   void front()
   {
    analogWrite(enA,power);
    analogWrite(enB,power);
    digitalWrite(B_mot_1,0);
    digitalWrite(B_mot_2,HIGH);
    digitalWrite(A_mot_1,0);
    digitalWrite(A_mot_2,HIGH);

    }
    
    void Left()
    {
    analogWrite(enA,power);
    analogWrite(enB,power);
    digitalWrite(B_mot_1,0);
    digitalWrite(B_mot_2,HIGH);
    digitalWrite(A_mot_1,HIGH);
    digitalWrite(A_mot_2,0);

    }
  void Right()
  {
    analogWrite(enA,power);
    analogWrite(enB,power);
    digitalWrite(B_mot_1,HIGH);
    digitalWrite(B_mot_2,0);
    digitalWrite(A_mot_1,0);
    digitalWrite(A_mot_2,HIGH);
  }
  void Stop ()
    {
    analogWrite(enA,0);
    analogWrite(enB,0);
    digitalWrite(B_mot_1,0);
    digitalWrite(B_mot_2,0);
    digitalWrite(A_mot_1,0);
    digitalWrite(A_mot_2,0);

  }
  void turn_r()
  {
    Stop();
    Right();
    delay(400);

    while(rd_ir_f ==0)
    {  
    Right();
    rd_ir_f = digitalRead(ir_f);
    }
    Stop();
  }
  
void turn_l()
{
  Stop();
  Left();
  delay(400);

  while(rd_ir_f ==0)
  {
    Left();
    rd_ir_f = digitalRead(ir_f);
  }
  Stop();
  delay(100);
}
void uturn()
{
  while(rd_ir_f ==0)
  {
    Back();
    rd_ir_f = digitalRead(ir_f);
  }
  Stop();
  //delay(100);
}
