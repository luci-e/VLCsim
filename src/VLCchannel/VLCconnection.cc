/*
 * VLCconnection.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: casper
 */

#include <VLCconnection.h>
#include <VLCpacket_m.h>
#include <VLCchannel.h>
#include <math.h>
#include <map>
#include <string>
#include <vector>
#include <VLCtransmissionModels.h>
#include <VLCtransmitter.h>
#include <VLCcommons.h>
#include <Log.h>

namespace VLC {

long unsigned int VLCconnection::connectionCounter = 0;

VLCconnection::VLCconnection(VLCdevice *  transmitter, VLCdevice *  receiver, VLCchannel* channel) : transmitter(dynamic_cast<VLCtransmitter*>(transmitter)), receiver(dynamic_cast<VLCreceiver*>(receiver)), channel(channel){
    // Dummy connections are built with NULL that would cause a segfault
    if( channel != NULL){

        // Assign an Id to this connection
        this->connectionId = connectionCounter;
        connectionCounter++;

        this->gainConstantPart = (( this->transmitter->getLambertianOrder() + 1.0 ) * this->receiver->getPhotoDetectorArea()) / ( 2.0 * M_PI );
        //ev<<"LambertianOrder: "<<this->transmitter->getLambertianOrder()<<" PhotoDetectorArea: "<<this->receiver->getPhotoDetectorArea()<<"\n";
        this->transmissionInfo = this->transmitter->getCurrentTransmissionInfo();
        this->updateConnection();
    }
}
// Add a new value to the SINR trend
void VLCconnection::calculateNextValue() {
    // TODO: magic responsivity number
    double receivedSignalPower = this->connectionGain * std::pow(10.0, ((this->transmissionInfo["transmissionPower"]) - 30) / 10.0);

    //ev<<"Received signal power is "<<receivedSignalPower<<"\n";
    //ev<<"Noise variance is "<<this->receiver->getNoiseVariance(receivedSignalPower)<<"\n";

    double SINR =  ((receivedSignalPower * 0.003) * (receivedSignalPower * 0.003)) / this->receiver->getNoiseVariance(receivedSignalPower);
    double SINRdB = 10.0 * log10(SINR);

    this->SINRTrend.push_back(VLCtimeSINR(simTime().dbl(), SINRdB));

    VLCdevViewInfo viewInfo = this->channel->getDevViewInfo(this->transmitter, this->receiver);
    LOGN << this->connectionId << ";"
            << viewInfo.device1 << ";"
            << viewInfo.device2 << ";"
            << simTime().dbl() << ";"
            << viewInfo.distance << ";"
            << viewInfo.angle1 << ";"
            << viewInfo.angle2 << ";"
            << SINRdB;
}

// Updates the values used to calculate the SINR
void VLCconnection::updateConnection() {
    VLCdevViewInfo viewInfo = this->channel->getDevViewInfo(this->transmitter, this->receiver);
    //ev<<"Distance is now "<<viewInfo.distance<<"\n";
    // TODO: Find the function for the optical filter gain and put it as part of the equation!
    this->connectionGain = (this->gainConstantPart / (viewInfo.distance * viewInfo.distance)) * std::pow(cos(viewInfo.angle1), transmitter->getLambertianOrder()) * 1.0 * this->receiver->opticalGain(viewInfo.angle2) * cos(viewInfo.angle2);
    //ev<<"GainConstantPart: "<<this->gainConstantPart<<\
            "\nConnectionGain: "<<this->connectionGain<<\
            "\nDevice distance is "<<viewInfo.distance<<\
            "\nReceivedPower: "<<this->connectionGain * this->transmissionInfo["transmissionPower"]<<"\n";
}

// A connection equals another iff the transmitter and receiver are the same
inline bool VLCconnection::operator==(const VLCconnection &c2) const{
    return ((this->transmitter) == (c2.transmitter)) && ((this->receiver) == (c2.receiver));
}


// C1 is lt C2 iff (c1.transmitter < c2.transmitter) or ((c1.transmitter == c2.transmitter) and (c1.receiver < c2.receiver ))
bool VLCconnection::operator<(const VLCconnection& c2) const {
    if(this->transmitter->getId() < c2.transmitter->getId()){
        return true;
    }

    if(this->transmitter->getId() == c2.transmitter->getId()){
        return this->receiver->getId() < c2.receiver->getId();
    }

    return false;
}

VLCconnection::~VLCconnection() {}

// Compute the outcome of this transmission, returns true if it was successful false otherwise
bool VLCconnection::getOutcome() {
    // Given the SINR trend and the parameters of the connection return the outcome of the transmission
    return VLC::packetTransmitted(this->SINRTrend, this->transmissionInfo);
}

// Abort this connection
void VLCconnection::abortConnection() const{
    ev<<"oh noes!\n";
}

} /* namespace VLC */


