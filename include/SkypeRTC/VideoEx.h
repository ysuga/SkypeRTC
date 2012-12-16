#pragma once

#include <string>
#include "skype-embedded_2.h"

//#include "ParticipantEx.h"


//class SkypeEx;

class ParticipantEx;

class VideoEx : public Video
{
 private:
  SERootObject* m_pOwner;
 public:
  typedef DRef<VideoEx, Video> Ref;
  typedef DRefs<VideoEx, Video> Refs;
  typedef DRef<ParticipantEx, Participant> ParticipantRef;

 private:

  bool m_IncomingStream;
  bool m_LocalPreview;
  ParticipantRef m_Participant;
 public:

 public:
  void setLocalPreview(bool flag) {m_LocalPreview = flag;}
  void setIncomingStream(bool flag) {m_IncomingStream = flag;}
  void setParticipant(ParticipantRef ref){m_Participant = ref;}
 private:

 public:
  VideoEx(unsigned int oid, SERootObject* root);
  ~VideoEx();


  void OnChange(int prop);
};
