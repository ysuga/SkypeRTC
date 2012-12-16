#pragma once


#include <string>
#include "skype-embedded_2.h"
#include "VideoEx.h"

class ParticipantEx : public Participant
{
 private:

 public:
  typedef DRef<ParticipantEx, Participant> Ref;
  typedef DRefs<ParticipantEx, Participant> Refs;

 private:
  bool m_Myself;
 private:
  VideoEx::Ref m_Video;

 public:
  ParticipantEx(unsigned int oid, SERootObject* root);
  ~ParticipantEx();


  void OnChange(int prop);
  void GetVideoIfAvailable();
};
