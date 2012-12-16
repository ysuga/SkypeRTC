#include "ParticipantEx.h"
#include "SkypeEx.h"
#include <iostream>

ParticipantEx::ParticipantEx(unsigned int oid, SERootObject* root) : Participant(oid, root)
{
  SEString accountName;
  SEString partName;
  SkypeEx* skype = (SkypeEx*)root;
  accountName = skype->getSkypename();
  this->GetPropIdentity(partName);
  m_Myself = ( accountName.equals(partName) );

  GetVideoIfAvailable();
}


ParticipantEx::~ParticipantEx()
{

}


void ParticipantEx::OnChange(int prop)
{
  //  std::cout << __FILE__ << ":onChange called." << std::endl;
  if (prop == Participant::P_VIDEO_STATUS ) GetVideoIfAvailable();
}

void ParticipantEx::GetVideoIfAvailable()
{
  std::cout << __FILE__ ":GetVideoIfAvailable" << std::endl;
  Participant::VIDEO_STATUS partVidStatus;
  this->GetPropVideoStatus(partVidStatus);

  if (partVidStatus == Participant::VIDEO_AVAILABLE) {
    this->GetVideo(m_Video);
    m_Video.fetch();
    m_Video->setIncomingStream(!this->m_Myself);
    m_Video->setParticipant(this->ref());

    Video::STATUS vidStatus;
    m_Video->GetPropStatus(vidStatus);
    if ( vidStatus == Video::AVAILABLE ) m_Video->Start();

  }
}
