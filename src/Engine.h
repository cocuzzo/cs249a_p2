#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>

#include "Types.h"
#include "Nominal.h"
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"

namespace Shipping {

#define MAX_BUF 64

// ordinal types
class Mile : public Ordinal<Mile, int> {
public:
	Mile(int num = 0) : Ordinal<Mile, int>(num) {
		if (num < 0) {
			std::ostringstream err;
			err << "Mile value error: " << num;
			throw err.str();
		}
	}
	std::string toString() { return std::to_string(value_); }
};

class Difficulty : public Ordinal<Difficulty, double> {
public:
	Difficulty(double num = 1.0) : Ordinal<Difficulty, double>(num) {
		if (num < 1.0 || num > 5.0) {
			std::ostringstream err;
			err << "Difficulty value error: " << num;
			throw err.str();
		}
	}
	std::string toString() { 
		char buf[MAX_BUF];
		std::sprintf(buf, "%.2f", value_);
		return std::string(buf);
	}
};

class Speed : public Ordinal<Speed, double> {
public:
	Speed(double num = 0.0) : Ordinal<Speed, double>(num) {
		if (num < 0) {
			std::ostringstream err;
			err << "Speed value error: " << num;
			throw err.str();
		}
	}
	std::string toString() { 
		char buf[MAX_BUF];
		std::sprintf(buf, "%.2f", value_);
		return std::string(buf);
	}
};

class Cost : public Ordinal<Cost, double> {
public:
	Cost(int num = 0.0) : Ordinal<Cost, double>(num) {
		if (num < 0) {
			std::ostringstream err;
			err << "Cost value error: " << num;
			throw err.str();
		}
	}
	std::string toString() { 
		char buf[MAX_BUF];
		std::sprintf(buf, "%.2f", value_);
		return std::string(buf);
	}
};

class Capacity : public Ordinal<Capacity, int> {
public:
	Capacity(int num = 0.0) : Ordinal<Capacity, int>(num) {
		if (num < 0) {
			std::ostringstream err;
			err << "Capacity value error: " << num;
			throw err.str();
		}
	}
	std::string toString() { return std::to_string(value_); }
};

class Time : public Ordinal<Time, int> {
public:
	Time(int num = 0) : Ordinal<Time, int>(num) {
		if (num < 0) {
			std::ostringstream err;
			err << "Time value error: " << num;
			throw err.str();
		}
	}
	std::string toString() { return std::to_string(value_); }
};

class Segment; //Forward declaration for Location

class Location : public Fwk::PtrInterface<Location> {
public:
	typedef Fwk::Ptr<Location const> PtrConst;
	typedef Fwk::Ptr<Location> Ptr;
	
	inline std::string name() const { return name_; }
	
	enum LocationType {
		undefinedLoc_ = 0,
		customerLoc_ = 1,
		portLoc_ = 2,
		boatTerminalLoc_ = 3,
		planeTerminalLoc_ = 4,
		truckTerminalLoc_ = 5
	};

	inline LocationType locationType() const {return locType_; }
	static inline LocationType undefinedLoc() { return undefinedLoc_; }
	static inline LocationType customerLoc() { return customerLoc_; }
	static inline LocationType portLoc() { return portLoc_; }
	static inline LocationType boatTerminalLoc() { return boatTerminalLoc_; }
	static inline LocationType planeTerminalLoc() { return planeTerminalLoc_; }
	static inline LocationType truckTerminalLoc() { return truckTerminalLoc_; }
	
	static Location::Ptr LocationNew(const std::string& _name, LocationType _locType) {
		Ptr m = new Location(_name, _locType);
  	return m;
	}
	~Location();
	virtual void segmentAdd(Fwk::Ptr<Segment> _segment);
	virtual void segmentRemove(Fwk::Ptr<Segment> _segment);
	inline U32 segments() const { return segments_.size(); }
	virtual Fwk::Ptr<Segment> segmentAtIndex(unsigned int _index);

	class NotifieeConst : public virtual Fwk::PtrInterface<NotifieeConst> {
	public:
		typedef Fwk::Ptr<NotifieeConst const> PtrConst;
		typedef Fwk::Ptr<NotifieeConst> Ptr;
		
		Location::PtrConst notifier() const { return notifier_; }
		// Non-const interface =================================
		~NotifieeConst();
		virtual void notifierIs(const Location::PtrConst& _notifier);
		
		static NotifieeConst::Ptr NotifieeConstIs() {
				Ptr m = new NotifieeConst();
				return m;
		}
		// Constructors ======================================
		protected:
		Location::PtrConst notifier_;
		NotifieeConst() {}
	};
	//PEO
	class Notifiee : public virtual NotifieeConst /*, public virtual Fwk::PtrInterface<Notifiee> */ {
	public:
		typedef Fwk::Ptr<Notifiee const> PtrConst;
		typedef Fwk::Ptr<Notifiee> Ptr;
		Location::PtrConst notifier() const { return NotifieeConst::notifier(); }
		Location::Ptr notifier() { return const_cast<Location *>(NotifieeConst::notifier().ptr()); }
		// Non-const interface ==================================
		static Notifiee::Ptr NotifieeIs() {
			Ptr m = new Notifiee();
				return m;
		}
	// Constructors ========================================
	protected:
		Notifiee() {}
	};

protected:
	explicit Location(const std::string& _name, LocationType _locType);
	LocationType locType_;
	std::string name_;
	std::list<Fwk::Ptr<Segment>> segments_;

	NotifieeConst *notifiee_;
  void newNotifiee( Location::NotifieeConst * n ) const {
    Location* me = const_cast<Location*>(this);
    me->notifiee_ = n;
  }
	void deleteNotifiee() const {
    Location* me = const_cast<Location*>(this);
    me->notifiee_ = NULL;
  }
};

class Customer : public Location {
public:
	typedef Fwk::Ptr<Customer const> PtrConst;
	typedef Fwk::Ptr<Customer> Ptr;

	static Customer::Ptr CustomerIs() {
		Ptr m = new Customer();
    return m;
	}	
	
protected:
	Customer();
};

class Port : public Location {
public:
	typedef Fwk::Ptr<Port const> PtrConst;
	typedef Fwk::Ptr<Port> Ptr;
	static Port::Ptr PortIs() {
		Ptr m = new Port();
		return m;
	}
	
	
protected:
	Port();
};

class BoatTerminal : public Location {
public:
	typedef Fwk::Ptr<BoatTerminal const> PtrConst;
	typedef Fwk::Ptr<BoatTerminal> Ptr;
	static BoatTerminal::Ptr BoatTerminalIs() {
		Ptr m = new BoatTerminal();
    return m;
	}
	
	
protected:
	BoatTerminal();
};

class PlaneTerminal : public Location {
public:
	typedef Fwk::Ptr<PlaneTerminal const> PtrConst;
	typedef Fwk::Ptr<PlaneTerminal> Ptr;
	static PlaneTerminal::Ptr PlaneTerminalIs() {
		Ptr m = new PlaneTerminal();
		return m;
	}
	
	
protected:
	PlaneTerminal();
};

class TruckTerminal : public Location {
public:
	typedef Fwk::Ptr<TruckTerminal const> PtrConst;
	typedef Fwk::Ptr<TruckTerminal> Ptr;
	static TruckTerminal::Ptr TruckTerminalIs() {
		Ptr m = new TruckTerminal();
    return m;
	}
	
	
protected:
	TruckTerminal();
};

class Segment : public Fwk::PtrInterface<Segment> {
public:
	typedef Fwk::Ptr<Segment const> PtrConst;
	typedef Fwk::Ptr<Segment> Ptr;
	
	inline std::string name() const { return name_; }
	
	enum SegmentType {
		undefinedSeg_ = 0,
		boatSeg_ = 1,
		planeSeg_ = 2,
		truckSeg_ = 3
	};
	SegmentType segmentType() const {return segType_; }
	static inline SegmentType undefinedSeg() { return undefinedSeg_; }
	static inline SegmentType boatSeg() { return boatSeg_; }
	static inline SegmentType planeSeg() { return planeSeg_; }
	static inline SegmentType truckSeg() { return truckSeg_; }

	enum Expedite {
		unsupported_ = 1,
		supported_ = 2
	};
	
	static inline Expedite unsupported() { return unsupported_; }
	static inline Expedite supported() { return supported_; }
	
	static Segment::Ptr SegmentNew(const std::string& _name, SegmentType _segType) {
		Ptr m = new Segment(_name, _segType);
    return m;
	}
	~Segment();

	inline Location::Ptr source() const { return source_; }
	virtual void sourceIs(Location::Ptr _loc);
	inline Mile length() const { return length_; }
	void lengthIs(Mile _length) { length_ = _length; } 
	inline Segment::Ptr returnSegment() const { return returnSegment_; }
	virtual void returnSegmentIs(Segment::Ptr _returnSegment);
	inline Difficulty difficulty() const { return diff_; }
	void difficultyIs(Difficulty _diff) { diff_ = _diff; }
	inline Expedite expedite() const { return expedite_; }
	void expediteIs(Expedite _expedite);
	
	class NotifieeConst : public virtual Fwk::PtrInterface<NotifieeConst> {
	public:
		typedef Fwk::Ptr<NotifieeConst const> PtrConst;
		typedef Fwk::Ptr<NotifieeConst> Ptr;
	
		Segment::PtrConst notifier() const { return notifier_; }
		// Non-const interface =================================
		~NotifieeConst();
		virtual void notifierIs(const Segment::PtrConst& _notifier);
  
    virtual void onSegmentSource() {}
		virtual void onSegmentReturn() {}
		virtual void onSegmentExpedite() {}
		
		static NotifieeConst::Ptr NotifieeConstIs() {
				Ptr m = new NotifieeConst();
				return m;
		}
		// Constructors ======================================
	protected:
		Segment::PtrConst notifier_;
		NotifieeConst() {}
	};
	
	//PEO
	class Notifiee : public virtual NotifieeConst /*, public virtual Fwk::PtrInterface<Notifiee> */{
	public:
		typedef Fwk::Ptr<Notifiee const> PtrConst;
		typedef Fwk::Ptr<Notifiee> Ptr;
		Segment::PtrConst notifier() const { return NotifieeConst::notifier(); }
		Segment::Ptr notifier() { return const_cast<Segment *>(NotifieeConst::notifier().ptr()); }
		// Non-const interface ==================================
		static Notifiee::Ptr NotifieeIs() {
			Ptr m = new Notifiee();
			return m;
		}
		// Constructors ========================================
	protected:
		Notifiee() {}
	};

protected:
	explicit Segment(const std::string& _name, SegmentType _segType);
	SegmentType segType_;
	std::string name_;
	Location::Ptr source_;
	Mile length_;
	Segment::Ptr returnSegment_;
	Difficulty diff_;
	Expedite expedite_;

	NotifieeConst *notifiee_;
  void newNotifiee( Segment::NotifieeConst * n ) const {
    Segment* me = const_cast<Segment*>(this);
    me->notifiee_ = n;
  }
	void deleteNotifiee() const {
    Segment* me = const_cast<Segment*>(this);
    me->notifiee_ = NULL;
  }
};


class BoatSegment : public Segment { 
public:
	typedef Fwk::Ptr<BoatSegment const> PtrConst;
	typedef Fwk::Ptr<BoatSegment> Ptr;
	static BoatSegment::Ptr BoatSegmentIs() {
		Ptr m = new BoatSegment();
    return m;
	}
	
	
protected:
	BoatSegment();

};

class PlaneSegment : public Segment {
public:
	typedef Fwk::Ptr<PlaneSegment const> PtrConst;
	typedef Fwk::Ptr<PlaneSegment> Ptr;
	static PlaneSegment::Ptr PlaneSegmentIs() {
		Ptr m = new PlaneSegment();
    return m;
	}
	
	
protected:	
	PlaneSegment();

};

class TruckSegment : public Segment {
public:
	typedef Fwk::Ptr<TruckSegment const> PtrConst;
	typedef Fwk::Ptr<TruckSegment> Ptr;
	static TruckSegment::Ptr TruckSegmentIs() {
		Ptr m = new TruckSegment();
    return m;
	}
	

protected:
	TruckSegment();
};


class Fleet : public Fwk::PtrInterface<Fleet> {
public:
	typedef Fwk::Ptr<Fleet const> PtrConst;
	typedef Fwk::Ptr<Fleet> Ptr;
	static Fleet::Ptr FleetNew() {
		Ptr m = new Fleet();
    return m;	
	}

	inline Capacity capacity() const { return capacity_; }
	void capacityIs(Capacity _capacity) { capacity_ = _capacity; }
	inline Cost cost() const { return cost_; }
	void costIs(Cost _cost) { cost_ = _cost; }
	inline Speed speed() const { return speed_; }
	void speedIs(Speed _speed) { speed_ = _speed; }
	
protected:
	Fleet() {}
	Capacity capacity_;
	Cost cost_;
	Speed speed_;
};


class Path : public Fwk::PtrInterface<Path> {
public:
  typedef Fwk::Ptr<Path const> PtrConst;
	typedef Fwk::Ptr<Path> Ptr;
	static Path::Ptr PathNew() {
		Ptr m = new Path();
    return m;	
	}
	static Path::Ptr PathNew(Path::Ptr _path); // copy constructor

	Mile distance() { return distance_; }
	Cost cost() { return cost_; }
	Time time() { return time_; }
	Segment::Expedite expedited() { return expedited_; }
	
	bool segmentAdd(Segment::Ptr _segment);
  U32 segments(){ return segments_.size(); }
  std::string toString();
  bool containsNode(Location::Ptr node);
	
protected:
	Path() { }
	Mile length_;
	Cost cost_;
	Time time_;
	Segment::Expedite expedited_;
	std::vector<Segment::Ptr> segments_;
	std::vector<Location::Ptr> nodes_;
};


class Engine;

class LocationReactor : public Location::Notifiee {
public:

	static LocationReactor * LocationReactorIs(Location *loc, Engine* owner) {
		LocationReactor *m = new LocationReactor(loc, owner);
		return m;
	}

protected:
	LocationReactor(Location *_loc, Engine* _owner) {
		notifierIs(_loc);
		owner_ = _owner;
	}

	Engine* owner_;
};

class SegmentReactor : public Segment::Notifiee {
public:

	static SegmentReactor * SegmentReactorIs(Segment* seg, Engine* owner) {
		SegmentReactor *m = new SegmentReactor(seg, owner);
		return m;
	}

	void onSegmentSource();
	void onSegmentReturn();
	void onSegmentExpedite(); 

protected:
	SegmentReactor(Segment* _seg, Engine* _owner) {
		notifierIs(_seg);
		owner_ = _owner;
	}

	Engine* owner_;
	Location::Ptr prevSource_;
	Segment::Ptr prevReturn_;
};


class Engine : public Fwk::PtrInterface<Engine> {
public:
	typedef Fwk::Ptr<Engine const> PtrConst;
	typedef Fwk::Ptr<Engine> Ptr;

	static Engine::Ptr EngineNew() {
		Ptr m = new Engine();
    return m;	
	}
	~Engine();

	Location::Ptr locationNew(const std::string& name, Location::LocationType locationType);
	Segment::Ptr segmentNew(const std::string& name, Segment::SegmentType segmentType);

	void locationDel(Location::Ptr loc);
	void segmentDel(Segment::Ptr seg);
	
	// look up capability for the client of the engine
	Location::Ptr location(const std::string& name);
	Segment::Ptr segment(const std::string& name);
	
	inline Fleet::Ptr boatFleet() const { return boatFleet_; }
	inline Fleet::Ptr planeFleet() const { return planeFleet_; }
	inline Fleet::Ptr truckFleet() const { return truckFleet_; }

	void handleSegmentSource( Segment::Ptr seg, Location::Ptr prevSource );
	void handleSegmentReturn( Segment::Ptr seg, Segment::Ptr prevReturn );
	void handleSegmentExpedite( Segment::Ptr seg );

	// Graph query methods
	std::vector<Path::Ptr> constrainedGraph(Location::Ptr loc, Mile distance, Cost cost, Time time, Segment::Expedite expedited);
	std::vector<Path::Ptr> connections(Location::Ptr start, Location::Ptr end);
	
	class NotifieeConst : public virtual Fwk::PtrInterface<NotifieeConst> {
	public:
		typedef Fwk::Ptr<NotifieeConst const> PtrConst;
		typedef Fwk::Ptr<NotifieeConst> Ptr;
	
		Engine::PtrConst notifier() const { return notifier_; }
		// Non-const interface =================================
		~NotifieeConst();
		virtual void notifierIs(const Engine::PtrConst& _notifier);
		virtual void onLocationNew( Location::Ptr loc ) {}
		virtual void onLocationDel( Location::Ptr loc ) {}
		virtual void onSegmentNew( Segment::Ptr seg ) {}
		virtual void onSegmentDel( Segment::Ptr seg ) {}
		virtual void onSegmentExpedite( Segment::Ptr seg ) {}
		static NotifieeConst::Ptr NotifieeConstIs() {
			Ptr m = new NotifieeConst();
			return m;
		}
		// Constructors ======================================
	protected:
		Engine::PtrConst notifier_;
		NotifieeConst() {}
	};
	
	//PEO
	class Notifiee : public virtual NotifieeConst /*, public virtual Fwk::PtrInterface<Notifiee> */ {
	public:
		typedef Fwk::Ptr<Notifiee const> PtrConst;
		typedef Fwk::Ptr<Notifiee> Ptr;
		Engine::PtrConst notifier() const { return NotifieeConst::notifier(); }
		Engine::Ptr notifier() { return const_cast<Engine *>(NotifieeConst::notifier().ptr()); }
		// Non-const interface ==================================
		static Notifiee::Ptr NotifieeIs() {
			Ptr m = new Notifiee();
				return m;
		}
		// Constructors ========================================
	protected:
		Notifiee() {}
	};

protected:
	Engine();
	
	std::map<std::string, LocationReactor::Ptr> locReactors_;
	std::map<std::string, SegmentReactor::Ptr> segReactors_;
	Fleet::Ptr boatFleet_;
	Fleet::Ptr planeFleet_;
	Fleet::Ptr truckFleet_;	

	NotifieeConst *notifiee_;
  void newNotifiee( Engine::NotifieeConst * n ) const {
    Engine* me = const_cast<Engine*>(this);
    me->notifiee_ = n;
  }
	void deleteNotifiee() const {
    Engine* me = const_cast<Engine*>(this);
    me->notifiee_ = NULL;
  }
};


class Stats : public Engine::Notifiee {
public:
	typedef Fwk::Ptr<Stats const> PtrConst;
	typedef Fwk::Ptr<Stats> Ptr;
	static Stats::Ptr StatsNew(Engine* eng) {
		Ptr m = new Stats(eng);
    return m;
	}
	~Stats();
	inline U32 customers() const { return customers_; }
	inline U32 ports() const { return ports_; }
	inline U32 boatTerminals() const { return boatTerminals_; }
	inline U32 planeTerminals() const { return planeTerminals_; }
	inline U32 truckTerminals() const { return truckTerminals_; }
	inline U32 boatSegments() const { return boatSegments_; }
	inline U32 planeSegments() const { return planeSegments_; }
	inline U32 truckSegments() const { return truckSegments_; }
	inline U32 expediteSegments() const { return expediteSegments_; }
	double expeditePercentage() const;
	
	void onLocationNew( Location::Ptr loc );
	void onLocationDel( Location::Ptr loc );
	void onSegmentNew( Segment::Ptr seg );
	void onSegmentDel( Segment::Ptr seg);
  void onSegmentExpedite( Segment::Ptr seg );

protected:
	Stats(Engine* eng) {
		notifierIs(eng);
		customers_ = 0;
		ports_ = 0;
		boatTerminals_ = 0;
		planeTerminals_ = 0;
		truckTerminals_ = 0;
		boatSegments_ = 0;
		planeSegments_ = 0;
		truckSegments_ = 0;
		expediteSegments_ = 0;
	}

	U32 customers_;
	U32 ports_;
	U32 boatTerminals_;
	U32 planeTerminals_;
	U32 truckTerminals_;
	U32 boatSegments_;
	U32 planeSegments_;
	U32 truckSegments_;
	U32 expediteSegments_;
};

} /* end shipping namespace */

#endif
