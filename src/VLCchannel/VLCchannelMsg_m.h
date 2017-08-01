//
// Generated file, do not edit! Created by nedtool 4.6 from src/VLCchannel/VLCchannelMsg.msg.
//

#ifndef _VLCCHANNELMSG_M_H_
#define _VLCCHANNELMSG_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>src/VLCchannel/VLCchannelMsg.msg:19</tt> by nedtool.
 * <pre>
 * message VLCmoveMsg
 * {
 *     int nodeId;
 *     // The position of the node given in meters
 *     double x;
 *     double y;
 *     double z;
 * 
 *     // The angles on the xy and xz planes. We ignore the roll
 *     double alpha;
 *     double beta;
 * }
 * </pre>
 */
class VLCmoveMsg : public ::cMessage
{
  protected:
    int nodeId_var;
    double x_var;
    double y_var;
    double z_var;
    double alpha_var;
    double beta_var;

  private:
    void copy(const VLCmoveMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLCmoveMsg&);

  public:
    VLCmoveMsg(const char *name=NULL, int kind=0);
    VLCmoveMsg(const VLCmoveMsg& other);
    virtual ~VLCmoveMsg();
    VLCmoveMsg& operator=(const VLCmoveMsg& other);
    virtual VLCmoveMsg *dup() const {return new VLCmoveMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getNodeId() const;
    virtual void setNodeId(int nodeId);
    virtual double getX() const;
    virtual void setX(double x);
    virtual double getY() const;
    virtual void setY(double y);
    virtual double getZ() const;
    virtual void setZ(double z);
    virtual double getAlpha() const;
    virtual void setAlpha(double alpha);
    virtual double getBeta() const;
    virtual void setBeta(double beta);
};

inline void doPacking(cCommBuffer *b, VLCmoveMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, VLCmoveMsg& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/VLCchannel/VLCchannelMsg.msg:31</tt> by nedtool.
 * <pre>
 * message VLCchannelSignalBegin
 * {
 *     int nodeId;
 *     double power;
 *     double carrierFreq;
 *     int modulationType;
 * }
 * </pre>
 */
class VLCchannelSignalBegin : public ::cMessage
{
  protected:
    int nodeId_var;
    double power_var;
    double carrierFreq_var;
    int modulationType_var;

  private:
    void copy(const VLCchannelSignalBegin& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLCchannelSignalBegin&);

  public:
    VLCchannelSignalBegin(const char *name=NULL, int kind=0);
    VLCchannelSignalBegin(const VLCchannelSignalBegin& other);
    virtual ~VLCchannelSignalBegin();
    VLCchannelSignalBegin& operator=(const VLCchannelSignalBegin& other);
    virtual VLCchannelSignalBegin *dup() const {return new VLCchannelSignalBegin(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getNodeId() const;
    virtual void setNodeId(int nodeId);
    virtual double getPower() const;
    virtual void setPower(double power);
    virtual double getCarrierFreq() const;
    virtual void setCarrierFreq(double carrierFreq);
    virtual int getModulationType() const;
    virtual void setModulationType(int modulationType);
};

inline void doPacking(cCommBuffer *b, VLCchannelSignalBegin& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, VLCchannelSignalBegin& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>src/VLCchannel/VLCchannelMsg.msg:38</tt> by nedtool.
 * <pre>
 * message VLCchannelSignalEnd
 * {
 *     int nodeId;
 * }
 * </pre>
 */
class VLCchannelSignalEnd : public ::cMessage
{
  protected:
    int nodeId_var;

  private:
    void copy(const VLCchannelSignalEnd& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const VLCchannelSignalEnd&);

  public:
    VLCchannelSignalEnd(const char *name=NULL, int kind=0);
    VLCchannelSignalEnd(const VLCchannelSignalEnd& other);
    virtual ~VLCchannelSignalEnd();
    VLCchannelSignalEnd& operator=(const VLCchannelSignalEnd& other);
    virtual VLCchannelSignalEnd *dup() const {return new VLCchannelSignalEnd(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getNodeId() const;
    virtual void setNodeId(int nodeId);
};

inline void doPacking(cCommBuffer *b, VLCchannelSignalEnd& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, VLCchannelSignalEnd& obj) {obj.parsimUnpack(b);}


#endif // ifndef _VLCCHANNELMSG_M_H_
