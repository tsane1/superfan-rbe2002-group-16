/* FILENAME: SuperfanFinal.ino
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

#include "Firefight.h"

boolean found, extinguished;
Fan f(/*TODO: Choose pins*/);

void setup() {
  found = f.scan();
}

void loop() {
  if(!found) found = f.scan();
  
}
