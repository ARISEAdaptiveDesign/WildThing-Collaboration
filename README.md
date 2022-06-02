# Arise_WildThing
Controls for Wild Thing Conversion

2022-05-30 v4.0 (VNH5019) from v3.3 (Spark)
General statement: Reset baseline back to VNH5019 and added wiring diagrams
Details
* Added Pull Down Resistors to electrical circuit
* Updated Software with v3.3 features and tuning
* Created Diagram Wiring for Backpack
* Changed Shield wiring from JST connectors to "soldered wire to connector"
* Deleted Green "Running" LED

2022-04-13 v3.3 GT's tuning
We have V3.0 for VNH5019 motor driver, but then made updates in SPARK version (for Grant's machine) to bring up to v3.0 software (with Pot and better tether logic). We then found software improvements for low power settings and made changes (Spin & Turn were updated to have forward vs backward tuning). So we want to merge these changes back to V3.0 for VNH5019 controller.
Also we found signal drift on analog signals (switched and joysticks) so added pulldown resistors back in.

2022-04-11 Debugged wiring and added pulldown resistors to all analog inputs. Setup for SPARK controller configuration

2022-03-21 added the DualVNH5019MotorShield dual motor controller to the list of configurable motor drivers.
The wiring diagram is setup specifically for the DualVNH5019MotorShield but other motor drivers might still be used (aka , "Amazon" & SPARK) but would need wiring diagrams to be used.

This version will be integrated into a "Backpack" electronics for the Arise_WildThing dubbed v3. It is intended that 8-10 of these versions will be made.

Software improvements yet to be made include:
  * cleanup the functions & variables
  * incorporate different user interface (RF, Sip-N-Puff, 2Button, 3Button, 4Button, Nintendo controller, Phone App, Eye Gaze, etc). This would best be done by simplifying/unifying the interface so that any combination of I/O could be used.
  * AutoLearn (or procedural Learn Sequence) to calibrate Joystick Orientation
  * More Software comments would be helpful

  Further development can be made by creating our own electronics PCB but that requires Electronic Hardware Engineering beyond the current scope.
