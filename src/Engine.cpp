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

Location::~Location() {
  // TODO: need to clear notifiees strong ptr?
}

void
Location::segmentAdd(Fwk::Ptr<Segment> _segment){
  for (Fwk::Ptr<Segment> s : segments_) {
    if (s->name() == _segment->name()) return;
  }
  segments_.push_back(_segment);
  // TODO: no need to notify notifiee here?
}

void
Location::segmentRemove(Fwk::Ptr<Segment> _segment){
  segments_.remove_if([&_segment](Fwk::Ptr<Segment> seg){ return seg->name() == _segment->name(); });
}

Fwk::Ptr<Segment>
Location::segmentAtIndex(unsigned int _index){
  Fwk::Ptr<Segment> seg;
  if (segments_.empty()) return NULL;
  try {
    auto it = segments_.begin();
    auto end = segments_.end();
    for (unsigned int i = 0; i < _index; i++) {
      it++;
      if (it == end) return NULL;
    }
    seg = *it;
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

Segment::~Segment() {
  // need to clear notifiees strong ptr?
}

void
Segment::sourceIs(Location::Ptr _source){
  if (source_ == _source) return;
  source_ = _source;
  if(notifiee_) try {
    notifiee_->onSegmentSource();
  } catch(...) { cerr << "notifiee exception caught during Segment::sourceIs" << endl; }
}

void
Segment::returnSegmentIs(Segment::Ptr _returnSegment){
	if (returnSegment_ == _returnSegment) return;
  returnSegment_ = _returnSegment;
  if(notifiee_) try {
    notifiee_->onSegmentReturn();
  } catch(...) { cerr << "notifiee exception caught during Segment::returnSegmentIs" << endl; }
}

void
Segment::expediteIs(Segment::Expedite _expedite) {
  if (expedite_ == _expedite) return;
  expedite_ = _expedite;
  if(notifiee_) try {
    notifiee_->onSegmentExpedite();
  } catch(...) { cerr << "notifiee exception caught during Segment::returnSegmentIs" << endl; }
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
** Fleet Impl
******************************************************************************/

/******************************************************************************
** LocationReactor Impl
******************************************************************************/

/******************************************************************************
** SegmentReactor Impl
******************************************************************************/
void
SegmentReactor::onSegmentSource() {
  Location::Ptr prevSource = prevSource_;
  prevSource_ = notifier()->source();
  owner_->handleSegmentSource(notifier(), prevSource);
}

void
SegmentReactor::onSegmentReturn() {
  Segment::Ptr prevReturn = prevReturn_;
  prevReturn_ = notifier()->returnSegment();
  owner_->handleSegmentReturn(notifier(), prevReturn);
}

void
SegmentReactor::onSegmentExpedite() {
  owner_->handleSegmentExpedite(notifier());
}

/******************************************************************************
** Engine Impl
******************************************************************************/
Engine::Engine(){
	boatFleet_ = Fleet::FleetNew();
	planeFleet_ = Fleet::FleetNew();
	truckFleet_ = Fleet::FleetNew();
}

Engine::~Engine(){

}

Location::Ptr
Engine::locationNew(const std::string& name, Location::LocationType locationType){
  // create new Location
  Location::Ptr loc = Location::LocationNew(name, locationType);
  if (!loc) {
    cerr << "unable to create new location in Engine::locationNew" << endl;
    return NULL;
  }
  // create new LocationReactor using Location
  LocationReactor *reactor = LocationReactor::LocationReactorIs(loc.ptr(), this);
  if (!reactor) {
    cerr << "unable to create new location reactor in Engine::locationNew" << endl;
    return NULL;
  }
  // add reactor to internal name->reactor map
  auto result = locReactors_.insert(make_pair(name, reactor));
  if (result.second == false) {
    free(reactor);
    cerr << "unable to add location reactor to map in Engine::locationNew" << endl;
  } else {
    // notify interested party
    if(notifiee_) try {
      notifiee_->onLocationNew(loc);
    } catch(...) { 
      cerr << "notifiee exception caught during Location::locationNew" << endl; 
    }
  }
	return loc;
}

Segment::Ptr
Engine::segmentNew(const std::string& name, Segment::SegmentType segmentType){
  // create new Segment
  Segment::Ptr seg = Segment::SegmentNew(name, segmentType);
  if (!seg) {
    cerr << "unable to create new segment in Engine::segmentNew" << endl;
    return NULL;
  }
  // create new SegmentReaction using Segment
  SegmentReactor *reactor = SegmentReactor::SegmentReactorIs(seg.ptr(), this);
  if (!reactor) {
    cerr << "unable to create new segment reactor in Engine::segmentNew" << endl;
    return NULL;
  }
  // add reactor to internal name->reactor map
  auto result = segReactors_.insert(make_pair(name, reactor));
  if (result.second == false) {
    delete reactor;
    cerr << "unable to add segment reactor to map in Engine::segmentNew" << endl;
  } else {
    // notify interested party
    if(notifiee_) try {
      notifiee_->onSegmentNew(seg);
    } catch(...) { 
      cerr << "notifiee exception caught during Segment::segmentNew" << endl; 
    }
  }
  return seg;
}

void
Engine::locationDel(Location::Ptr loc) {
  // iterate through segments in a location, clear the source
  for (U32 i = 0; i < loc->segments(); i++) {
    Segment::Ptr seg = loc->segmentAtIndex(i);
    seg->sourceIs(NULL);
  }
  // remove the reactor from Engine's internal map
  if (locReactors_.erase(loc->name()) != 1) return;
  // send out notification
  if(notifiee_) {
    notifiee_->onLocationDel(loc);
  }
}

void
Engine::segmentDel(Segment::Ptr seg) {
  // remove segment from segment source's segment list
  Location::Ptr loc = seg->source();
  if (loc) loc->segmentRemove(seg);
  // remove the reactor from Engine's internal map
  if (segReactors_.erase(seg->name()) != 1) return;
  // send out notification
  if (notifiee_) {
    notifiee_->onSegmentDel(seg);
  }
}

Location::Ptr 
Engine::location(const std::string& name){
	//look up in map of location reactors and return its notifier location
	map<string, LocationReactor::Ptr>::iterator result = locReactors_.find(name);
	return (result == locReactors_.end()) ? NULL : (*result).second->notifier();
}

Segment::Ptr 
Engine::segment(const std::string& name){
  //look up in map of segment reactors and return its notifier segment
  map<string, SegmentReactor::Ptr>::iterator result = segReactors_.find(name);
  return (result == segReactors_.end()) ? NULL : (*result).second->notifier();
}

//----------| Trampoline Notification Methods |------------//
void
Engine::handleSegmentSource( Segment::Ptr seg, Location::Ptr prevSource ) {
  // remove segment from prev source's segment list
  if (prevSource) prevSource->segmentRemove(seg);
  // add the segment to the source's segment list
  Location::Ptr source = seg->source();
  if (source) source->segmentAdd(seg);
}

void
Engine::handleSegmentReturn( Segment::Ptr seg, Segment::Ptr prevReturn ) {
  // remove returnSegment from prev return segment's state
  if (prevReturn) prevReturn->returnSegmentIs(NULL);
  // add the segment to the return segments returnSegment field
  Segment::Ptr retSeg = seg->returnSegment();
  if (retSeg) retSeg->returnSegmentIs(seg); 
}

void
Engine::handleSegmentExpedite( Segment::Ptr seg ) {
  // send notification to notifiee
  if (notifiee_) {
    notifiee_->onSegmentExpedite(seg);
  }
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

//----------| Graph Query Implementation |------------//
/*
For both connect and explore queries, the paths should not visit a location more than once. 
Also, if there is more than one path to the same location, then all the paths should be returned, 
with each path on its own line.

Here is a sample explore output with two possible paths from loc0 to loc3:

loc0(seg0:len0:seg0')^loc1(seg1:len1:seg1')^loc2(seg2:len2:seg2')^loc3$
loc0(seg3:len3:seg3')^loc3$
and here is a sample connect output with two possible paths from loc0 to loc3:

cost^time^expedited;^loc0(seg0:len0:seg0')^loc1(seg1:len1:seg1')^loc2(seg2:len2:seg2')^loc3$
cost^time^expedited;^loc0(seg3:len3:seg3')^loc3$
In the examples above, ^ is a single space and $ is a single newline (don't actually print ^ or $!). 
There should be no other leading or trailing whitespace. 
Note that, as a floating-point value, the lengths should be printed to 2 decimals.
*/

vector<Path::Ptr>
Engine::constrainedGraph(Location::Ptr loc, Mile distance, 
                       Cost cost, Time time, Segment::Expedite expedited) {
  vector<Path::Ptr> path;
  return path; 
}

vector<Path::Ptr>
Engine::connections(Location::Ptr start, Location::Ptr end){
  
  vector<Path::Ptr> path;
  return path; 
}

/******************************************************************************
** Stats Impl
******************************************************************************/
Stats::~Stats() {}

void
Stats::onLocationNew( Location::Ptr loc ){
  if (loc) {
    if (loc->locationType() == Location::customerLoc()) customers_++;
    else if (loc->locationType() == Location::portLoc()) ports_++;
    else if (loc->locationType() == Location::boatTerminalLoc()) boatTerminals_++;
    else if (loc->locationType() == Location::planeTerminalLoc()) planeTerminals_++;
    else if (loc->locationType() == Location::truckTerminalLoc()) truckTerminals_++;
    else cerr << "undefined location type in Stats::onLocationNew" << endl;
  }
}

void
Stats::onLocationDel( Location::Ptr loc ){
  if (loc) {
    if (loc->locationType() == Location::customerLoc()) customers_--;
    else if (loc->locationType() == Location::portLoc()) ports_--;
    else if (loc->locationType() == Location::boatTerminalLoc()) boatTerminals_--;
    else if (loc->locationType() == Location::planeTerminalLoc()) planeTerminals_--;
    else if (loc->locationType() == Location::truckTerminalLoc()) truckTerminals_--;
    else cerr << "undefined location type in Stats::onLocationDel" << endl;
  }
}

void
Stats::onSegmentNew( Segment::Ptr seg ){
  if (seg) {
    if (seg->segmentType() == Segment::boatSeg()) boatSegments_++;
    else if (seg->segmentType() == Segment::planeSeg()) planeSegments_++;
    else if (seg->segmentType() == Segment::truckSeg()) truckSegments_++;
    else cerr << "undefined segment type in Stats::onSegmentNew" << endl;
  }
}

void
Stats::onSegmentDel( Segment::Ptr seg ){
  if (seg) {
    if (seg->segmentType() == Segment::boatSeg()) boatSegments_--;
    else if (seg->segmentType() == Segment::planeSeg()) planeSegments_--;
    else if (seg->segmentType() == Segment::truckSeg()) truckSegments_--;
    else cerr << "undefined segment type in Stats::onSegmentDel" << endl;
  }
}

void
Stats::onSegmentExpedite( Segment::Ptr seg ){
  if (seg) {
    (seg->expedite() == Segment::supported()) ? expediteSegments_++ : expediteSegments_--;
  }
}

double Stats::expeditePercentage() const {
	double perc = ((double)expediteSegments_)/(boatSegments_ + planeSegments_ + truckSegments_) * 100; 
	return perc;
} 





