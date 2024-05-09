# CALO-L1 SWATCH
SWATCH (SoftWare for Automating conTrol of Common Hardware) is a software framework that is used to control and monitor electronics for upgrades of the CMS experiment; it was originally developed as part of the phase-1 upgrade to the level-1 trigger system. 

CALO-L1 is the layer-1 of the CMS experiment calorimeter level-1 trigger system.

## Setup for Paulaner Machine:
### Login & Docker setup
```
ssh -X -l  user login.hep.wisc.edu
ssh paulaner
mkdir -p /scratch/$USER/containers
mkdir -p /afs/hep.wisc.edu/home/$USER/.local/share/
cd /afs/hep.wisc.edu/home/$USER/.local/share/
rm -r /scratch/$USER/containers #if the containers director exists
ln -s /scratch/$USER/containers .
```

### VNC server setup
- Using VNC server make life easy to run firefox on Paulaner machine, needed for SWATCH GUI. Add following lines to .ssh/config file of yourlocal machine:
```
Host paulaner-via-login
    User user
    HostName paulaner.hep.wisc.edu
    ProxyCommand ssh login.hep.wisc.edu nc %h %p
```
- In Paulaner, start the vncserver - you do this command after you stopped vncserver by hand or otherwise, using:
```
vncserver -localhost -geometry 1024x768
```
- This command, vncserver, tells you the number of your X-Windows Display, example paulaner.hep.wisc.edu:3, where :3 is your display
- Now tunnel to paulaner using this localhost:
```
ssh paulaner-via-login -L590X:127.0.0.1:590X (Here X is display port, eg. 3)
```
- You can kill you VNC server (:3) using the command:
```
vncserver -kill :3
```


### Some useful Docker commands:
- List all dockers
```
docker ps -a
```
- Start and stop the docker:
```
docker stop (or start, or rm) <container-ID> (from docker ps -a)
```
- Look inside the container using its ID:
```
- docker exec -u 0 -it <container-ID> bash
```

### First time setup
```

```

### Working
