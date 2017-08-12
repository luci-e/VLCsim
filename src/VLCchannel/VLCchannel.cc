//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <VLCchannel.h>
#include <VLCdevice.h>
#include <cgate.h>
#include <cchannel.h>
#include <VLCchannelMsg_m.h>
#include <VLCconnection.h>
#include <list>
#include <set>
#include <algorithm>


void VLC::VLCchannel::initialize(){};

void VLC::VLCchannel::handleMessage(cMessage *msg){
    if(msg->isSelfMessage()){

    }else{
        VLCchannelMsg *chMsg = dynamic_cast<VLCchannelMsg*>(msg);
        switch(chMsg->getMessageType()){
            case CH_MOVE_MSG:{
                VLCmoveMsg *moveMsg = dynamic_cast<VLCmoveMsg*>(msg);
                this->notifyChange((VLCdevice*)sim->getModule(moveMsg->getNodeId()));
                break;
            }
            case CH_BEGIN_COMM_MSG:{
                VLCchannelSignalBegin *msgBegin = dynamic_cast<VLCchannelSignalBegin*>(msg);
                VLCdevice * transmitter = (VLCdevice*) sim->getModule(msgBegin->getTransmitterNodeId());
                this->startTransmission(transmitter);
                break;
            }
            case CH_END_COMM_MSG:{
                VLCchannelSignalEnd *msgEnd = dynamic_cast<VLCchannelSignalEnd*>(msg);
                VLCdevice * transmitter = (VLCdevice*) sim->getModule(msgEnd->getTransmitterNodeId());
                this->endTransmission(transmitter);
                break;
            }
        }
    }
    delete msg;
};

// Nothing to do here
VLC::VLCchannel::VLCchannel() {}
VLC::VLCchannel::~VLCchannel() {}

void VLC::VLCchannel::addDevice(VLCdevice* device, cGate *deviceGateIn, cGate *deviceGateOut) {
    this->VLCdevices.insert(device);
    int newGateVSize = gateSize("devicePort")+1;

    setGateSize("devicePort", newGateVSize);

    cIdealChannel *channelIn = cIdealChannel::create(VLC::randomString(16));
    cIdealChannel *channelOut = cIdealChannel::create(VLC::randomString(16));

    gate("devicePort$o", newGateVSize-1)->connectTo(deviceGateIn, channelOut);
    deviceGateOut->connectTo(gate("devicePort$i", newGateVSize-1), channelIn);

    channelIn->callInitialize();
    channelOut->callInitialize();

    this->VLCcurrentViews[device] = this->devicesInFoVOf(device);
    this->notifyChange(device);

    VLCdeviceGates[device] = newGateVSize-1;
}

std::set<VLC::VLCdevViewInfo>* VLC::VLCchannel::devicesInFoVOf(VLC::VLCdevice* device) {
    std::set<VLCdevViewInfo> *deviceList = new std::set<VLCdevViewInfo>();

    for( auto& currentDevice : this->VLCdevices ){
        if(device != currentDevice){
            VLCdevViewInfo dvi = devsPerspective(device, currentDevice);
            if( dvi.seeEachOther){
                deviceList->insert(dvi);
            }

        }
    }

    return deviceList;
}

// Calculates the viewing info about two devices
VLC::VLCdevViewInfo VLC::VLCchannel::devsPerspective(VLCdevice* device1, VLCdevice* device2){

    VLCnodePosition dev1Position = device1->getNodePosition();
    VLCnodeDirection dev1Direction = device1->getNodeDirection();
    double dev1FoV = device1->getSemiAngle();

    //ev<<"Device1 with id "<<device1->getId()<<" position: ";
    //VLC::printPosition(dev1Position);

    //ev<<"Device1 with id "<<device1->getId()<<" vector: ";
    //VLC::printVector(dev1Direction);

    VLCnodePosition dev2Position = device2->getNodePosition();
    VLCnodeDirection dev2Direction = device2->getNodeDirection();
    double dev2FoV = device2->getSemiAngle();

    //ev<<"Device2 with id "<<device2->getId()<<" position: ";
    //VLC::printPosition(dev2Position);

    //ev<<"Device2 with id "<<device2->getId()<<" vector: ";
    //VLC::printVector(dev2Direction);

    VLCdevViewInfo devsPer;
    devsPer.device1 = device1->getId();
    devsPer.device2 = device2->getId();

    devsPer.distance = distance(dev1Position, dev2Position);

    //ev<<"Devices distance is "<<devsPer.distance<<"\n";

    // Calculate if the second device is in the FoV of the first one
    vector3d distanceVector = {dev2Position.x-dev1Position.x, dev2Position.y-dev1Position.y, dev2Position.z-dev1Position.z};
    vector3d normalisedDistance = normalise(distanceVector);

    //ev<<"1 to 2 normalised vector:";
    //VLC::printVector(normalisedDistance);

    devsPer.angle1 = acos(dotProduct3d(dev1Direction, normalisedDistance));
    bool can1see2 =  devsPer.angle1  <= dev1FoV;

    //ev<<"Can 1 see 2? "<<can1see2<<"\n";

    // Do the same for the second device
    distanceVector = opposite(distanceVector);
    normalisedDistance = normalise(distanceVector);

    //ev<<"2 to 1 normalised vector:";
    //VLC::printVector(normalisedDistance);

    devsPer.angle2 = acos(dotProduct3d(dev2Direction, normalisedDistance));
    bool can2see1 = devsPer.angle2 <= dev2FoV;

    //ev<<"Can 2 see 1? "<<can2see1<<"\n";

    devsPer.seeEachOther = can1see2 && can2see1;

    return devsPer;
}

int VLC::VLCchannel::createConnection(VLCdevice * transmitter, VLCdevice * receiver) {
    VLC::VLCconnection* conn = this->connectionExists(transmitter, receiver);
    if( !conn ){
        VLC::VLCconnection newConn(transmitter, receiver);
        this->VLCconnections.insert(newConn);
        return 0;
    }
    return -1;
}

int VLC::VLCchannel::dropConnection(VLCdevice * transmitter, VLCdevice * receiver) {
    VLC::VLCconnection* conn = this->connectionExists(transmitter, receiver);
    if(conn){
        this->VLCconnections.erase((*conn));
        return 0;
    }
    return -1;
}

int VLC::VLCchannel::abortConnection(VLCdevice* transmitter, VLCdevice* receiver) {
    VLC::VLCconnection* conn = this->connectionExists(transmitter, receiver);
    if(conn){
        conn->abortConnection();
        this->VLCconnections.erase(*conn);
        ev<<"Erased the connection\n";
        return 0;
    }
    return -1;
}

VLC::VLCconnection* VLC::VLCchannel::connectionExists(VLCdevice * transmitter, VLCdevice * receiver) {
    VLCconnection toFind(transmitter, receiver);
    std::set <VLC::VLCconnection, VLC::VLCconnection::comparator>::iterator conn = this->VLCconnections.find(toFind);
    if( conn != this->VLCconnections.end()){
        return const_cast<VLCconnection*>(&(*conn));
    }
    return NULL;
}

void VLC::VLCchannel::startTransmission(VLCdevice* transmitter) {
    std::set<VLCdevViewInfo> * receiversInFoV = this->devicesInFoVOf(transmitter);
    for( std::set<VLCdevViewInfo>::iterator receiverIt = receiversInFoV->begin(); receiverIt != receiversInFoV->end(); receiverIt++){
        VLCdevice * receiver = (VLCdevice*) this->sim->getModule((*receiverIt).device2);
        if( receiver->getDeviceType() == RECEIVER_DEVICE){
            this->createConnection(transmitter, receiver);
            ev<<"created connection between "<<transmitter->getId()<<" and "<<receiver->getId()<<"\n";
        }
    }
}

void VLC::VLCchannel::endTransmission(VLCdevice* transmitter) {
    for( std::set <VLC::VLCconnection, VLC::VLCconnection::comparator>::iterator conn = this->VLCconnections.begin(); conn != this->VLCconnections.end(); conn++){
        VLCconnection c = *conn;
        ev<<"Ze transmitter is "<<c.transmitter<<"\n";
        if(c.transmitter == transmitter){
            if( c.getOutcome() ){
                ev<<"Sending message back to ze devise\n";
                cGate * receiverGate = gate("devicePort$o", this->VLCdeviceGates[const_cast<VLCdevice*>(c.receiver)]);
                send(new cMessage(), receiverGate);
            }

            this->VLCconnections.erase(c);
        }
    }
}

void VLC::VLCchannel::updateChannel(){
    for( std::set<VLCconnection>::iterator conn = this->VLCconnections.begin(); conn != this->VLCconnections.end(); conn++){
        VLCconnection c = *conn;
        // Calculate the SINR between the devices in the connection
        // Update the connection by adding a new value to the SINRtrend
    }
}



void VLC::VLCchannel::notifyChange(VLCdevice * device) {
    std::set<VLCdevViewInfo> *newView = this->devicesInFoVOf(device);
    std::set<VLCdevViewInfo> *oldView = this->VLCcurrentViews[device];

    // Get the devices that have come into the FoV of the device
    std::set<VLCdevViewInfo> newDevices;
    std::set_difference(newView->begin(), newView->end(), oldView->begin(), oldView->end(),  std::inserter(newDevices, newDevices.begin()) );

    // Get the devices that went of the FoV of the device
    std::set<VLCdevViewInfo> outDevices;
    std::set_difference(oldView->begin(), oldView->end(), newView->begin(), newView->end(), std::inserter(outDevices, outDevices.begin()) );

    if( device->getDeviceType() == TRANSMITTER_DEVICE ){
        for(std::set<VLCconnection>::iterator conn = this->VLCconnections.begin(); conn != this->VLCconnections.end();){
            if( conn->transmitter == device){
                conn->abortConnection();
                ev<<this->VLCconnections.size()<<" connections on the net you take one down pass it around";
                this->VLCconnections.erase(conn++);
                ev<<this->VLCconnections.size()<<" connections on the net\n";
            }else{
                conn++;
            }
        }
    }else if(device->getDeviceType() == RECEIVER_DEVICE){
        for(std::set<VLCconnection>::iterator conn = this->VLCconnections.begin(); conn != this->VLCconnections.end();){
            if( conn->receiver == device){
                conn->abortConnection();
                ev<<this->VLCconnections.size()<<" connections on the net you take one down pass it around";
                this->VLCconnections.erase(conn++);
                ev<<this->VLCconnections.size()<<" connections on the net\n";
            }else{
                conn++;
            }
        }
    }

    for( std::set<VLCdevViewInfo>::iterator newDevs = newDevices.begin(); newDevs != newDevices.end(); newDevs++){
        this->VLCcurrentViews[(VLCdevice*) this->sim->getModule(newDevs->device2)]->insert(invertedView(*newDevs));
    }

    for( std::set<VLCdevViewInfo>::iterator outDevs = outDevices.begin(); outDevs != outDevices.end(); outDevs++){
        this->VLCcurrentViews[(VLCdevice*) this->sim->getModule(outDevs->device2)]->erase(invertedView(*outDevs));
    }

    delete this->VLCcurrentViews[device];
    this->VLCcurrentViews[device] = newView;
    this->updateChannel();
}

// Connect the VLCchannel to the mobility manager through an ideal channel
void VLC::VLCchannel::addMobility(cGate* mobilityGate) {
    setGateSize("mobilityPort", gateSize("mobilityPort")+1);
    cIdealChannel *c = cIdealChannel::create(VLC::randomString(16));
    mobilityGate->connectTo(gate("mobilityPort", gateSize("mobilityPort")-1), c);
    c->callInitialize();
}
