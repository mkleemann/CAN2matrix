CAN2matrix - AVR based project to get information from one CAN bus to another.
==============================================================================

Since new cars come with CAN bus to control anything, the need to use these
signals arose when trying to build in my old CAN controlled radio.

Single CAN solutions exist, but often they lack in something or other. So I
used the tips and tricks to combine such solutions.

The project initially is Atmel Studio 6 based. The build process is now 
changed to use cmake. All artefacts, like makefiles, will be removed and no
longer maintained. Documentation can be built via doxygen: 

$> doxygen doxygen.conf

To build out of source use:

$> mkdir -p /path/to/some/build/dir
$> cd /path/to/some/build/dir
$> cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/CAN2matrix/cmake/genric-gcc-avr.cmake /path/to/CAN2matrix
$> make

For Windows use the following call instead:

$> cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/path/to/CAN2matrix/cmake/genric-gcc-avr.cmake /path/to/CAN2matrix

This builds, but does not upload, the application. Use

$> make help

to get a list of all targets available, including the programming targets.

Both CAN buses (could) use a different bitrate. Only a small number of
signals are needed to communicate between buses. Sleep and/or power down
functionality is a must, if you don't want to help your car waking up after
a weekend. Of course, if you have access to other information, make use of
it.

Current Stage:
==============

Current state is the implementation of the dual CAN treatment. Some special
CAN ids (minimal navigation signals) are now treated and can be tested with
a CAN adapter or the real target radio. It shows, that not all IDs are right
the first time the implementation took place. This needs some fine tuning.

Some further testing is also needed to get all information from the car CAN
bus and treat them as necessary. Especially the communication to the
instrument cluster to show some station information like the original car
radio would be nice.

Additionally bus sleep detection is working to put the connected CAN
controllers (MCP2515) to sleep. For CAN1 (master) the connected MCP2551
is not going to sleep, since it is needed for detecting CAN activity. The
slave MCP2551 is also put into sleep mode and woken up with its controller.

The AVR goes into power down mode with INT0 enabled to wake it up (wake on
CAN activity).

Note: Only standard CAN frames are supported right now.

Next Steps/Ideas:
=================
(M)andatory
(O)ptional
(S)tarted Implementation
(P)artly Implemented
(T)esting and Optimizing
(D)one

- (D) Power Down (all chips including AVR) by CAN bus sleep
- (D) Wake Up by CAN activity on the (master) bus (MCP25*) - new hardware
- (D) Wake Up by CAN activity for AVR itself
- (T) Implement "The CAN Matrix" in code
- (D) dim unit using a light dependent resistor
- (S) implement CAN message filtering to get only IDs used
- (D) implement (simple) CAN error treatment/signalling
- (O) EEPROM use for message matrix
- (O) implement support for extended CAN frames

Postponed or different projects
===============================
- PDC distance showed by LEDs (see PDCViewer project)
- make use of other AVRs to include CAN2analog or Analog2CAN possibility
- turn lights using fog lights with fade out

Have fun!

M.
