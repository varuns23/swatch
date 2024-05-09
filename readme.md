# CALO-L1 SWATCH
SWATCH (SoftWare for Automating conTrol of Common Hardware) is a software framework that is used to control and monitor electronics for upgrades of the CMS experiment; it was originally developed as part of the phase-1 upgrade to the level-1 trigger system. 

CALO-L1 is the layer-1 of the CMS experiment calorimeter level-1 trigger system.

### CaloL1 swatch on Paulaner
Instructions to login, setting up docker on paulaner machine can be found in respective sections later in the text. 
Now run following commands to get the swatch docker image and set it up:
```
ssh paulaner-via-login
docker run -it -p 3333:80 --name calol1Test --entrypoint bash gitlab-registry.cern.ch/cms-cactus/core/swatch/xdaq15-swatch14:tag-v1.6.2 
yum install protobuf-lite-devel
exit // to get back to main shell
docker start calol1Test
git clone git@github.com:varuns23/swatch.git
docker rm calol1Test:/swatch/cell/example/test/dummySystem
docker cp $pwd/swatch/dummySystem calol1Test:/swatch/cell/example/test/dummySystem
docker rm calol1Test:/swatch/dummy 
docker cp $pwd/swatch/dummy calol1Test:/swatch/dummy
docker exec -u 0 -it calol1Test bash
source scl_source enable devtoolset-8
cd /swatch/dummy
make clean
make -j2
cp lib/libcactus_swatch_dummy.so /opt/cactus/lib/.
xdaqwrapper /swatch/cell/example/test/runStandalone.sh
```

#### Few words about the cell
- The cell can be run using:
```
xdaqwrapper /swatch/cell/example/test/runStandalone.sh
```
- It can be stopped by Ctrl+C and restart it after modifications, NOTE: changes will disappear if the container is killed or removed
- After the cell is running, SWATCH cell in GUI can pick your configuration once you add in Configuration Key (string):
```
file://
```
it reads now from the files specified by the SWATCH_DEFAULT_INIT_FILE & SWATCH_DEFAULT_GATEKEEPER_XML environment variables in that configuration , and now you can set it all up through "Running" with even some dummy warnings.

#### How to add a new processor to a new card
- Assuming you start from some example and want to add new processor, you need to register new name and add new commands.
- Based on Dummy cell example it can be done by adding 4 files
   - example/DummyProcessor7.cpp
   - example/DummyProcessorCommands7.cpp
   - example/DummyProcessor7.hpp
   - example/DummyProcessorCommands7.hpp
- One can now order the execution of various processor commands by modifying "example/DummySystem.cpp" file:
```
// Create 2 local lists
std::deque<DummyProcessor*> lDps;
std::deque<DummyProcessor7*> lD7ps;
for(swatch::processor::Processor* lP : getProcessors()) {
if ( DummyProcessor* lDp = dynamic_cast<DummyProcessor*>(lP) ) lDps.push_back(lDp);
else if ( DummyProcessor7* lD7p = dynamic_cast<DummyProcessor7*>(lP) ) lD7ps.push_back(lD7p);
}

//  fsm.align.add(getProcessors(), ProcFSM_t::kStateConfigured, ProcFSM_t::kTrAlign);
fsm.align
.add(lDps, ProcFSM_t::kStateConfigured, ProcFSM_t::kTrAlign)
.add(lD7ps, ProcFSM_t::kStateConfigured, ProcFSM_t::kTrAlign);
```
   - In the above example, ID7ps will be started only after IDps is finished. 
   - Add DummyProcessor7 instead of DummyProcessor in /swatch/cell/example/test/dummySystem/dummySystem.xml for card number 7

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
