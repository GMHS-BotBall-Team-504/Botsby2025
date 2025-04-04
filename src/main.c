#include <kipr/wombat.h>
#include <stdlib.h>

const int lightPort = 0;
int lm = 3;  //left motor
int rm = 0;  // right motor

int left_ir = 5; //left ir CHECK SENSORS
int right_ir = 0; //right ir CHECK SENSORS
int base = 0;
int arm = 1;
int claw = 2;



//movement
//int speed = 200; //motor speed
int fiddle_speed = 50; //used to adjust for stopline
int turn_speed = 100; //used for turn function


//servo values
int realBase = 1024;
int realArm = 1024;
int realClaw = 1024;

int foodBase = 1150; //was 1170
int foodArm = 1850;
int openClaw = 300;
int closeClaw = 1390;

int dropBase = 1400;
int dropArm = 1400;

//sensor values
int tape = 2900; //black tape on ir sensors

void drive(int direction, int speed, int distance); // 2000 for distance, 22.4 cm  INPUT IN CM
void turn(int direction, int turn_speed, int distance);
void collegeBotsby(int speed, int fiddle_speed);
void collegeBotsbyBack(int speed, int fiddle_speed);
void startServos();
void driveWall(int speed);
void lineFollowRight(int distance, const int correction);
void lineFollowLeft(int distance, const int correction);
void startServos();
int set_servos (int basePos, int armPos, int clawPos, double delay);

int speed = 1000;
int tSpeed = 500;

int main()
{
    disable_servos();
    msleep(1000);
    startServos();
    printf("set");
    set_servos(foodBase, foodArm, openClaw, 2 );
    collegeBotsby(speed, 30);
   
     drive(1, speed, 32);
    set_servos(foodBase, foodArm, closeClaw, 2);
    msleep(400); //picked up hot dog
    
    collegeBotsbyBack(speed, 30);
    turn(-1, tSpeed, 1095);
    drive(1, speed, 100);
    turn(-1, tSpeed, 1095);
    set_servos(dropBase, dropArm, openClaw, 2); //dropped off hot dog
    set_servos(foodBase, foodArm, openClaw, 2);
    disable_servos();    
    drive(1, speed, 6); 
    
    turn(-1, tSpeed, 1000);
    collegeBotsby(speed, 40); //on bump line
   
      
    drive(1, speed, 45);
    collegeBotsby(speed, 30); //at vertical 1 facing left
    
    drive(1, speed, 15);
    turn(-1, tSpeed, 1095);
    collegeBotsby(speed, 20); //at horizontal black
    drive(1, speed, 30);
    turn(-1, tSpeed, 50);
    drive(1, speed, 3);
    set_servos(foodBase, foodArm, closeClaw-150, 2); //grabbed taco
    drive(-1, speed, 3);
    turn(1, tSpeed, 50);

    
   // turn(-1, tSpeed, 200);
    //drive(-1, speed, 5);
   // turn(1, tSpeed, 200);
    
    collegeBotsbyBack(speed, 30);
    turn(-1, tSpeed, 1095);
    drive(1, speed, 60);
    turn(-1, tSpeed, 870);
    set_servos(dropBase, dropArm, openClaw, 2); //dropped off taco
    
    set_servos(foodBase, foodArm, openClaw, 2);
    collegeBotsbyBack(speed, 100);
    drive(1, speed, 14); //ready to move back
    turn(-1, tSpeed, 1095);
    collegeBotsby(speed, 50);
    drive(1, speed, 22);
    turn(-1, tSpeed, 1095); 
    collegeBotsby(speed, 60); //ready to grab
    
    drive(1, speed, 30);
    turn(1, tSpeed, 300);
    drive(1, speed, 5);
    set_servos(foodBase, foodArm, closeClaw, 2); //grabbed burger
    drive(-1, speed, 5);
    turn(-1, tSpeed, 300);
    collegeBotsbyBack(speed, 50); //at horizontal black
    
    drive(-1, speed, 8);
    turn(-1, tSpeed, 1095);
    collegeBotsby(speed, 50);
    drive(1, speed, 80);
    collegeBotsby(speed, 50);
    drive(1, speed, 40);
    turn(-1, tSpeed, 1200);
    set_servos(dropBase, dropArm, openClaw, 2);
    set_servos(foodBase, foodArm, openClaw, 2); //burger off
    
    drive(-1, speed, 5); //back up a little
    turn(-1, tSpeed, 2190);
    collegeBotsby(speed, 30);
    
    disable_servos();
    
   
    return 0;
}




//--------------------------------------------------------------------------------------------------------------------------

void collegeBotsby (int speed, int fiddle_speed){
	while( (analog(left_ir) < tape)  &&  (analog(right_ir) < tape) ){
		mav(lm, speed);
        mav(rm, speed);		
		msleep(10);
	}
    
    //if left sensor hits tape first
    if ( (analog(left_ir) > tape)  &&  (analog(right_ir) < tape) ){
    	//Stop tuahs left
        while(analog(right_ir) < tape ){
            while(analog (left_ir) > tape ) {
                freeze(rm);
                mav(lm, -fiddle_speed);
                msleep(1);
            }
            while(analog(left_ir) < tape ) {
            mav(lm, fiddle_speed);
            mav(rm, fiddle_speed);	
            }
        }
    }
    //if right sensor hits tape first
    if ( (analog(left_ir) < tape)  &&  (analog(right_ir) > tape) ){
    	//Stop tuahs right
        while(analog(left_ir) < tape ){
            while(analog (right_ir) > tape ) {
                freeze(lm);
                mav(rm, -fiddle_speed);
                msleep(1);
            }
            while(analog(right_ir) < tape ) {
            mav(lm, fiddle_speed);
            mav(rm, fiddle_speed);	
            }
        }
    }
    ao();
}

void collegeBotsbyBack (int speed, int fiddle_speed){
	while( (analog(left_ir) < tape)  &&  (analog(right_ir) < tape) ){
		mav(lm, -speed);
        mav(rm, -speed);		
		msleep(10);
	}
    
    //if left sensor hits tape first
    if ( (analog(left_ir) > tape)  &&  (analog(right_ir) < tape) ){
    	//Stop tuahs left
        while(analog(right_ir) < tape ){
            while(analog (left_ir) > tape ) {
                freeze(rm);
                mav(lm, fiddle_speed);
                msleep(1);
            }
            while(analog(left_ir) < tape ) {
            mav(lm, -fiddle_speed);
            mav(rm, -fiddle_speed);	
            }
        }
    }
    //if right sensor hits tape first
    if ( (analog(left_ir) < tape)  &&  (analog(right_ir) > tape) ){
    	//Stop tuahs right
        while(analog(left_ir) < tape ){
            while(analog (right_ir) > tape ) {
                freeze(lm);
                mav(rm, fiddle_speed);
                msleep(1);
            }
            while(analog(right_ir) < tape ) {
            mav(lm, -fiddle_speed);
            mav(rm, -fiddle_speed);	
            }
        }
    }
    ao();
}

void driveWall(int speed){
    mav(lm, speed);
    mav(rm, speed);
    msleep(300);
    ao();
}

void drive(int direction, int speed, int distance)
{
    cmpc(lm);
    cmpc(rm);
    distance = distance * 100.015873;  //WAS 101.1206896
    if (direction > 0) //forward if 1
    { 
        while(( ( abs(gmpc(rm)) + abs(gmpc(lm)) ) / 2 ) < distance) // takes the clear motors clear value and moves until its reached the distance
        {
            mav(lm,speed);
            mav(rm,speed);
        }
    }
    if (direction < 0) //back if 1
    { 
        while(( ( abs(gmpc(rm)) + abs(gmpc(lm)) ) / 2 ) < distance) // takes the clear motors clear value and moves until its reached the distance
        {
            mav(lm,-speed);
            mav(rm,-speed);
        }
    }
    ao();
    cmpc(lm);
    cmpc(rm);
}
void turn (int direction, int turn_speed, int distance)   // positive = CCW, negative = CW +=left - = right
{
    cmpc(lm);
    cmpc(rm);
    if (direction < 0) //turn left
    {
    	while(( (abs(gmpc(rm)) + abs(gmpc(lm)) ) / 2) < distance)
        {
       	    mav(lm,abs(turn_speed));
            mav(rm,-abs(turn_speed));
        } 

    }
    if (direction > 0) // turn right
    {
        while(( (abs(gmpc(rm)) + abs(gmpc(lm)) ) / 2) < distance)
        {
            mav(lm,-abs(turn_speed));
            mav(rm,abs(turn_speed));
        } 

    }
    cmpc(lm);
    cmpc(rm);
    ao();
}



void lineFollowRight(int distance, const int correction){
 distance = distance * 100.015873;
    
 while( (analog(left_ir) < tape)  &&  (analog(right_ir) < tape) ){
		mav(lm, speed);
        mav(rm, speed);		
		msleep(10);
	}
    ao();
    cmpc(lm);
    cmpc(rm);
    if (analog(right_ir) > tape){
        while (gmpc(lm) < distance){
            if ((analog(right_ir) > tape)){
            mav(lm, speed);
            mav(rm, speed+correction);
            msleep(10);
                ao();
            }
            if ((analog(right_ir) < tape)){
            mav(lm, speed+correction);          
        	mav(rm, speed);
        	msleep(10);
            ao();
    		}
    	}
    }
}

void lineFollowLeft(int distance, const int correction){
 distance = distance * 100.015873;
    
 while( (analog(left_ir) < tape)  &&  (analog(right_ir) < tape) ){
		mav(lm, speed);
        mav(rm, speed);		
		msleep(10);
	}
    ao();
    cmpc(lm);
    cmpc(rm);
    if (analog(left_ir) > tape){
        while (gmpc(lm) < distance){
            if ((analog(left_ir) > tape)){
            mav(lm, speed+correction);
            mav(rm, speed);
            msleep(10);
                ao();
            }
            if ((analog(left_ir) < tape)){
            mav(lm, speed);          
        	mav(rm, speed+correction);
        	msleep(10);
            ao();
    		}
    	}
    }
}

void startServos(){
	enable_servos();
    //whatever you set the servo position to, change these to match
    realBase = 1024;
    realArm = 1024;
    realClaw = 1024;
    set_servo_position(base, 1024);
    set_servo_position(arm, 1024);
    set_servo_position(claw, 1024);
    msleep(2000);

	disable_servos();
}

int set_servos (int basePos, int armPos, int clawPos, double delay) {
    //guard clause, if base, arm, or claw is too small or large, function exits with -1
    if (basePos < 50 || basePos > 2000 || armPos < 50 || armPos > 2000 || clawPos < 50 || clawPos > 2000) return -1;
   
    //initializing
    int baseD;
    int armD;
    int baseDiff = basePos - realBase;
    int armDiff = armPos - realArm;
   
    if (baseDiff < 0)
    {
        baseD = -2;
    }
    else
    {
        baseD = 2;
    }
    if (armDiff < 0)
    {
        armD = -2;
    }
    else
    {
        armD = 2;
    }
   
   
    //Move base and arm first
    enable_servo(base);
    enable_servo(arm);

    while (realBase != basePos || realArm != armPos)
    {
        if (realBase != basePos) {
            realBase = realBase + baseD;
            set_servo_position(base, realBase);
            printf("setting position to: ");
            printf("%d", realBase);
            printf("\n");
        }
        if (realArm != armPos) {
            realArm = realArm + armD;
            set_servo_position(arm, realArm);
            printf("setting position to: ");
            printf("%d", realArm);
            printf("\n");
        }


        msleep(delay);
    }
    
    //Move claw instantly
    enable_servo(claw);
    set_servo_position(claw, clawPos);
    msleep(200);
    disable_servo(base);
    disable_servo(arm);
   
    return 0;
}
