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

#include <VLClineMobilityManager.h>
#include <VLCmobilityManager.h>

VLC::VLClineMobilityManager::VLClineMobilityManager() {
}
;
VLC::VLClineMobilityManager::~VLClineMobilityManager() {
}
;

void VLC::VLClineMobilityManager::initialize() {
    VLCmobilityManager::initialize();
}

void VLC::VLClineMobilityManager::handleMessage(cMessage* cMsg){
    if (cMsg->isSelfMessage()) {
        VLCupdateMovMsg* movMsg = dynamic_cast<VLCupdateMovMsg*>(cMsg);
        switch (movMsg->getType()) {
        case STEP_TRANSLATION:
            break;
        case STEP_ROTATION:
            break;
        }
    } else {

    }
    delete cMsg;
}
;

void VLC::VLClineMobilityManager::moveNode() {
}


void VLC::VLClineMobilityManager::rotateNode() {
}

