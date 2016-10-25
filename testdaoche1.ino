unsigned char hexdata[3]={0x88,0x88,0x00};
int i = 0;
int l = 0; //left radar
int m1 = 0;//front1 radar
int m2 = 0;//front2 radar
int  r = 0;//right radar
unsigned int sensers[4] = {l,m1,m2,r};
int currentCounts = 0;
int lastCounts = 0;
bool needStop = true;
void setup() {  
  Serial1.begin(4800);  
  Serial.begin(9600);
//  Serial.begin(4800);
  while(Serial1.read()>= 0){}//clear serial port  
//  while(Serial.read()>= 0){}//clear serial port  
} 

void setupParameters() {
  l = 0;
  m1 = 0;
  m2 = 0;
  r = 0;
  lastCounts = 0;
}

void currentCount () {
  currentCounts = count();
}

void lastCount () {
  lastCounts = count();
}

int count () {
  
  return (sensers[0] * 1000 + sensers[1] * 100 + sensers[2] * 10 + sensers[3]);
}

void stopRobot () {
  if (needStop) {
    Serial.println("s");
    delay(1000);
    i = 0;
  }
}

void straight () {
  Serial.println("b");
  i = 0;
}

void back () {
    Serial.println("a");
    i = 0;
}

void turnRight () {
    Serial.println("r");
    i = 0;
}

void turnLeft () {
    Serial.println("l");
    i = 0;
}
   
void loop() {  
  if(Serial1.available()>0) {  
      delay(100);  
     Serial1.readBytes(hexdata,3); 
     setupParameters();
    sensers[0] = hexdata[0]&0x0f;
    sensers[1] = ((hexdata[0]&0xf0) >> 4);
    sensers[2] = (hexdata[1]&0x0f);
    sensers[3] = ((hexdata[1]&0xf0) >> 4);
    for(int index = 0;index < (sizeof(sensers)/sizeof(sensers[0]));index++ ) {
      (sensers[index] > 2) ? sensers[index] = 2 : (sensers[index] = sensers[index]);
    }
    
  for(int i=0; i < (sizeof(hexdata)/sizeof(hexdata[0])); i++){  
        hexdata[i]='\0'; 
      //  Serial.read(); 
    }  

    currentCount();
//    Serial.print(currentCounts);
    //判断机器人3段范围内是否有障碍物
//    Serial.print();
    if (currentCounts >= 2222) {//无障碍物直行
      straight();
    }else {//有障碍物
      //取出距离最近的一个传感器和其对应的数值
    int minIndex = 0;
    int minSenserState = sensers[0];
    for (int index = 0;index < (sizeof(sensers)/sizeof(sensers[0]));index++) {
      if(minSenserState > sensers[index]) {
        minSenserState = sensers[index];
        minIndex = index;
      }
    }
//    Serial.print("count:");
//    Serial.print(currentCounts);
//    Serial.print("minIndex:");
//  Serial.print(minIndex);
//  Serial.print("minSenserState:");
//  Serial.println(minSenserState);
  if (minSenserState == 1) {//如果是在1段范围内
      //根据距离最近的那个传感器，执行相应操作
      if (currentCounts == 1221) {//前方无障碍物，左右为1段范围时，直行
        straight();
      }else {
         switch (minIndex) {
    //left
        case 0:
          turnRight();
        break;
    
    //straight1
        case 1:
          turnRight();
        break;
    
    //straight2
        case 2:
          turnLeft();
        break;
    
    //right
        case 3:
          turnLeft();
        break;
   }
      }
  
  }else if (minSenserState == 0) {//如果是0段范围内
      //根据距离最近的那个传感器，执行相应操作
     switch (minIndex) {
      //left
      case 0:
        back();
      break;
    
      //straight1
      case 1:
        back();
      break;
    
      //straight2
      case 2:
        back();
      break;
    
      //right
      case 3:
        back();
      break;
    }
  }

  }
  //清零
  while(Serial1.read()>= 0){};

    }
}  
