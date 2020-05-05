import processing.serial.*;


Serial myPort;  // Create object from Serial class
float inByte =0;
float data;      // Data received from the serial port in degrees
float rad;
float needle;
float dia;
float nx,ny;
PImage imgCompassRing;
float offset ;

void setup(){
  size(300,300);
  
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
  stroke(255);
  
  //rad = min(width, height) / 2;
  rad = 100;
  needle= rad * 0.60;
  dia = rad * 1.8;
  
  nx= (width/2);
  ny=(height/2);
  
  
  
}

void draw(){
 background(0);
 imgCompassRing = loadImage("compassRing.png");
 image(imgCompassRing, 18.5, 18);
 if ( myPort.available() > 0) {  // If data is available,
    data= myPort.read();         // read it and store it in val
    println(data);
  }
 fill(80);                       // compass background
 noStroke();
 ellipse(nx,ny,dia+5,dia+5);
  
 
  
  //needle sketch
  stroke(255);
  line(nx, ny, nx + (cos(data) * rad), ny + (sin(data) * rad));
  
  delay(5);
}




void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {
    // trim off any whitespace:
    inString = trim(inString);
    // convert to an int and map to the screen height:
    inByte = float(inString);
    //println(inByte);
    //inByte = map(inByte, 0, 360, 0, height);
  }}
  