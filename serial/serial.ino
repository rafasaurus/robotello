void setup() {
  // put your setup code here, to run once:void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  Serial.println("hi");
}
void loop(){
  String str;  
  if (Serial.available()) {
    // say what you got:
    Serial.print("I received: ");
    str = Serial.readString();
    if (str == "asdf") {
      Serial.println("Yes");
      } else {
      Serial.println("No");
      }
    }
}
