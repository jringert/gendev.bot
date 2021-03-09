# Eclipse project gendev.bot.runner
Project for executing some engine for generated code and making it available for the robot to connect to via HTTP.

Currently supported engines:
* UMPLE StateMaching code (examples included)
* Spectra synthesized controllers (untested, no examples)

![schematic system overview](pics/architecture.jpg)

## Project contents
* UMPLE StateMachines are in the root of folder src (to generate code from Eclipse)

## Plug-Ins
* UMPLE code generation requires the UMPLE plug-in available from `http://cruise.umple.org/org.cruise.umple.eclipse.plugin.update.site`
* Spectra synthesis requires the Spectra Tools plug-in available from `http://smlab.cs.tau.ac.il/syntech/spectra/tools/update/`
