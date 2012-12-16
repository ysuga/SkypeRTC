#include "VideoEx.h"
#include <iostream>

VideoEx::VideoEx(unsigned int oid, SERootObject* root):
  Video(oid, root), m_IncomingStream(false), m_LocalPreview(false)
{

}

VideoEx::~VideoEx()
{

}


void VideoEx::OnChange(int prop)
{

#ifdef DEBUG
  std::cout << __FILE__ << ":OnChange" << std::endl;
#endif
  if ( prop == Video::P_STATUS ) {
    Video::STATUS vidStatus;
    GetPropStatus(vidStatus);
    
    if ( m_LocalPreview ) {
#ifdef DEBUG
      std::cout << "LOCAL" << std::endl;
#endif
      //      QString dbg = "LOCAL ";
      //      dbg.append((const char*)tostring(vidStatus));
      //      dispatcher->log(dbg);
      return;
    };

    //QString dbg;
    if(m_IncomingStream) {
      std::cout << "Incoming" << std::endl;
    } else {
      std::cout << "Outgoing" << std::endl;
    }
    //if ( incomingStream ) dbg = "INCOMING "; else dbg = "OUTGOING ";
    //dbg.append((const char*)tostring(vidStatus));
    //dispatcher->log(dbg);
    
    if ( vidStatus == Video::AVAILABLE ) this->Start();
    
    if ( vidStatus == Video::RUNNING ) {
      if ( m_IncomingStream ) {
	Conversation::Ref convX;
	this->GetPropConvoId(convX);
	//dispatcher->log("Incoming video stream is now running.");
	//dispatcher->SendOpenVideoWindowSignal(this->participant);
      }
    };

    if ( ( vidStatus == Video::STOPPING ) && ( m_IncomingStream ) ) {
      //      dispatcher->SendCloseVideoWindowSignal(this->participant);
    };
  };
}



