#include "SkypeEx.h"
#include "VideoEx.h"
#include <iostream>

VideoEx::VideoEx(unsigned int oid, SERootObject* root):
  Video(oid, root), m_IncomingStream(false), m_LocalPreview(false)
{
  m_pOwner = root;
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
      return;
    };

    if(m_IncomingStream) {
      std::cout << "Incoming" << std::endl;
    } else {
      std::cout << "Outgoing" << std::endl;
    }
    
    if ( vidStatus == Video::AVAILABLE ) this->Start();
    
    if ( vidStatus == Video::RUNNING ) {
      if ( m_IncomingStream ) {
	Conversation::Ref convX;
	this->GetPropConvoId(convX);
	SEString partName;
	m_Participant->GetPropIdentity(partName);
	SEString autotake(((SkypeEx*)m_pOwner)->getAutoTakeUserName().c_str());
	if(partName.equals(autotake)) {
	  std::cout << "Get DA ZE!!!" << std::endl;
	}

      }
    };

    if ( ( vidStatus == Video::STOPPING ) && ( m_IncomingStream ) ) {
      //      dispatcher->SendCloseVideoWindowSignal(this->participant);
    };
  };
}


