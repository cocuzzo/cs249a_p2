#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Instance.h"
#include "Engine.h"
#include "Types.h"

namespace Shipping {

using namespace std;

/**********************************************************************************
** Rep Layer Class Definitions 
**********************************************************************************/

class LocationRep;
class SegmentRep;
class StatsRep;
class FleetRep;
class ConnRep;

class ManagerImpl : public Instance::Manager {
public:
	ManagerImpl();

	// Manager method
	Ptr<Instance> instanceNew(const string& name, const string& type);

	// Manager method
	Ptr<Instance> instance(const string& name);

  // Manager method
  void instanceDel(const string& name);
  
  Engine::Ptr engine(){ return engine_; }

private:  

  map<string,Ptr<LocationRep> > locReps_;
  map<string,Ptr<SegmentRep> > segReps_;
  
  Engine::Ptr engine_;
  
 	Ptr<StatsRep> statsRep_;
  string statsName_;
  Ptr<FleetRep> fleetRep_;
  string fleetName_;
  Ptr<ConnRep> connRep_;
  string connName_;
  
  
  
};

class LocationRep : public Instance {
public:
	//typedef Fwk::Ptr<LocationRep const> PtrConst;
	//typedef Fwk::Ptr<LocationRep> Ptr;

	LocationRep(const string& name, ManagerImpl* manager, Location::LocationType type) :
		Instance(name), manager_(manager)
	{
		Engine::Ptr eng = manager_->engine();
		loc_ = eng->locationNew(name, type);
	}
	
	~LocationRep(); //PEO
	
	// Instance method
	string attribute(const string& name);

  // Instance method
  void attributeIs(const string& name, const string& v);
  
  inline Location::Ptr location() const { return loc_; }
  void locationIs(Location::Ptr _loc){ loc_ = _loc; }

private:
  //Ptr<ManagerImpl> manager_;
  ManagerImpl* manager_;
	Location::Ptr loc_;
	
	int segmentNumber(const string& name);

};

class CustomerRep : public LocationRep {
public:
	//typedef Fwk::Ptr<CustomerRep const> PtrConst;
	//typedef Fwk::Ptr<CustomerRep> Ptr;
	
	CustomerRep(const string& name, ManagerImpl *manager):
		LocationRep(name, manager, Location::customerLoc() ){ }
};

class PortRep : public LocationRep {
public:
	//typedef Fwk::Ptr<PortRep const> PtrConst;
	//typedef Fwk::Ptr<PortRep> Ptr;
	
	PortRep(const string& name, ManagerImpl *manager) :
		LocationRep(name, manager, Location::portLoc() ){ }
};

                                                                                               
class TruckTerminalRep : public LocationRep {
public:
	//typedef Fwk::Ptr<TruckTerminalRep const> PtrConst;
	//typedef Fwk::Ptr<TruckTerminalRep> Ptr;
	
  TruckTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager, Location::truckTerminalLoc() )
  {
  	// Nothing else to do.
  }

};

class BoatTerminalRep : public LocationRep {
public:
	//typedef Fwk::Ptr<BoatTerminalRep const> PtrConst;
	//typedef Fwk::Ptr<BoatTerminalRep> Ptr;
	
  BoatTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager, Location::boatTerminalLoc() )
  {
  	// Nothing else to do.
  }

};

class PlaneTerminalRep : public LocationRep {
public:
	//typedef Fwk::Ptr<PlaneTerminalRep const> PtrConst;
	//typedef Fwk::Ptr<PlaneTerminalRep> Ptr;
	
  PlaneTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager, Location::planeTerminalLoc() )
  {
  	// Nothing else to do.
  }

};

class SegmentRep : public Instance {
public:
	//typedef Fwk::Ptr<SegmentRep const> PtrConst;
	//typedef Fwk::Ptr<SegmentRep> Ptr;
	
	SegmentRep(const string& name, ManagerImpl* manager, Segment::SegmentType type) :
		Instance(name), manager_(manager)
	{
		Engine::Ptr eng = manager_->engine();
		seg_ = eng->segmentNew(name, type);
	}
	
	~SegmentRep(); //PEO

	// Instance method
	string attribute(const string& name);

  // Instance method
  void attributeIs(const string& name, const string& v);
  
  inline Segment::Ptr segment() const { return seg_; }
  void segmentIs(Segment::Ptr _seg){ seg_ = _seg; }

private:
  //Ptr<ManagerImpl> manager_;
  ManagerImpl* manager_;
	Segment::Ptr seg_;

};

class TruckSegmentRep : public SegmentRep {
public:
	//typedef Fwk::Ptr<TruckSegmentRep const> PtrConst;
	//typedef Fwk::Ptr<TruckSegmentRep> Ptr;
	
  TruckSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager, Segment::truckSeg() )
  {
  	// Nothing else to do.
  }

};

class BoatSegmentRep : public SegmentRep {
public:
	//typedef Fwk::Ptr<BoatSegmentRep const> PtrConst;
	//typedef Fwk::Ptr<BoatSegmentRep> Ptr;
	
  BoatSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager, Segment::boatSeg() )
  {
  	// Nothing else to do.
  }

};

class PlaneSegmentRep : public SegmentRep {
public:
	//typedef Fwk::Ptr<PlaneSegmentRep const> PtrConst;
	//typedef Fwk::Ptr<PlaneSegmentRep> Ptr;
	
  PlaneSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager, Segment::planeSeg() )
  {
  	// Nothing else to do.
  }

};

class StatsRep : public Instance {
public:
	//typedef Fwk::Ptr<StatsRep const> PtrConst;
	//typedef Fwk::Ptr<StatsRep> Ptr;
	
	static Ptr<StatsRep> StatsRepNew(const string& name, ManagerImpl* manager) {
		Ptr<StatsRep> m = new StatsRep(name, manager);
    return m;
	}
	
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	StatsRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) {
		stats_ = Stats::StatsNew(manager_->engine().ptr());
	}
	
	//Ptr<ManagerImpl> manager_;
	ManagerImpl* manager_;
	Stats::Ptr stats_;
	
};

class ConnRep : public Instance {
public:
	//typedef Fwk::Ptr<ConnRep const> PtrConst;
	//typedef Fwk::Ptr<ConnRep> Ptr;
	
	static Ptr<ConnRep> ConnRepNew(const string& name, ManagerImpl* manager) {
		Ptr<ConnRep> m = new ConnRep(name, manager);
    return m;
	}
	
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	ConnRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) { 
	}
	
	//Ptr<ManagerImpl> manager_;
	ManagerImpl* manager_;
	
	void tokenizeLine(vector<string>& tokens, const string& str);
	void parseAttr(vector<string>& tokens, Mile& dist, Cost& cost, Time& time, string& exp);
	string stringifyConnect(vector<Path::Ptr>& paths);
	string stringifyExplore(vector<ConstrainedPath::Ptr>& paths);
};

class FleetRep : public Instance {
public:
	//typedef Fwk::Ptr<FleetRep const> PtrConst;
	//typedef Fwk::Ptr<FleetRep> Ptr;
	
	static Ptr<FleetRep> FleetRepNew(const string& name, ManagerImpl* manager) {
		Ptr<FleetRep> m = new FleetRep(name, manager);
    return m;
	}
	
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	FleetRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) { 
	}
	
	//Ptr<ManagerImpl> manager_;
	ManagerImpl* manager_;
	
	string getTypeStr(const string& name);
	string getAttrStr(const string& name);
	
};


/**********************************************************************************
** Rep Layer Class Implementation 
**********************************************************************************/

/* Manager */

ManagerImpl::ManagerImpl() { 
	engine_ = Engine::EngineNew();
	statsRep_ = StatsRep::StatsRepNew("StatsRep", this);
	fleetRep_ = FleetRep::FleetRepNew("FleetRep", this);
	connRep_ = ConnRep::ConnRepNew("ConnRep", this);
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
	Ptr<Instance> newInst;
	
	if(locReps_.find(name) != locReps_.end() ||
		 segReps_.find(name) != segReps_.end() ||
		 name == statsName_ || 
		 name == fleetName_ ||
		 name == connName_  ||
		 name == "")
	{
		throw Exception( string("Error: Already have instance named " + name) );
	}
	
	if (type == "Customer") {
		Ptr<CustomerRep> t = new CustomerRep(name, this);
		locReps_[name] = t;
		newInst = t;
	}
	else if (type == "Port") {
		Ptr<PortRep> t = new PortRep(name, this);
		locReps_[name] = t;
		newInst = t;
	}
	else if (type == "Truck terminal") {
		Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
		locReps_[name] = t;
		newInst = t;
	}
	else if (type == "Boat terminal") {
		Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
		locReps_[name] = t;
		newInst = t;
	}
	else if (type == "Plane terminal") {
		Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
		locReps_[name] = t;
		newInst = t;
	}
	else if (type == "Truck segment") {
		Ptr<TruckSegmentRep> t = new TruckSegmentRep(name, this);
		segReps_[name] = t;
		newInst = t;
	}
	else if (type == "Boat segment") {
		Ptr<BoatSegmentRep> t = new BoatSegmentRep(name, this);
		segReps_[name] = t;
		newInst = t;
	}
	else if (type == "Plane segment") {
		Ptr<PlaneSegmentRep> t = new PlaneSegmentRep(name, this);
		segReps_[name] = t;
		newInst = t;
	}
	else if (type == "Stats") {
		newInst = statsRep_;
		if( "" == statsName_ ){
			statsName_ = name;
		}
		else {
			throw Exception( string("Error: Only 1 allowed stats instance, returning stats instance named " + statsName_) );
		}
	}
	else if (type == "Conn") {
		newInst = connRep_;
		if( "" == connName_ ){
			connName_ = name;
		}
		else {
			throw Exception( string("Error: Only 1 allowed conn instance, returning conn instance named " + connName_) ); 
		}
	}
	else if (type == "Fleet") {
		newInst = fleetRep_;
		if( "" == fleetName_ ){
			fleetName_ = name;
		}
		else {
			throw Exception( string("Error: Only 1 allowed fleet instance, returning fleet instance named " + fleetName_) );
		}
	}
	else{
		throw Exception( string("Error: Invalid instance type " + type) );
	}
  
  return newInst;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
	if(name == statsName_) return statsRep_;
	else if(name == fleetName_) return fleetRep_;
	else if(name == connName_) return connRep_;
  map<string,Ptr<LocationRep> >::const_iterator lRep = locReps_.find(name);
  if(lRep != locReps_.end()) return (*lRep).second;
  map<string,Ptr<SegmentRep> >::const_iterator sRep = segReps_.find(name);
  if(sRep != segReps_.end()) return (*sRep).second;
  return NULL;
}

void ManagerImpl::instanceDel(const string& name) {
	
	map<string,Ptr<LocationRep> >::const_iterator lRep = locReps_.find(name);
	map<string,Ptr<SegmentRep> >::const_iterator sRep = segReps_.find(name);
	
	if(lRep != locReps_.end()){
		Ptr<LocationRep> lr = (*lRep).second;
		engine_->locationDel(lr->location());
		lr->locationIs(NULL);
		locReps_.erase(name); 
	}
	else if(sRep != segReps_.end()){
		Ptr<SegmentRep> sr = (*sRep).second;
		engine_->segmentDel(sr->segment());
		sr->segmentIs(NULL);
		segReps_.erase(name);
	}
	else if(statsName_ == name) statsName_ = "";
	else if(connName_ == name) connName_ = "";
	else if(fleetName_ == name) {
		fleetName_ = "";
		Cost dCost; Capacity dCapacity; Speed dSpeed;
		engine_->truckFleet()->costIs(dCost);
		engine_->truckFleet()->capacityIs(dCapacity);
		engine_->truckFleet()->speedIs(dSpeed);
		engine_->boatFleet()->costIs(dCost);
		engine_->boatFleet()->capacityIs(dCapacity);
		engine_->boatFleet()->speedIs(dSpeed);
		engine_->planeFleet()->costIs(dCost);
		engine_->planeFleet()->capacityIs(dCapacity);
		engine_->planeFleet()->speedIs(dSpeed);
	}
	
}

/* LocationRep Impl */

LocationRep::~LocationRep(){
	if(loc_){
		Engine::Ptr eng = manager_->engine();
		eng->locationDel(loc_);
	}
}

string LocationRep::attribute(const string& name) {
	string response;
	try{	
		if(loc_ == NULL) throw Exception( string("Error: Trying to access deleted instance") );
		
		int index = segmentNumber(name);
		if(index != -1){ 
			//index into vector starts at 0, client will start at 1
			Segment::Ptr seg = loc_->segmentAtIndex(index - 1);
			if(NULL == seg){
				throw Exception( string("Error: Invalid segment number ") );
			}
			response = seg->name();
		}
		else if("transfer rate" == name){
			if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
			Capacity rate = loc_->shipmentRate();
			response = rate.toString();
		}
		else if("shipment size" == name){
			if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
			Capacity size = loc_->shipmentSize();
			response = size.toString();
		}
		else if("destination" == name){
			if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
			Customer::Ptr dest = loc_->shipmentDestination();
			if(dest) response = dest->name();
		}
		else if("shipments received" == name){
			if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
			Capacity recvd = loc_->shipmentsReceived();
			response = recvd.toString();
		}
		else if("average latency" == name){
			if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
			Capacity avgLat = loc_->shipmentsAvgLatency();
			response = avgLat.toString();
		}
		else if("total cost" == name){
			if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
			Cost cost = loc_->shipmentsTotalCost();
			response = cost.toString();
		}
		else{
			throw Exception( string("Error: Invalid attribute => " + name) );
		}
		
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
	return response;
}


void LocationRep::attributeIs(const string& name, const string& v) {
   try{	
		if(loc_ == NULL) throw Exception( string("Error: Trying to access deleted instance") );
		if(Location::customerLoc() != loc_->locationType() ) throw Exception( string("Error: Location is not a Customer") );
		
		if("transfer rate" == name){
			Capacity rate( atoi(v.c_str()) );
			loc_->shipmentRateIs(rate);
		}
		else if("shipment size" == name){
			Capacity size( atoi(v.c_str()) );
			loc_->shipmentSizeIs(size);
		}
		else if("destination" == name){
			Engine::Ptr eng = manager_->engine();
			Location::Ptr dest = eng->location(v);
			if(NULL == dest) throw Exception( string("Error: Invalid dest name => " + v) );
			if(Location::customerLoc() != dest->locationType()) throw Exception( string("Error: Destination " + v + " is not a Customer") );
			loc_->shipmentDestinationIs(dest);
		}
		else{
			throw Exception( string("Error: Invalid attribute => " + name) );
		}
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
	if (name.substr(0, segmentStrlen) == segmentStr) {
      const char* t = name.c_str() + segmentStrlen;
      return atoi(t);
  }
  return -1;
}

/* SegmentRep Impl */

SegmentRep::~SegmentRep(){
	if(seg_){
		Engine::Ptr eng = manager_->engine();
		eng->segmentDel(seg_);
	}
}

string SegmentRep::attribute(const string& name){
	string response;
	try{
		if(seg_ == NULL) throw Exception( string("Error: Trying to access deleted instance") );
		
		if("source" == name){
			Location::Ptr source = seg_->source();
			if(source) response = source->name();
		}
		else if("length" == name){
			Mile length = seg_->length();
			response = length.toString();
		}
		else if("return segment" == name){
			Segment::Ptr returnSeg = seg_->returnSegment();
			if(returnSeg) response = returnSeg->name();
		}
		else if("difficulty" == name){
			Difficulty diff = seg_->difficulty();
			response = diff.toString();
		}
		else if("expedite support" == name){
			Segment::Expedite exp = seg_->expedite();
			response = (Segment::supported() == exp) ? "yes" : "no";
		}
		else{
			throw Exception( string("Error: Invalid attribute => " + name) );
		}
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
	
	return response;
}


void SegmentRep::attributeIs(const string& name, const string& v){
	try{	
		if(seg_ == NULL) throw Exception( string("Error: Trying to access deleted instance") );
		
		if("source" == name){
			if("" == v){
				seg_->sourceIs(NULL);
				return;
			}
			Engine::Ptr eng = manager_->engine();
			Location::Ptr source = eng->location(v);
			if(source == NULL) throw Exception( string("Error: Invalid source name => " + v) );
			seg_->sourceIs(source);
		}
		else if("length" == name){
			Mile length( atof(v.c_str()) );
			seg_->lengthIs(length);
		}
		else if("return segment" == name){
			if("" == v){
				seg_->returnSegmentIs(NULL);
				return;
			}
			Engine::Ptr eng = manager_->engine();
			Segment::Ptr returnSeg = eng->segment(v);
			if(returnSeg == NULL)	throw Exception( string("Error: Invalid return segment name => " + v) );
			seg_->returnSegmentIs(returnSeg);
		}
		else if("difficulty" == name){
			Difficulty diff( atof(v.c_str()) );
			seg_->difficultyIs(diff);
		}
		else if("expedite support" == name){
			if("yes" == v){
				seg_->expediteIs(Segment::supported());
			}
			else if("no" == v){
				seg_->expediteIs(Segment::unsupported());
			}
			else{
				throw Exception( string("Error: Invalid expedite value " + v) );
			}
		}
		else{
			throw Exception( string("Error: Invalid attribute => " + name) );
		}
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block

}

/* StatsRep Impl */

string StatsRep::attribute(const string& name){
	ostringstream s;
	try{
		if("Customer" == name) s << stats_->customers();
		else if("Port" == name) s << stats_->ports();
		else if("Truck terminal" == name) s << stats_->truckTerminals();
		else if("Boat terminal" == name) s << stats_->boatTerminals();
		else if("Plane terminal" == name) s << stats_->planeTerminals();
		else if("Truck segment" == name) s << stats_->truckSegments();
		else if("Boat segment" == name) s << stats_->boatSegments();
		else if("Plane segment" == name) s << stats_->planeSegments();
		else if("expedite percentage" == name){ 
			char buf[MAX_BUF];
			std::sprintf(buf, "%.2f", stats_->expeditePercentage());
			s << std::string(buf);
		}
		else {
			throw Exception( string("Error: Invalid attribute => " + name) );
		}
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
	return s.str();
}

void StatsRep::attributeIs(const string& name, const string& v){
// Nothing to do, no writable attributes
}

/* FleetRep Impl */

string FleetRep::attribute(const string& name){
	string response;
	try{
		string type = getTypeStr(name);
		string attr = getAttrStr(name);
		Engine::Ptr eng = manager_->engine();
		
		if("Truck" == type && "cost" == attr) response = eng->truckFleet()->cost().toString();
		else if("Truck" == type && "capacity" == attr) response = eng->truckFleet()->capacity().toString();
		else if("Truck" == type && "speed" == attr) response = eng->truckFleet()->speed().toString();
		else if("Boat" == type && "cost" == attr) response = eng->boatFleet()->cost().toString();
		else if("Boat" == type && "capacity" == attr) response = eng->boatFleet()->capacity().toString();
		else if("Boat" == type && "speed" == attr) response = eng->boatFleet()->speed().toString();
		else if("Plane" == type && "cost" == attr) response = eng->planeFleet()->cost().toString();
		else if("Plane" == type && "capacity" == attr) response = eng->planeFleet()->capacity().toString();
		else if("Plane" == type && "speed" == attr) response = eng->planeFleet()->speed().toString();
		
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
	return response;
}

void FleetRep::attributeIs(const string& name, const string& v){
	try {
		string type = getTypeStr(name);
		string attr = getAttrStr(name);
		Engine::Ptr eng = manager_->engine();
		Cost cost; Capacity cap; Speed speed;
		
		if("Truck" == type && "cost" == attr){
			cost = Cost(atof( v.c_str()) );
			eng->truckFleet()->costIs(cost);
		}
		else if("Truck" == type && "capacity" == attr){
			cap = Capacity(atoi( v.c_str()) );
			eng->truckFleet()->capacityIs(cap);
		}
		else if("Truck" == type && "speed" == attr){
			speed = Speed(atof( v.c_str()) );
			eng->truckFleet()->speedIs(speed);
		}
		else if("Boat" == type && "cost" == attr){
			cost = Cost(atof( v.c_str()) );
			eng->boatFleet()->costIs(cost);
		}
		else if("Boat" == type && "capacity" == attr){
			cap = Capacity(atoi( v.c_str()) );
			eng->boatFleet()->capacityIs(cap);
		}
		else if("Boat" == type && "speed" == attr){
			speed = Speed(atof( v.c_str()) );
			eng->boatFleet()->speedIs(speed);
		}
		else if("Plane" == type && "cost" == attr){
			cost = Cost(atof( v.c_str()) );
			eng->planeFleet()->costIs(cost);
		}
		else if("Plane" == type && "capacity" == attr){
			cap = Capacity(atoi( v.c_str()) );
			eng->planeFleet()->capacityIs(cap);
		}
		else if("Plane" == type && "speed" == attr){
			speed = Speed(atof( v.c_str()) );
			eng->planeFleet()->speedIs(speed);
		}
		
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
	
}

string FleetRep::getTypeStr(const string& name){
	size_t comma = name.find(',');
	return name.substr(0, comma);
}
string FleetRep::getAttrStr(const string& name){
	size_t comma = name.find(',');
	//offset is 2 because comma and space before the attr
	return name.substr(comma + 2); 
}

/* ConnRep Impl */
static const string exploreStr = "explore";
static const int exploreStrlen = exploreStr.length();
static const string connectStr = "connect";
static const int connectStrlen = connectStr.length();

static inline bool delimFn(const char& c){ return 0 != isspace(c); }

void ConnRep::tokenizeLine(vector<string>& tokens, const string& str){
  auto end = str.end();
  auto start = str.begin();
  while(start != end){
    start = find_if_not(start,end, delimFn);
    if(start == end) break;
    auto foundNext = find_if(start,end, delimFn);
    tokens.push_back(string(start,foundNext));
    start = foundNext;
  }
  return;
}

string ConnRep::attribute(const string& name){
	
	string response;
	try{
		Engine::Ptr eng = manager_->engine();
		string param;
		if(name.substr(0, exploreStrlen) == exploreStr){
			param = name.substr(exploreStrlen + 1);
			int colon = param.find(':');
			string locStr = param.substr(0, colon - 1);
			string attr = param.substr(colon + 2);
			vector<string> tokens;
			tokenizeLine(tokens, attr);
			
			Location::Ptr loc = eng->location(locStr);
			//set default max values so parseAttr can update them
			Mile maxDist = Mile::Max(); Cost maxCost = Cost::Max(); Time maxTime = Time::Max(); string exp = "no";
			parseAttr(tokens, maxDist, maxCost, maxTime, exp);
			Segment::Expedite expedited = (exp == "yes") ? Segment::supported() : Segment::unsupported();
			vector<ConstrainedPath::Ptr> conPaths = eng->constrainedGraph(loc, expedited, maxCost, maxDist, maxTime);
			response = stringifyExplore(conPaths);
		}
		else if(name.substr(0, connectStrlen) == connectStr){
			param = name.substr(connectStrlen + 1);
			int colon = param.find(':');
			string startStr = param.substr(0, colon - 1);
			string endStr = param.substr(colon + 2);
			Location::Ptr start = eng->location(startStr);
			Location::Ptr end = eng->location(endStr);
			vector<Path::Ptr> paths = eng->connections(start, end);
			response = stringifyConnect(paths);
		}
		else{
			throw Exception( string("Error: Invalid attribute => " + name) );
		}
	}//end try block
	catch(Exception& e){
		cerr << e.what() << endl;
	}//end catch block
	
	return response;
}

//Parameters are passed by reference so they can stay in scope and get updated
void ConnRep::parseAttr(vector<string>& tokens, Mile& dist, Cost& cost, Time& time, string& exp){
	//check for even number of tokens - attr, value pairs
	int i = 0;
	int numTokens = tokens.size();
	while( i < numTokens ){
		if("distance" == tokens[i]){
			++i;
			if(i >= numTokens) throw Exception( string("Error: No value field") );
			dist = Mile( atoi(tokens[i].c_str()) );
			++i;
		}
		else if("cost" == tokens[i]){
			++i;
			if(i >= numTokens) throw Exception( string("Error: No value field") );
			cost = Cost(atof( tokens[i].c_str()) );
			++i;
		}
		else if("time" == tokens[i]){
			++i;
			if(i >= numTokens) throw Exception( string("Error: No value field") );
			time = Time( atoi( tokens[i].c_str()) );
			++i;
		}
		else if("expedited" == tokens[i]){
			exp = "yes";
			++i;
		}
		else{
			throw Exception( string("Error: Invalid attribute => " + tokens[i]) );
		} 
	}//end while
}

string ConnRep::stringifyConnect(vector<Path::Ptr>& paths){
	ostringstream ss;
	for(unsigned int i = 0; i < paths.size(); i++){
		Path::Ptr p = paths[i];
		Cost c = p->cost();
		Time t = p->time();
		Segment::Expedite exp = p->expedite();
		string expStr = (Segment::supported() == exp) ? "yes" : "no";
		ss << c.toString() << " " << t.toString() << " "
		<< expStr << "; " << p->toString() << "\n";
	}
	return ss.str();
}

string ConnRep::stringifyExplore(vector<ConstrainedPath::Ptr>& paths){
	ostringstream ss;
	for(unsigned int i = 0; i < paths.size(); i++){
		ConstrainedPath::Ptr p = paths[i];
		ss << p->path()->toString() << "\n";
	}
	return ss.str();
}

void ConnRep::attributeIs(const string& name, const string& v){
// Nothing to do here, no writeable attributes
}


} //end shipping namespace

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
  return new Shipping::ManagerImpl();
}

