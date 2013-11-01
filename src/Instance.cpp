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
  map<string,Ptr<Instance> > instance_;
  
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

	// Instance method
	string attribute(const string& name);

  // Instance method
  void attributeIs(const string& name, const string& v);

private:
  Ptr<ManagerImpl> manager_;
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

	// Instance method
	string attribute(const string& name);

  // Instance method
  void attributeIs(const string& name, const string& v);

private:
  Ptr<ManagerImpl> manager_;
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
	
	StatsRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) {
		stats_ = Stats::StatsNew(manager_->engine().ptr());
	}
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	Ptr<ManagerImpl> manager_;
	Stats::Ptr stats_;
};

class ConnRep : public Instance {
public:
	//typedef Fwk::Ptr<ConnRep const> PtrConst;
	//typedef Fwk::Ptr<ConnRep> Ptr;
	
	ConnRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) { 
	}
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	Ptr<ManagerImpl> manager_;
	
	void tokenizeLine(vector<string>& tokens, const string& str);
	void parseAttr(vector<string>& tokens, Mile& dist, Cost& cost, Time& time, string& exp);
};
class FleetRep : public Instance {
public:
	//typedef Fwk::Ptr<FleetRep const> PtrConst;
	//typedef Fwk::Ptr<FleetRep> Ptr;
	
	FleetRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) { 
	}
	
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	Ptr<ManagerImpl> manager_;
	
	string getTypeStr(const string& name);
	string getAttrStr(const string& name);
	
};


/**********************************************************************************
** Rep Layer Class Implementation 
**********************************************************************************/

/* Manager */

ManagerImpl::ManagerImpl() { 
	engine_ = Engine::EngineNew();
	statsRep_ = new StatsRep("StatsRep", this);
	fleetRep_ = new FleetRep("FleetRep", this);
	connRep_ = new ConnRep("ConnRep", this);
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
	
	if(instance_.find(name) != instance_.end() ||
		 name == statsName_ || 
		 name == fleetName_ ||
		 name == connName_ )
	{
			
		cerr << "Error: Already have instance named " << name << endl;
	}
	
	if (type == "Customer") {
  	Ptr<CustomerRep> t = new CustomerRep(name, this);
    instance_[name] = t;
    return t;
  }
	else if (type == "Port") {
  	Ptr<PortRep> t = new PortRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Truck terminal") {
  	Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Boat terminal") {
  	Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Plane terminal") {
  	Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Truck segment") {
  	Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Boat segment") {
  	Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Plane segment") {
  	Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Stats") {
  	if( "" == statsName_ ){
  		instance_[name] = statsRep_;
  		statsName_ = name;
  	}
  	else {
  		cerr << "Error: Only 1 allowed stats instance, returning stats instance named " 
  				 << statsName_ << endl;
  	}
    return statsRep_;
  }
  else if (type == "Conn") {
  	if( "" == connName_ ){
    	instance_[name] = connRep_;
    	connName_ = name;
 	 	}
 	 	else {
 	 		cerr << "Error: Only 1 allowed conn instance, returning conn instance named " 
  				 << connName_ << endl;
  	}
 		return connRep_;
 	}
  else if (type == "Fleet") {
    if( "" == fleetName_ ){
    	instance_[name] = fleetRep_;
    	fleetName_ = name;
 	 	}
 	 	else {
 	 		cerr << "Error: Only 1 allowed fleet instance, returning fleet instance named " 
  				 << fleetName_ << endl;
  	}
 		return fleetRep_;
  }
  else{
  	cerr << "Error: Invalid instance type " << type << endl;
  	return NULL;
  }
  
  return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
	if(name == statsName_) return statsRep_;
	else if(name == fleetName_) return fleetRep_;
	else if(name == connName_) return connRep_;
  map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);
  return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
	
	if(instance_.find(name) == instance_.end()){
		cerr << "Error: No instance named " << name << endl; 
  	return;
	}
	instance_.erase(name); //should call destructor if loc or segment?
	if(statsName_ == name) statsName_ = "";
	else if(connName_ == name) connName_ = "";
	else if(fleetName_ == name) fleetName_ = "";
	
}

/* LocationRep Impl */

string LocationRep::attribute(const string& name) {
	int index = segmentNumber(name);
	//index into vector starts at 0, client will start at 1
	Segment::Ptr seg = loc_->segmentAtIndex(index - 1);
	if((Segment::Ptr)NULL == seg){
		cerr << "Error: Invalid segment number " << index << endl;
		return "";
	}
	return seg->name();
}


void LocationRep::attributeIs(const string& name, const string& v) {
   //nothing to do, has no writable attributes
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
	if (name.substr(0, segmentStrlen) == segmentStr) {
      const char* t = name.c_str() + segmentStrlen;
      return atoi(t);
  }
  return 0;
}

/* SegmentRep Impl */

string SegmentRep::attribute(const string& name){
	string response;
	if("source" == name){
		Location::Ptr source = seg_->source();
		response = source->name();
	}
	else if("length" == name){
		Mile length = seg_->length();
		//response = length.toString();
		response = "PRINT MILES HERE";
	}
	else if("return segment" == name){
		Segment::Ptr returnSeg = seg_->returnSegment();
		response = returnSeg->name();
	}
	else if("difficulty" == name){
		Difficulty diff = seg_->difficulty();
		//response = diff.toString();
		response = "PRINT DIFFICULTY HERE";
	}
	else if("expedite support" == name){
		Segment::Expedite exp = seg_->expedite();
		response = (Segment::supported() == exp) ? "yes" : "no";
	}
	else{
		cerr << "Error: Invalid attribute => " << name << endl;
	}
	return response;
}


void SegmentRep::attributeIs(const string& name, const string& v){
	if("source" == name){
		Engine::Ptr eng = manager_->engine();
		Location::Ptr source = eng->location(v);
		seg_->sourceIs(source);
	}
	else if("length" == name){
		Mile length( atoi(v.c_str()) );
		seg_->lengthIs(length);
	}
	else if("return segment" == name){
		Engine::Ptr eng = manager_->engine();
		Segment::Ptr returnSeg = eng->segment(v);
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
			cerr << "Error: Invalid expedite value " << v << endl;
		}
	}
	else{
		cerr << "Error: Invalid attribute => " << name << endl;
	}

}

/* StatsRep Impl */

string StatsRep::attribute(const string& name){
	ostringstream s;
	
	if("Customer" == name) s << stats_->customers();
	else if("Port" == name) s << stats_->ports();
	else if("Truck terminal" == name) s << stats_->truckTerminals();
	else if("Boat terminal" == name) s << stats_->boatTerminals();
	else if("Plane terminal" == name) s << stats_->planeTerminals();
	else if("Truck segment" == name) s << stats_->truckSegments();
	else if("Boat segment" == name) s << stats_->boatSegments();
	else if("Plane segment" == name) s << stats_->planeSegments();
	else if("expedite percentage" == name) s << stats_->expeditePercentage();
	else {
		cerr << "Error: Invalid attribute => " << name << endl;
	}
	return s.str();
}

void StatsRep::attributeIs(const string& name, const string& v){
// Nothing to do, no writable attributes
}

/* FleetRep Impl */

string FleetRep::attribute(const string& name){
	string response;
	string type = getTypeStr(name);
	string attr = getAttrStr(name);
	Engine::Ptr eng = manager_->engine();
	/*
	if("Truck" == type && "cost" == attr) response = eng->truckFleet()->cost().toString();
	else if("Truck" == type && "capacity" == attr) response = eng->truckFleet()->capacity().toString();
	else if("Truck" == type && "speed" == attr) response = eng->truckFleet()->speed().toString();
	else if("Boat" == type && "cost" == attr) response = eng->boatFleet()->cost().toString();
	else if("Boat" == type && "capacity" == attr) response = eng->boatFleet()->capacity().toString();
	else if("Boat" == type && "speed" == attr) response = eng->boatFleet()->speed().toString();
	else if("Plane" == type && "cost" == attr) response = eng->planeFleet()->cost().toString();
	else if("Plane" == type && "capacity" == attr) response = eng->planeFleet()->capacity().toString();
	else if("Plane" == type && "speed" == attr) response = eng->planeFleet()->speed().toString();
	*/
	return response;
}

void FleetRep::attributeIs(const string& name, const string& v){
	string type = getTypeStr(name);
	string attr = getAttrStr(name);
	Engine::Ptr eng = manager_->engine();
	/*
	if("Truck" == type && "cost" == attr){
		eng->truckFleet()->costIs(???);
	}
	else if("Truck" == type && "capacity" == attr){
		eng->truckFleet()->capacityIs(???);
	}
	else if("Truck" == type && "speed" == attr){
		eng->truckFleet()->speedIs(???);
	}
	else if("Boat" == type && "cost" == attr){
		eng->boatFleet()->cost()
	}
	else if("Boat" == type && "capacity" == attr){
		eng->boatFleet()->capacityIs(???);
	}
	else if("Boat" == type && "speed" == attr){
		eng->boatFleet()->speedIs(???);
	}
	else if("Plane" == type && "cost" == attr){
		eng->planeFleet()->cost(???);
	}
	else if("Plane" == type && "capacity" == attr)
		eng->planeFleet()->capacityIs(???);
	}
	else if("Plane" == type && "speed" == attr){
		eng->planeFleet()->speedIs(???);
	}
	*/
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
	Engine::Ptr eng = manager_->engine();
	string response;
	string param;
	if(name.substr(0, exploreStrlen) == exploreStr){
		//cout << conn->attribute("explore customer1 : distance 1500") << endl
		param = name.substr(exploreStrlen + 1);
		int colon = param.find(':');
		string locStr = param.substr(0, colon - 1);
		string attr = param.substr(colon + 2);
		vector<string> tokens;
		tokenizeLine(tokens, attr);
		
		Mile maxDist; Cost maxCost; Time maxTime; string exp = "no";
		parseAttr(tokens, maxDist, maxCost, maxTime, exp);
		Location::Ptr loc = eng->location(locStr);
		Segment::Expedite expedited = (exp == "yes") ? Segment::supported() : Segment::unsupported();
		//vector<Path::Ptr> paths = eng->constrainedGraph(loc, maxDist, maxCost, maxTime, expedited);
		//stringify paths
	}
	else if(name.substr(0, connectStrlen) == connectStr){
		//cout << conn->attribute("connect customer2 : customer1") << endl;
		param = name.substr(exploreStrlen + 1);
		int colon = param.find(':');
		string startStr = param.substr(0, colon - 1);
		string endStr = param.substr(colon + 2);
		Location::Ptr start = eng->location(startStr);
		Location::Ptr end = eng->location(endStr);
		//vector<Path::Ptr> paths = eng->connections(start, end);
		//stringify paths
	}
	else{
	
	}
	return response;
}

//Parameters are passed by reference so they can stay in scope and get updated
void ConnRep::parseAttr(vector<string>& tokens, Mile& dist, Cost& cost, Time& time, string& exp){
	//check for even number of tokens - attr, value pairs
	if(0 != tokens.size() % 2) return;
	for(int i = 0; i < tokens.size(); i += 2){
		
	
	}
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
