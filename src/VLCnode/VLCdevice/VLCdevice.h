/*
 * VLCdevice.h
 *
 *  Created on: Jul 30, 2017
 *      Author: casper
 */

#ifndef VLCDEVICE_H_
#define VLCDEVICE_H_

#include <omnetpp.h>
#include <VLCcommons.h>


#define TRANSMITTER_DEVICE 0
#define RECEIVER_DEVICE 1


namespace VLC{
    // Forward declarations
    class VLCchannel;
    class VLCmobilityManager;

    class VLCdevice : public ::cSimpleModule{

        protected:
            int nodeId;
            int deviceType;
            double semiAngle;
            VLCchannel* channel;
            VLCmobilityManager* mobilityManager;

            void initialize();
            void handleMessage(cMessage *msg){};


        public:
            VLCdevice();
            virtual ~VLCdevice();

            VLCnodePosition getNodePosition();
            VLCnodeDirection getNodeDirection();

            double getSemiAngle() const;
            int getNodeId() const;
};
}

#endif /* VLCDEVICE_H_ */