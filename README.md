Unreal Plugin interfacing NidaqServer
=====================================

This is a project plugin for Unreal Engine projects that provides a Blueprint
Function Library to interface the NidaqServer. The NidaqServer is part of the
[ARCADE suite](https://github.com/esi-neuroscience/arcade) and allows issueing 
digital events and rewards via National Instruments cards for behavioral
experiments.

For the Unreal Engine to be able to start the NidaqServer, the NidaqServer.exe
needs to be on the user's path. The provided Blueprint functions can be mapped
to any kinds of events such as `BeginPlay`, keyboard presses, etc., see for
example

![ExampleBlueprint](/Docs/ExampleBlueprint.png)

This plugin is free but copyrighted software, distributed
under the terms of the GNU General Public Licence as published by
the Free Software Foundation (either version 3, or at your option
any later version). See the file LICENSE for more details.