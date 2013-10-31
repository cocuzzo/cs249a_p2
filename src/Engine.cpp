#include "Instance.h"
#include "Engine.h"

using namespace std;
using namespace Shipping;

/******************************************************************************
** Location Impl
******************************************************************************/
Location::Location(LocationType _locType){


}

void Location::segmentAdd(Fwk::Ptr<Segment> _segment){


}

void Location::segmentRemove(Fwk::Ptr<Segment> _segment){


}

Fwk::Ptr<Segment> Location::segmentAtIndex(unsigned int _index){

	return NULL;
}

/******************************************************************************
** Segment Impl
******************************************************************************/
Segment::Segment(SegmentType _segType){


}

void Segment::sourceIs(Location::Ptr _loc){

}

void Segment::returnSegmentIs(Segment::Ptr _returnSegment){


}

/******************************************************************************
** Conn Impl
******************************************************************************/
vector<Path::Ptr> Conn::ConstrainedGraph(Location::Ptr loc, Mile distance, 
														Cost cost, Time time, Segment::Expedite expedited) {
	vector<Path::Ptr> path;
	return path; 
}
vector<Path::Ptr> Conn::Connections(Location::Ptr start, Location::Ptr end){
	
	vector<Path::Ptr> path;
	return path; 
}

/******************************************************************************
** LocationReactor Impl
******************************************************************************/
void LocationReactor::onLocationNew(){


}
void LocationReactor::onLocationDel(){



}


/******************************************************************************
** SegmentReactor Impl
******************************************************************************/
void SegmentReactor::onSegmentNew(){

}
void SegmentReactor::onSegmentDel(){

}
void SegmentReactor::onSegmentSource(){

}
void SegmentReactor::onSegmentReturn(){

}
void SegmentReactor::onSegmentExpedite(){

}

/******************************************************************************
** Engine Impl
******************************************************************************/
Engine::~Engine(){

}

Location::Ptr Engine::locationNew(std::string name, Location::LocationType _locationType){

	return NULL; //placeholder
}
Segment::Ptr Engine::segmentNew(std::string name, Segment::SegmentType _segmentType){

	return NULL; //placeholder
}

/******************************************************************************
** Stats Impl
******************************************************************************/
void Stats::onLocationNew( Location::Ptr loc ){

}
void Stats::onSegmentNew( Segment::Ptr seg ){

}
void Stats::onSegmentExpedite( Segment::Ptr seg ){

}










