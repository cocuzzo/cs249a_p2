#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include <list>

#include "Types.h"
#include "Nominal.h"
#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"

namespace Shipping {


// ordinal types
class Mile : public Ordinal<Mile, unsigned int> {
public:
	Mile(unsigned int num) : Ordinal<Mile, unsigned int>(num){}
};

class Difficulty : public Ordinal<Difficulty, float> {
public:
	Difficulty(float num) : Ordinal<Difficulty, float>(num){}
};

class Speed : public Ordinal<Speed, float> {
public:
	Speed(float num) : Ordinal<Speed, float>(num){}
};

class Cost : public Ordinal<Cost, float> {
public:
	Cost(float num) : Ordinal<Cost, float>(num){}
};

class Capacity : public Ordinal<Capacity, float> {
public:
	Capacity(float num) : Ordinal<Capacity, float>(num){}

};

class Time : public Ordinal<Time, unsigned int> {
public:
	Time(unsigned int num) : Ordinal<Time, unsigned int>(num){}
};

class Segment; //Location needs to know about Segments before decl

class Location : public Fwk::PtrInterface<Location> {
public:
	typedef Fwk::Ptr<Location const> PtrConst;
	typedef Fwk::Ptr<Location> Ptr;
	
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
	
	static Location::Ptr LocationNew(LocationType _locType) {
		Ptr m = new Location(_locType);
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
		
		virtual void onLocationNew( Location::Ptr ) {}
		virtual void onLocationDel( Location::Ptr ) {}
		static NotifieeConst::Ptr NotifieeConstIs() {
				Ptr m = new NotifieeConst();
				return m;
		}
		// Constructors ======================================
		protected:
		Location::PtrConst notifier_;
		NotifieeConst();
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
		Notifiee();
	};

protected:
	explicit Location(LocationType _locType);
	LocationType locType_;
	std::vector<Fwk::Ptr<Segment> > segments_;
};

class Customer : public Location {
public:
	typedef Fwk::Ptr<Customer const> PtrConst;
	typedef Fwk::Ptr<Customer> Ptr;

	static Customer::Ptr CustomerIs() {
		Ptr m = new Customer();
    return m;
	}
	~Customer();
	
protected:
	explicit Customer();
};

class Port : public Location {
public:
	typedef Fwk::Ptr<Port const> PtrConst;
	typedef Fwk::Ptr<Port> Ptr;
	static Port::Ptr PortIs() {
		Ptr m = new Port();
		return m;
	}
	~Port();
	
protected:
	explicit Port();
};

class BoatTerminal : public Location {
public:
	typedef Fwk::Ptr<BoatTerminal const> PtrConst;
	typedef Fwk::Ptr<BoatTerminal> Ptr;
	static BoatTerminal::Ptr BoatTerminalIs() {
		Ptr m = new BoatTerminal();
    return m;
	}
	~BoatTerminal();
	
protected:
	explicit BoatTerminal();
};

class PlaneTerminal : public Location {
public:
	typedef Fwk::Ptr<PlaneTerminal const> PtrConst;
	typedef Fwk::Ptr<PlaneTerminal> Ptr;
	static PlaneTerminal::Ptr PlaneTerminalIs() {
		Ptr m = new PlaneTerminal();
		return m;
	}
	~PlaneTerminal();
	
protected:
	explicit PlaneTerminal();
};

class TruckTerminal : public Location {
public:
	typedef Fwk::Ptr<TruckTerminal const> PtrConst;
	typedef Fwk::Ptr<TruckTerminal> Ptr;
	static TruckTerminal::Ptr TruckTerminalIs() {
		Ptr m = new TruckTerminal();
    return m;
	}
	~TruckTerminal();
	
protected:
	explicit TruckTerminal();
};

class Segment : public Fwk::PtrInterface<Segment> {
public:
	typedef Fwk::Ptr<Segment const> PtrConst;
	typedef Fwk::Ptr<Segment> Ptr;
	
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
		unsupported_ = 0,
		supported_ = 1
	};
	
	static inline Expedite unsupported() { return unsupported_; }
	static inline Expedite supported() { return supported_; }
	
	static Segment::Ptr SegmentNew(SegmentType _segType) {
		Ptr m = new Segment(_segType);
    return m;
	}
	~Segment();

	inline Location::Ptr source() const { return source_; }
	virtual void sourceIs(Location::Ptr _loc);
	inline Mile distance() const { return distance_; }
	inline Segment::Ptr returnSegment() const { return returnSegment_; }
	virtual void returnSegmentIs(Segment::Ptr _returnSegment);
	inline Difficulty difficulty() const { return diff_; }
	void difficultyIs(Difficulty _diff) { diff_ = _diff; }
	inline Expedite expedite() const { return expedite_; }
	void expediteIs(Expedite _expedite) { expedite_ = _expedite; }

	
	class NotifieeConst : public virtual Fwk::PtrInterface<NotifieeConst> {
	public:
		typedef Fwk::Ptr<NotifieeConst const> PtrConst;
		typedef Fwk::Ptr<NotifieeConst> Ptr;
	
		Segment::PtrConst notifier() const { return notifier_; }
		// Non-const interface =================================
		~NotifieeConst();
		virtual void notifierIs(const Segment::PtrConst& _notifier);
  
		virtual void onSegmentNew( Segment::Ptr ) {}
    virtual void onSegmentDel( Segment::Ptr ) {}
    virtual void onSegmentSource( Segment::Ptr ) {}
		virtual void onSegmentReturn( Segment::Ptr ) {}
		virtual void onSegmentExpedite( Segment::Ptr ) {}
		
		static NotifieeConst::Ptr NotifieeConstIs() {
				Ptr m = new NotifieeConst();
				return m;
		}
		// Constructors ======================================
	protected:
		Segment::PtrConst notifier_;
		NotifieeConst();
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
		Notifiee();
	};

protected:
	explicit Segment(SegmentType _segType);
	SegmentType segType_;
	Location::Ptr source_;
	Mile distance_;
	Segment::Ptr returnSegment_;
	Difficulty diff_;
	Expedite expedite_;
};


class BoatSegment : public Segment { 
public:
	typedef Fwk::Ptr<BoatSegment const> PtrConst;
	typedef Fwk::Ptr<BoatSegment> Ptr;
	static BoatSegment::Ptr BoatSegmentIs() {
		Ptr m = new BoatSegment();
    return m;
	}
	~BoatSegment();
	
protected:
	explicit BoatSegment();

};

class PlaneSegment : public Segment {
public:
	typedef Fwk::Ptr<PlaneSegment const> PtrConst;
	typedef Fwk::Ptr<PlaneSegment> Ptr;
	static PlaneSegment::Ptr PlaneSegmentIs() {
		Ptr m = new PlaneSegment();
    return m;
	}
	~PlaneSegment();
	
protected:	
	explicit PlaneSegment();

};

class TruckSegment : public Segment {
public:
	typedef Fwk::Ptr<TruckSegment const> PtrConst;
	typedef Fwk::Ptr<TruckSegment> Ptr;
	static TruckSegment::Ptr TruckSegmentIs() {
		Ptr m = new TruckSegment();
    return m;
	}
	~TruckSegment();

protected:
	explicit TruckSegment();
};


class Fleet : public Fwk::PtrInterface<Fleet> {
public:
	typedef Fwk::Ptr<Fleet const> PtrConst;
	typedef Fwk::Ptr<Fleet> Ptr;
	static Fleet::Ptr FleetNew() {
		Ptr m = new Fleet();
    return m;	
	}
	~Fleet();
	inline Capacity capacity() const { return capacity_; }
	void capacityIs(Capacity _capacity) { capacity_ = _capacity; }
	inline Cost cost() const { return cost_; }
	void costIs(Cost _cost) { cost_ = _cost; }
	inline Speed speed() const { return speed_; }
	void speedIs(Speed _speed) { speed_ = _speed; }
	
protected:
	explicit Fleet();
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
	static Path::Ptr PathNew(Path::Ptr _path);
	~Path();

	Mile distance() { return distance_; }
	Cost cost() { return cost_; }
	Time time() { return time_; }
	Segment::Expedite expedited() { return expedited_; }
	
	void segmentAdd(Segment::Ptr _segment);
	U32 segments(){ return segments_.size(); }
	std::string toString();
protected:
	explicit Path();
	Mile distance_;
	Cost cost_;
	Time time_;
	Segment::Expedite expedited_;
	std::vector<Segment::Ptr> segments_;
};

class Conn : public Fwk::PtrInterface<Conn> {
public:
	typedef Fwk::Ptr<Conn const> PtrConst;
	typedef Fwk::Ptr<Conn> Ptr;
	static Conn::Ptr ConnNew() {
		Ptr m = new Conn();
	  return m;	
	}
	~Conn();
	std::vector<Path::Ptr> ConstrainedGraph(Location::Ptr loc, Mile distance, Cost cost, Time time, Segment::Expedite expedited);
	std::vector<Path::Ptr> Connections(Location::Ptr start, Location::Ptr end);

protected:
	explicit Conn();
};

class Engine;

class LocationReactor : public Location::Notifiee {
public:
	void onLocationNew(Location::Ptr loc);
	void onLocationDel(Location::Ptr loc); 

	static LocationReactor * LocationReactorIs(Location *loc, Engine* owner) {
		LocationReactor *m = new LocationReactor(loc, owner);
		return m;
	}
protected:
	LocationReactor(Location *_loc, Engine* _owner) : Location::Notifiee() {
		notifierIs(_loc);
		owner_ = _owner;
	}
	Engine* owner_;
};

class SegmentReactor : public Segment::Notifiee {
public:
	void onSegmentNew(Segment::Ptr seg);
	void onSegmentDel(Segment::Ptr seg);
	void onSegmentSource(Segment::Ptr seg);
	void onSegmentReturn(Segment::Ptr seg);
	void onSegmentExpedite(Segment::Ptr seg); 

	static SegmentReactor * SegmentReactorIs(Segment* seg, Engine* owner) {
		SegmentReactor *m = new SegmentReactor(seg, owner);
		return m;
	}
protected:
	SegmentReactor(Segment* _seg, Engine* _owner) : Segment::Notifiee() {
		notifierIs(_seg);
		owner_ = _owner;
	}
	Engine* owner_;
};


class Engine : public Fwk::PtrInterface<Engine> {
public:
	typedef Fwk::Ptr<Engine const> PtrConst;
	typedef Fwk::Ptr<Engine> Ptr;

	/* note: constructor must instantiate fleets & stats */
	static Engine::Ptr EngineNew() {
		Ptr m = new Engine();
    return m;	
	}
	~Engine();

	Location::Ptr locationNew(std::string name, Location::LocationType _locationType);
	Segment::Ptr segmentNew(std::string name, Segment::SegmentType _segmentType);
	
	inline Fleet::Ptr boatFleet() const { return boatFleet_; }
	inline Fleet::Ptr planeFleet() const { return planeFleet_; }
	inline Fleet::Ptr truckFleet() const { return truckFleet_; }
	
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
		NotifieeConst();
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
		Notifiee();
	};

protected:
	explicit Engine();
	std::vector<LocationReactor::Ptr> locReactors;
	std::vector<SegmentReactor::Ptr> segReactors;
	Fleet::Ptr boatFleet_;
	Fleet::Ptr planeFleet_;
	Fleet::Ptr truckFleet_;	
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
	inline float expeditePercentage() const { return ((float)expediteSegments_)/(boatSegments_ + planeSegments_ + truckSegments_); }
	
	void onLocationNew( Location::Ptr loc );
	void onSegmentNew( Segment::Ptr seg );
  void onSegmentExpedite( Segment::Ptr seg );

protected:
	Stats(Engine *eng) : Engine::Notifiee() {
		notifierIs(eng);
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
	float expeditePercentage_;
};



} /* end shipping namespace */

#endif
