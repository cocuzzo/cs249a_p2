#include "Instance.h"
#include "Engine.h"

using namespace std;
using namespace Shipping;

/******************************************************************************
** Location Impl
******************************************************************************/
Location::Location(const string& _name, LocationType _locType) {
  locType_ = _locType;
  name_ = _name;
}

void
Location::segmentAdd(Fwk::Ptr<Segment> _segment){
  // need to refactor for idempotency
  segments_.push_back(_segment);
}

void
Location::segmentRemove(Fwk::Ptr<Segment> _segment){

}

Fwk::Ptr<Segment>
Location::segmentAtIndex(unsigned int _index){
  Fwk::Ptr<Segment> seg;
  try {
    seg = segments_.at(_index);
  } catch (...) {
    return NULL;
  }
	return seg;
}

//----------| NotifieeConst Implementation |------------//

Location::NotifieeConst::~NotifieeConst() {
  if(notifier_) {
    notifier_->deleteNotifiee();
  }
}

void
Location::NotifieeConst::notifierIs(const Location::PtrConst& _notifier) {
  if(_notifier == notifier_) return;
  notifier_ = _notifier;
  if(_notifier) {
    _notifier->newNotifiee(this);
  }
}

//----------| Notifiee Implementation |------------//

/******************************************************************************
** Segment Impl
******************************************************************************/
Segment::Segment(const string& _name, SegmentType _segType){
  segType_ = _segType;
  name_ = _name;
}

void
Segment::sourceIs(Location::Ptr _loc){

}

void
Segment::returnSegmentIs(Segment::Ptr _returnSegment){


}

//----------| NotifieeConst Implementation |------------//

Segment::NotifieeConst::~NotifieeConst() {
  if(notifier_) {
    notifier_->deleteNotifiee();
  }
}

void
Segment::NotifieeConst::notifierIs(const Segment::PtrConst& _notifier) {
  if(_notifier == notifier_) return;
  notifier_ = _notifier;
  if(_notifier) {
    _notifier->newNotifiee(this);
  }
}

//----------| Notifiee Implementation |------------//

/******************************************************************************
** Conn Impl
******************************************************************************/
vector<Path::Ptr>
Conn::ConstrainedGraph(Location::Ptr loc, Mile distance, 
														Cost cost, Time time, Segment::Expedite expedited) {
	vector<Path::Ptr> path;
	return path; 
}
vector<Path::Ptr>
Conn::Connections(Location::Ptr start, Location::Ptr end){
	
	vector<Path::Ptr> path;
	return path; 
}

/******************************************************************************
** LocationReactor Impl
******************************************************************************/
void
LocationReactor::onLocationNew(){


}
void
LocationReactor::onLocationDel(){



}


/******************************************************************************
** SegmentReactor Impl
******************************************************************************/
void
SegmentReactor::onSegmentNew(){

}
void
SegmentReactor::onSegmentDel(){

}
void
SegmentReactor::onSegmentSource(){

}
void
SegmentReactor::onSegmentReturn(){

}
void
SegmentReactor::onSegmentExpedite(){

}

/******************************************************************************
** Engine Impl
******************************************************************************/
Engine::~Engine(){

}

Location::Ptr
Engine::locationNew(std::string name, Location::LocationType _locationType){

	return NULL; //placeholder
}
Segment::Ptr
Engine::segmentNew(std::string name, Segment::SegmentType _segmentType){

	return NULL; //placeholder
}

//----------| NotifieeConst Implementation |------------//

Engine::NotifieeConst::~NotifieeConst() {
  if(notifier_) {
    notifier_->deleteNotifiee();
  }
}

void
Engine::NotifieeConst::notifierIs(const Engine::PtrConst& _notifier) {
  if(_notifier == notifier_) return;
  notifier_ = _notifier;
  if(_notifier) {
    _notifier->newNotifiee(this);
  }
}



/******************************************************************************
** Stats Impl
******************************************************************************/
void
Stats::onLocationNew( Location::Ptr loc ){

}
void
Stats::onSegmentNew( Segment::Ptr seg ){

}
void
Stats::onSegmentExpedite( Segment::Ptr seg ){

}









