# VNC
Remote Control Desktop for Windows (Win32 API)

## What is it ?
A Windows software to take control of a remote Windows computer like TeamViewer or RealVNC.

### So, hum...you're recoding a VNC, why ?
The goal of this school projet is to learn Win32 API with apre-existing code base (barebones only)  
and a library for keyboard, mouse inputs. Anyway I think it's a cool and there's a lot to learn.

### Developement environment
  - Windows 7 64bit
  - Visual Studio 2015 community
  - VNCToolslib
  - C programming language

### Implementation
I am following the specification of the RFB Protocol from RealVNC.  
For the network connection, there is one socket for the commands (keyboard, mouse inputs)  
and an other one for data transmission (screen rectangle).

