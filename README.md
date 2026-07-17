# krc-overlay

### installation

1. copy the `krc-overlay` folder to `D:\`.
2. edit the `C:\KRC\StartKRC.exe.config` file and add the following line:

```xml
...
<application filename="D:\krc-overlay\krc-overlay.exe" mainWindowId="KRCOverlay" />
...
```

the line must be placed immediately after the `SmartHMI.exe` entry, for example:

```xml
...
  <application filename="SmartHmi\SmartHMI.exe" waitType="WaitForKrcReady" timeOut="200" mainWindowId="SmartHMI" />
  <application filename="D:\krc-overlay\krc-overlay.exe" mainWindowId="KRCOverlay" />
...
```