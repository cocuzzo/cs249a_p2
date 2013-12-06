#include "Instance.h"
#include "Engine.h"
#include <cassert>

using namespace std;
using namespace Shipping;

/******************************************************************************
** Shipment Impl
******************************************************************************/
Shipment::Shipment(Location::Ptr _source, Location::Ptr _destination, Capacity _numPackages, Time _startTime) :
  numPackages_(_numPackages),
  source_(_source),
  destination_(_destination),
  startTime_(_startTime)
{}

/******************************************************************************
** Location Impl
******************************************************************************/
Location::Location(const string& _name, LocationType _locType) :
  locType_(_locType),
  name_(_name)
{}

void
Location::segmentAdd(Fwk::Ptr<Segment> _segment){
  for (Fwk::Ptr<Segment> s : segments_) {
    if (s->name() == _segment->name()) return;
  }
  segments_.push_back(_segment);
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

void
Location::shipmentIs(Shipment::Ptr _shipment) {
  if (notifiee_) try {
    notifiee_->onShipment(_shipment);
  } catch(...) { cerr << "notifiee exception caught during Location::shipmentIs" << endl; }
}

void
Location::shipmentRateIs(Capacity _shipmentRate) {
  shipmentRate_ = _shipmentRate;
  if (notifiee_) try {
    notifiee_->onShipmentAttr();
  } catch(...) { cerr << "notifiee exception caught during Location::shipmentRateIs" << endl; }
}

void
Location::shipmentSizeIs(Capacity _shipmentSize) {
  shipmentSize_ = _shipmentSize;
  if (notifiee_) try {
    notifiee_->onShipmentAttr();
  } catch(...) { cerr << "notifiee exception caught during Location::shipmentSizeIs" << endl; }
}

void
Location::shipmentDestinationIs(Location::Ptr _shipmentDestination) {
  if (_shipmentDestination) {
    shipmentDestination_ = _shipmentDestination;
    if (notifiee_) try {
      notifiee_->onShipmentAttr();
    } catch(...) { cerr << "notifiee exception caught during Location::shipmentDestinationIs" << endl; }
  }
}

bool
Location::readyToShip() {
  return ( shipmentRate_ != Capacity(0) &&
           shipmentSize_ != Capacity(0) &&
           shipmentDestination_ != NULL );
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
static const int DEFAULT_SEGMENT_CAPACITY = 10;

Segment::Segment(const string& _name, SegmentType _segType) :
  segType_(_segType),
  name_(_name),
  expedite_(Segment::unsupported()),
  capacity_(DEFAULT_SEGMENT_CAPACITY)
{}

void
Segment::sourceIs(Location::Ptr _source){
  if (source_ == _source) return;
  if (_source != NULL){
  	if (_source->locationType() == Location::boatTerminalLoc() && 
  		  segType_ != Segment::boatSeg()) return;
 		if (_source->locationType() == Location::truckTerminalLoc() && 
  		  segType_ != Segment::truckSeg()) return;
  	if (_source->locationType() == Location::planeTerminalLoc() && 
  		  segType_ != Segment::planeSeg()) return;	
  }		
  source_ = _source;
  if (notifiee_) try {
    notifiee_->onSegmentSource();
  } catch(...) { cerr << "notifiee exception caught during Segment::sourceIs" << endl; }
}

void
Segment::returnSegmentIs(Segment::Ptr _returnSegment){
	if (returnSegment_ == _returnSegment) return;
	if (_returnSegment != NULL){
		if (segType_ != _returnSegment->segmentType()) return;
	}
  returnSegment_ = _returnSegment;
  if (notifiee_) try {
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

void
Segment::shipmentIs(Shipment::Ptr _shipment) {
  if (notifiee_) try {
    notifiee_->onShipment(_shipment);
  } catch(...) { cerr << "notifiee exception caught during Segment::shipmentIs" << endl; }
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

void
LocationReactor::onLocationDel() {
  owner_->locationDel(notifier());
}

void
LocationReactor::onShipmentAttr() {
  if (injectReactor_) {
    // inject activity exists
    // now make sure the 3 attributes still require injecting
    if ( !notifier()->readyToShip() ){
     injectReactor_->notifier()->lastNotifieeIs(NULL);
     injectReactor_ = NULL;
    }
  } 
  else if ( notifier()->readyToShip() ) {
    // let Engine create & schedule new inject activity
    injectReactor_ = owner_->injectActivityNew(notifier());
  }
}


void
LocationReactor::onShipment(Shipment::Ptr _shipment) {
  // check if the current location is the desination of the shipment
  if (notifier_->name() == _shipment->destination()->name()) {
    // the shipment has been delivered, log some stats
    notifier()->shipmentsReceivedInc();
    Time timeTaken(owner_->manager()->now().value() - _shipment->startTime().value());
    notifier()->shipmentsTotalTimeInc(timeTaken);
    notifier()->shipmentsTotalCostInc(_shipment->cost());
  } 
  else {
    // determine the next segment to forward the shipment along
    Segment::Ptr nextSegment = owner_->routeShipment(_shipment, notifier());
    // now put the shipment onto the segment and let the segment/reactor
    // 	take care of creating and scheduling the forward activity
    nextSegment->shipmentIs(_shipment);
  } 
  
}

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

void
SegmentReactor::onSegmentDel() {
  owner_->segmentDel(notifier());
}

void SegmentReactor::onShipment(Shipment::Ptr _shipment) {
  // create new forwarding reactor for this shipment
  ForwardActivityReactor::Ptr reactor = owner_->forwardActivityNew(this, _shipment);
  
  Activity::Ptr activity = reactor->notifier();
  Activity::Manager::Ptr manager = owner_->manager();
  Segment::Ptr seg = notifier();
  
  seg->shipmentsReceivedInc();
  //cout << seg->name() << " : " << seg->shipmentsTraversing().toString() << endl;
  if(seg->shipmentsTraversing() < seg->capacity()){
  	//Segment accepted shipment
  	seg->shipmentsTraversingInc();
  	reactor->deliveredIs(true);
  	Fleet::Ptr fleet;
  	if(Segment::boatSeg() == seg->segmentType()) fleet = owner_->boatFleet();
  	else if(Segment::truckSeg() == seg->segmentType()) fleet = owner_->truckFleet();
  	else fleet = owner_->planeFleet();
  	Time traversalTime( seg->length().value() / fleet->speed().value() );
  	activity->nextTimeIs( manager->now().value() + traversalTime.value() );
  }
  else{
  	//Segment refuses shipment
  	seg->shipmentsRefusedInc();
  	activity->nextTimeIs( manager->now().value() + 1.0 );
  }
  
  activity->statusIs(Activity::nextTimeScheduled);

  forwardReactors_.push_back(reactor);
}

void SegmentReactor::forwardActivityDel(ForwardActivityReactor::Ptr far){
	far->notifier()->lastNotifieeIs(NULL);
	for(auto it = forwardReactors_.begin(); it != forwardReactors_.end(); ++it){
		if(*it == far){
			forwardReactors_.erase(it);
			break;
		}
	}
}

/******************************************************************************
** Engine Impl
******************************************************************************/
Engine::Engine() :
	boatFleet_(Fleet::FleetNew()),
	planeFleet_(Fleet::FleetNew()),
	truckFleet_(Fleet::FleetNew()),
  manager_(activityManagerInstance())
{}

Engine::~Engine(){

}


Location::Ptr
Engine::locationNew(const std::string& name, Location::LocationType locationType){
  // create new Location
  Location::Ptr loc = Location::LocationNew(name, locationType);
  if (!loc) {
    // cerr << "unable to create new location in Engine::locationNew" << endl;
    throw Exception ("unable to create new location in Engine::locationNew");
  }
  // create new LocationReactor using Location
  LocationReactor *reactor = LocationReactor::LocationReactorIs(loc.ptr(), this);
  if (!reactor) {
    // cerr << "unable to create new location reactor in Engine::locationNew" << endl;
    throw Exception ("unable to create new location reactor in Engine::locationNew");
  }
  // add reactor to internal name->reactor map
  auto result = locReactors_.insert(make_pair(name, reactor));
  if (result.second == false) {
    delete reactor;
    // cerr << "unable to add location reactor to map in Engine::locationNew" << endl;
    throw Exception ("unable to add location reactor to map in Engine::locationNew");
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
    // cerr << "unable to create new segment in Engine::segmentNew" << endl;
    throw Exception ("unable to create new segment in Engine::segmentNew");
  }
  // create new SegmentReaction using Segment
  SegmentReactor *reactor = SegmentReactor::SegmentReactorIs(seg.ptr(), this);
  if (!reactor) {
    // cerr << "unable to create new segment reactor in Engine::segmentNew" << endl;
    throw Exception ("unable to create new segment reactor in Engine::segmentNew");
  }
  // add reactor to internal name->reactor map
  auto result = segReactors_.insert(make_pair(name, reactor));
  if (result.second == false) {
    delete reactor;
    // cerr << "unable to add segment reactor to map in Engine::segmentNew" << endl;
    throw Exception ("unable to add segment reactor to map in Engine::segmentNew");
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
  // the list grows smaller each iteration b/c of reactor setup,
  // so must loop over index 0!!!
  U32 totalSegments = loc->segments();
  for (U32 i = 0; i < totalSegments; i++) {
    Segment::Ptr seg = loc->segmentAtIndex(0);
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
  //remove segment from returnSeg's return path!
  Segment::Ptr returnSeg = seg->returnSegment();
  if(returnSeg) returnSeg->returnSegmentIs(NULL);
  //remove the reactor from Engine's internal map
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

//----------| Trampoline Activity Creation Methods |------------//
InjectActivityReactor*
Engine::injectActivityNew(Location::Ptr _customer) {

  Activity::Ptr injectActivity = manager_->activityNew(string(_customer->name() + "_injectActivity"));
  InjectActivityReactor* reactor = new InjectActivityReactor(manager_, injectActivity.ptr(), _customer);
  if (!reactor) {
    throw Exception ("unable to create new inject reactor in Engine::injectActivityNew");
  } 
  injectActivity->lastNotifieeIs(reactor);
  injectActivity->nextTimeIs(manager_->now());
  injectActivity->statusIs(Activity::nextTimeScheduled);
  
  return reactor;
}

static unsigned long gFAid = 0;

ForwardActivityReactor*
Engine::forwardActivityNew(SegmentReactor::Ptr _segReactor, Shipment::Ptr _shipment) {
	ostringstream s;
	s << "ForwardActivity_" << gFAid++;
  Activity::Ptr forwardActivity = manager_->activityNew( s.str() );
  //cout << forwardActivity->name() << endl;
  ForwardActivityReactor* reactor = new ForwardActivityReactor(manager_, forwardActivity.ptr(), _shipment, _segReactor);
  if (!reactor) {
    throw Exception ("unable to create new forward reactor in Engine::forwardActivityNew");
  } 
  forwardActivity->lastNotifieeIs(reactor);
  	
  return reactor;
}

// shipment routing helper function that consults the spanning tree graph
Segment::Ptr
Engine::routeShipment(Shipment::Ptr shipment, Location::Ptr current)
{
  if (routingTable_.empty()) {
    // construct the routing table for the first time
    // assumes that the graph has at least 2 connected locations (not degenerate)
    Engine::generateRoutingTable(Engine::minCost());
    
    // //Print routing table
    // for (auto p : routingTable_) {
    // 	string key = p.first;
    // 	Segment::Ptr val = p.second;
    // 	cout << key << " : " << val->name() << endl;
    // }
    
  }
  
  Location::Ptr dest = shipment->destination();
  Segment::Ptr seg;
  string key = current->name() + "-" + dest->name();
  auto it = routingTable_.find(key);
  if (it != routingTable_.end()) {
    seg = it->second;
  } else {
    seg = Segment::Ptr(NULL);
  }
  return seg;
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

vector<ConstrainedPath::Ptr>
Engine::constrainedGraph(Location::Ptr _start, Segment::Expedite _expedite,
  Cost _maxCost, Mile _maxLength, Time _maxTime) {

  vector<ConstrainedPath::Ptr> results;
  list<ConstrainedPath::Ptr> frontier;

  ConstrainedPath::Ptr emptyPath = 
    ConstrainedPath::ConstrainedPathNew(this, _start, _expedite, _maxCost, _maxLength, _maxTime);

  for(U32 i = 0; i < _start->segments(); i++){
    Segment::Ptr seg = _start->segmentAtIndex(i);
    ConstrainedPath::Ptr cp = ConstrainedPath::ConstrainedPathNew(emptyPath); // deep copy
    if (cp->segmentAdd(seg)) frontier.push_back(cp);
  }

  for (ConstrainedPath::Ptr cp : frontier) {
  }

  while (!frontier.empty()) {
    ConstrainedPath::Ptr cp = frontier.front();
    frontier.pop_front();
    results.push_back(cp);  // add current path to the results set
    Location::Ptr lastNode = cp->path()->lastNode();

    // if the last node is a customer location, stop exploring along this path
    if ( lastNode->locationType() == Location::customerLoc() ) continue;

    for(U32 i = 0; i < lastNode->segments(); i++){
      Segment::Ptr seg = lastNode->segmentAtIndex(i);
      ConstrainedPath::Ptr cpc = ConstrainedPath::ConstrainedPathNew(cp);
      if (cpc->segmentAdd(seg)) frontier.push_back(cpc);
    }
  }

  /* FOR DEBUGGING */
  /*
  for (ConstrainedPath::Ptr cp : results) {
    cout << "CP: " << cp->path()->toString() 
         << " (expedite=" << cp->path()->expedite()
         << ", cost=" << cp->path()->cost().toString() 
         << ", length=" << cp->path()->length().toString() 
         << ", time=" << cp->path()->time().toString() 
         << ")" << endl;
  } */
  return results; 
}

  
vector<Path::Ptr> 
Engine::connections(Location::Ptr start, Location::Ptr end){
	
	vector<Path::Ptr> results;
	list<Path::Ptr> frontier;
	Path::Ptr emptySlow = Path::PathNew(this, start, Segment::unsupported() );
	Path::Ptr emptyFast = Path::PathNew(this, start, Segment::supported() );
	
	for(U32 i = 0; i < start->segments(); i++){
		Segment::Ptr seg = start->segmentAtIndex(i);
		Path::Ptr currSlow = Path::PathNew(emptySlow);
		Path::Ptr currFast = Path::PathNew(emptyFast);
		bool slowValid = currSlow->segmentAdd(seg);
		bool fastValid = currFast->segmentAdd(seg);
		if(slowValid){
		 frontier.push_back(currSlow);
		}
		if(fastValid) {
			frontier.push_back(currFast);
		}
	}//end for loop
	
	while( !frontier.empty() ) {
		Path::Ptr curr = frontier.front();
		frontier.pop_front();
		Location::Ptr last = curr->lastNode();
		if(NULL == last) continue;
		if(end == last){
			results.push_back(Path::PathNew(curr));
			continue;
		}
    if (last->locationType() == Location::customerLoc()) continue;

		for(U32 i = 0; i < last->segments(); i++){
			Path::Ptr currSpawn = Path::PathNew(curr);
			Segment::Ptr seg = last->segmentAtIndex(i);
			string debug = (seg != NULL) ? seg->name() : string("NULL");
			bool valid = currSpawn->segmentAdd(seg);
			if(valid){
			 frontier.push_back(currSpawn);
			}
		}// end for loop
	}// end while loop
	return results;
}

static const double kExpeditedRateCost = 1.5;
Cost
Engine::segmentCost(Segment::Ptr _seg, Segment::Expedite _expedite) {
  Cost cost;
  if (_seg->segmentType() == Segment::boatSeg()) {
    cost = boatFleet_->cost();
  } else if (_seg->segmentType() == Segment::planeSeg()) {
    cost = planeFleet_->cost();
  } else if (_seg->segmentType() == Segment::truckSeg()) {
    cost = truckFleet_->cost();
  } else {
    cerr << "bad segmentType in method Engine::segmentCost" << endl;
    return Cost();
  } 

  Cost segmentCost = Cost( cost.value() * _seg->difficulty().value() * _seg->length().value() );
  if (_expedite == Segment::supported()) {
    assert(_seg->expedite() == Segment::supported()); // MUST be true
    return Cost( segmentCost.value() * kExpeditedRateCost );
  } else {
    return segmentCost;
  }  
}

static const double kExpeditedRateTime = 1.3;
Time
Engine::segmentTime(Segment::Ptr _seg, Segment::Expedite _expedite) {
  Speed speed;
  if (_seg->segmentType() == Segment::boatSeg()) {
    speed = boatFleet_->speed();
  } else if (_seg->segmentType() == Segment::planeSeg()) {
    speed = planeFleet_->speed();
  } else if (_seg->segmentType() == Segment::truckSeg()) {
    speed = truckFleet_->speed();
  } else {
    cerr << "bad segmentType in method Engine::segmentTime" << endl;
    return Time();
  } 

  Time segmentTime = Time( _seg->length().value() / speed.value() );
  if (_expedite == Segment::supported()) {
    assert(_seg->expedite() == Segment::supported()); // MUST be true
    segmentTime = Time( segmentTime.value() / kExpeditedRateTime );
  }
  return segmentTime;
}

//----------| Protected Implementation |------------//
// construct the routing table for routing shipments
// called just before simulation time starts running
void
Engine::generateRoutingTable(Engine::RoutingHeuristic _heuristic) {
/*
for each location i
  for each location j
    compute paths from i to j
    if paths not empty
      choose shortest path based on heuristic
      extract first segment s
      add entry to routing table: (<i,j>,s)
*/
  for (auto pair_i  : locReactors_) {
    for (auto pair_j : locReactors_) {
      LocationReactor::Ptr reactor_i = pair_i.second;
      LocationReactor::Ptr reactor_j = pair_j.second;
      Location::Ptr i = reactor_i->notifier();
      Location::Ptr j = reactor_j->notifier();
      if (i->name() != j->name()) {
        vector<Path::Ptr> paths = connections(i, j);
        if (!paths.empty()) {
          Path::Ptr bestPath = paths.front();
          switch (_heuristic) {
            case minCost_: // find path with min cost
            {
              Cost minCost = MAX_COST;
              for (Path::Ptr p : paths) {
                if (p->cost() < minCost) {
                  bestPath = p;
                  minCost = p->cost();
                }
              }
              break;
            }
            case minLength_: // find path with min dist
            {
              Mile minLength = MAX_MILE;
              for (Path::Ptr p : paths) {
                if (p->length() < minLength) {
                  bestPath = p;
                  minLength = p->length();
                }
              }
              break;
            }
            case minTime_: // find path with min time
            {
              Time minTime = MAX_TIME;
              for (Path::Ptr p : paths) {
                if (p->time() < minTime) {
                  bestPath = p;
                  minTime = p->time();
                }
              }
              break;
            }
            default:
            {
              throw Exception("invalid RoutingHeuristic specified in generateRoutingTable");
              break;
            }
          }
          Segment::Ptr s = bestPath->segments().front();
          string key = i->name() + "-" + j->name();
          routingTable_.insert(make_pair(key, s));
        } 
      }
    }
  }
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
    if (seg->expedite() == Segment::supported()) expediteSegments_--;
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

/******************************************************************************
** Path Impl
******************************************************************************/
Path::Path(Engine::Ptr _engine, Location::Ptr _start, Segment::Expedite _expedite) :
  engine_(_engine),
  expedite_(_expedite)
{
  nodes_.push_back(_start);
}

Path::Path(Path::Ptr _path) :
  engine_(_path->engine()),
  cost_(_path->cost()),
  length_(_path->length()),
  time_(_path->time()),
  expedite_(_path->expedite())
{
  for (Segment::Ptr seg : _path->segments()) segments_.push_back(seg);
  for (Location::Ptr node : _path->nodes()) nodes_.push_back(node);
}

bool
Path::segmentAdd(Segment::Ptr _segment) {
  // only adds the segment if it has a valid return segment
  // which has a valid source node (valid end location)
  Segment::Ptr testReturn;
  Location::Ptr destNode;

  if ( !_segment ) return false; // arg is not a valid segment
  if ( !(testReturn = _segment->returnSegment()) ) return false; // no valid return segment
  if ( !(destNode = testReturn->source()) ) return false; // no valid destination location

  // is the destination location already in the nodes list?
  if ( containsNode(destNode) ) return false;

  // verify that the Segment supports expedited shipping if 
  // this Path is designated as expedited
  if ( (expedite_ == Segment::supported()) && 
       (_segment->expedite() == Segment::unsupported()) ) return false;

  // update Path stats
  cost_ = Cost(cost_.value() + engine_->segmentCost(_segment, expedite_).value());
  time_ = Time(time_.value() + engine_->segmentTime(_segment, expedite_).value());
  length_ = Mile(length_.value() + _segment->length().value());

  segments_.push_back(_segment);
  nodes_.push_back(destNode);
  return true;
}

string
Path::toString(){
	ostringstream ss;
	unsigned int numSegments = segments_.size();
	for(unsigned int i = 0; i < numSegments; i++){
		Segment::Ptr seg = segments_[i];
		ss << seg->source()->name() << "(" << seg->name() << ":"
			<< seg->length().toString() << ":"
			<< seg->returnSegment()->name() << ") ";
		if(i == numSegments - 1) ss << seg->returnSegment()->source()->name();
	}
	return ss.str();
}

bool
Path::containsNode(Location::Ptr _node) {
  for (Location::Ptr node : nodes_) {
    if (_node == node) return true;
  }
  return false;
}

/******************************************************************************
** ConstrainedPath Impl
******************************************************************************/
ConstrainedPath::ConstrainedPath(Engine::Ptr _engine, Location::Ptr _start,
  Segment::Expedite _expedite, Cost _costConstraint, 
  Mile _lengthConstraint, Time _timeConstraint) :
  costConstraint_(_costConstraint),
  lengthConstraint_(_lengthConstraint),
  timeConstraint_(_timeConstraint) 
{
  path_ = Path::PathNew(_engine, _start, _expedite);
}

ConstrainedPath::ConstrainedPath(ConstrainedPath::Ptr _cpath) :
  costConstraint_(_cpath->costConstraint()),
  lengthConstraint_(_cpath->lengthConstraint()),
  timeConstraint_(_cpath->timeConstraint()) 
{
  path_ = Path::PathNew(_cpath->path());
}

bool
ConstrainedPath::segmentAdd(Segment::Ptr _segment) {
  // first check that adding the new segment does not violate the constraints
  // compute new ConstrainedPath stats
  if ( (path_->expedite() == Segment::supported()) && (_segment->expedite() == Segment::unsupported()) ) return false;

  Cost newCost = Cost(path_->cost().value() + path_->engine()->segmentCost(_segment, path_->expedite()).value());
  Time newTime = Time(path_->time().value() + path_->engine()->segmentTime(_segment, path_->expedite()).value());
  Mile newLength = Mile(path_->length().value() + _segment->length().value());

  // check if any constraints are violated
  if ( (path_->expedite() == Segment::supported()) && (_segment->expedite() == Segment::unsupported()) ) return false;
  if ( (costConstraint_ < Cost::Max()) && (newCost > costConstraint_) ) return false;
  if ( (lengthConstraint_ < Mile::Max()) && (newLength > lengthConstraint_) ) return false;
  if ( (timeConstraint_ < Time::Max()) && (newTime > timeConstraint_) ) return false;

  // now let the Path class determine if this Segment can be added
  return path_->segmentAdd(_segment);
}

/******************************************************************************
** InjectActivityReactor Impl
******************************************************************************/

void InjectActivityReactor::onStatus() {
	switch (activity_->status()) {

		case Activity::executing:
			{
				//Do the injection...
				if( injectLoc_->readyToShip() ){
					Shipment::Ptr newShipment = Shipment::ShipmentNew(injectLoc_, 
																														injectLoc_->shipmentDestination(), 
																														injectLoc_->shipmentSize(),
																														manager_->now());
											
					injectLoc_->shipmentIs(newShipment);
					activity_->statusIs(Activity::free);
				}
				break;
			}
	
		case Activity::free:
			{
				//when done, automatically enqueue myself for next execution
				activity_->nextTimeIs(Time(activity_->nextTime().value() + 24.0/(injectLoc_->shipmentRate().value()) ));
				activity_->statusIs(Activity::nextTimeScheduled);
				break;
			}

		case Activity::nextTimeScheduled:
			{
				//add myself to be scheduled
				manager_->lastActivityIs(activity_);
				break;
			}

		default:
			{
				break;
			}
			
	}

}

void InjectActivityReactor::onNextTime() { }

/******************************************************************************
** ForwardActivityReactor Impl
******************************************************************************/

void ForwardActivityReactor::onStatus() {
	Segment::Ptr seg = segReactor_->notifier();
	Engine::Ptr eng = segReactor_->owner();
	Fleet::Ptr fleet;
	if(Segment::boatSeg() == seg->segmentType()) fleet = eng->boatFleet();
	else if(Segment::truckSeg() == seg->segmentType()) fleet = eng->truckFleet();
	else fleet = eng->planeFleet();
	
	switch (activity_->status()) {

		case Activity::executing:
		{
			//Do the forwarding...
			if(delivered_){
				//update total cost
				double size = shipment_->source()->shipmentSize().value();
				double fleetCap = fleet->capacity().value();
				double numVehicles = ceil(size / fleetCap);
				Cost segCost( numVehicles * seg->length().value() * seg->difficulty().value() * fleet->cost().value() );
				shipment_->costInc(segCost);
				//give to next location
				Location::Ptr nextLoc = seg->returnSegment()->source();
				nextLoc->shipmentIs(shipment_);
				//delete activity and reactor
				//cout << seg->name() << " : " << seg->shipmentsTraversing().toString() << endl;
				if(seg->shipmentsTraversing() > 0) seg->shipmentsTraversingDec(); //PEO
				activity_->statusIs(Activity::deleted);
				
			}
			else{
				activity_->statusIs(Activity::waiting);
			}
			
			break;
		}
		case Activity::deleted:
		{
			segReactor_->forwardActivityDel(this);
			break;
		}
		
		case Activity::waiting:
		{
			if(seg->shipmentsTraversing() < seg->capacity()){
				seg->shipmentsTraversingInc();
				delivered_ = true;
				Time traversalTime( seg->length().value() / fleet->speed().value() );
				activity_->nextTimeIs(manager_->now().value() + traversalTime.value());
			}
			else{
				activity_->nextTimeIs(manager_->now().value() + 1.0);
			}
			
			activity_->statusIs(Activity::nextTimeScheduled);
			break;
		}

		case Activity::nextTimeScheduled:
		{
			//add myself to be scheduled
			manager_->lastActivityIs(activity_);
			break;
		}

		default:
		{
			break;
		}
		
	}

}

void ForwardActivityReactor::onNextTime() { }
