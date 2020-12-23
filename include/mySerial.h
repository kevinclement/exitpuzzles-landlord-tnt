#ifndef MYSERIAL_h
#define MYSERIAL_h

class Conditions;
  
class MySerial {
public:
  MySerial(Conditions &conditions);  
  void setup();
  void handle();
  void teardown();
  void printHelp();

private:
  Conditions &_conditions;
  bool _permanentPenalty = false;
};

#endif
