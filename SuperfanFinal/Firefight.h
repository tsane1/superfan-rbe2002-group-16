/* FILENAME: Firefight.h
 * 
 * BRIEF: 
 * 
 * AUTHORS:
 * @author Tanuj Sane
 * @author Dominic Cupo
 * @author Annie Hernandez
 * @author Patrick Murphy
 * 
 * START DATE: Dec. 12, 2015
 */
 
enum ScanStates {
  FIRE, SCANNING, NO_FIRE
};
enum FireStatus {
  FLAME_ON, FLAME_OFF
};

class Fighter{
  public:
    Fighter(); // default constructor
    int sweep(); // Function to scan for fire using stepper
    int fight
  private:
    
};

