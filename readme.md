# SWATCH
SWATCH (SoftWare for Automating conTrol of Common Hardware) is a software framework that is used to control and monitor electronics for upgrades of the CMS experiment; it was originally developed as part of the phase-1 upgrade to the level-1 trigger system.

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



### First time setup
```

```

### Working
