#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"

namespace Shipping {


class Engine : public Fwk::PtrInterface<Engine> {
public:
	typedef Fwk::Ptr<Engine const> PtrConst;
	typedef Fwk::Ptr<Engine> Ptr;

	/* note: constructor must instantiate fleets & stats */
	static Engine::Ptr EngineNew() {
		Ptr m = new Engine();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;	
	}
	~Engine();

	Location::Ptr locationNew(std::string name, Location::LocationType _locationType);
	Segment::Ptr segmentNew(std::string name, Segment::SegmentType _segmentType);
	
	Fleet::Ptr boatFleet() { return boatFleet_; }
	Fleet::Ptr planeFleet() { return planeFleet_; }
	Fleet::Ptr truckFleet() { return truckFleet_; }
	
	class NotifieeConst : public virtual Fwk::PtrInterface<NotifeeConst> {
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
				m->referencesDec(1);
				// decr. refer count to compensate for initial val of 1
				return m;
		}
		// Constructors ======================================
	protected:
		Engine::PtrConst notifier_;
		NotifieeConst();
	};
	
	class Notifiee : public virtual NotifieeConst, public virtual Fwk::PtrInterface<Notifee> {
	public:
		typedef Fwk::Ptr<Notifiee const> PtrConst;
		typedef Fwk::Ptr<Notifiee> Ptr;
		Engine::PtrConst notifier() const { return NotifieeConst::notifier(); }
		Segment::Ptr notifier() { return const_cast<Engine *>(NotifieeConst::notifier().ptr()); }
		// Non-const interface ==================================
		static Notifiee::Ptr NotifieeIs() {
			Ptr m = new Notifiee();
				m->referencesDec(1);
				// decr. refer count to compensate for initial val of 1
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


class Location : public Fwk::PtrInterface<Location> {
public:
	typedef Fwk::Ptr<Location const> PtrConst;
	typedef Fwk::Ptr<Location> Ptr;
	
	static Location::Ptr LocationNew(LocationType _locType) {
		Ptr m = new Location(_locType);
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
  	return m;
	}
	~Location();

	enum LocationType {
		undefinedLoc_ = 0,
		customerLoc_ = 1,
		portLoc_ = 2,
		boatTerminalLoc_ = 3,
		planeTerminalLoc_ = 4,
		truckTerminalLoc_ = 5
	};

	LocationType locationType() const {return locType_; }
	static inline LocationType undefinedLoc() { return undefinedLoc_; }
	static inline LocationType customerLoc() { return customerLoc_; }
	static inline LocationType portLoc() { return portLoc_; }
	static inline LocationType terminalLoc() { return terminalLoc_; }
		
	virtual void segmentAdd(Segment::Ptr _segment);
	virtual void segmentRemove(Segment::Ptr _segment);
	unsigned int segments() const { return segments_.length(); }
	virtual Segment::Ptr segmentAtIndex(unsigned int _index);

	class NotifieeConst : public virtual Fwk::PtrInterface<NotifeeConst> {
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
				m->referencesDec(1);
				// decr. refer count to compensate for initial val of 1
				return m;
		}
		// Constructors ======================================
		protected:
		Location::PtrConst notifier_;
		NotifieeConst();
	};

	class Notifiee : public virtual NotifieeConst, public virtual Fwk::PtrInterface<Notifee> {
	public:
		typedef Fwk::Ptr<Notifiee const> PtrConst;
		typedef Fwk::Ptr<Notifiee> Ptr;
		Location::PtrConst notifier() const { return NotifieeConst::notifier(); }
		Location::Ptr notifier() { return const_cast<Location *>(NotifieeConst::notifier().ptr()); }
		// Non-const interface ==================================
		static Notifiee::Ptr NotifieeIs() {
			Ptr m = new Notifiee();
				m->referencesDec(1);
				// decr. refer count to compensate for initial val of 1
				return m;
		}
	// Constructors ========================================
	protected:
		Notifiee();
	};

protected:
	explicit Location(LocationType _locType);
	LocationType locType_;
	std::vector<Segment::Ptr> segments_;
};

class Customer : public Location {
public:
	typedef Fwk::Ptr<Customer const> PtrConst;
	typedef Fwk::Ptr<Customer> Ptr;

	static Customer::Ptr CustomerIs() {
		Ptr m = new Customer();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
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
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
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
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~BoatTerminal();
	void segmentAdd(Segment::Ptr _segment);
	void segmentRemove(Segment::Ptr _segment);
	protected:
	explicit BoatTerminal();
};

class PlaneTerminal : public Location {
public:
	typedef Fwk::Ptr<PlaneTerminal const> PtrConst;
	typedef Fwk::Ptr<PlaneTerminal> Ptr;
	static PlaneTerminal::Ptr PlaneTerminalIs() {
		Ptr m = new PlaneTerminal();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
		return m;
	}
	~PlaneTerminal();
	void segmentAdd(Segment::Ptr _segment);
	void segmentRemove(Segment::Ptr _segment);
protected:
	explicit PlaneTerminal();
};

class TruckTerminal : public Location {
public:
	typedef Fwk::Ptr<TruckTerminal const> PtrConst;
	typedef Fwk::Ptr<TruckTerminal> Ptr;
	static TruckTerminal::Ptr TruckTerminalIs() {
		Ptr m = new TruckTerminal();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~TruckTerminal();
	void segmentAdd(Segment::Ptr _segment);
	void segmentRemove(Segment::Ptr _segment);
protected:
	explicit TruckTerminal();
};

class Segment : public Fwk::PtrInterface<Segment> {
public:
	typedef Fwk::Ptr<Segment const> PtrConst;
	typedef Fwk::Ptr<Segment> Ptr;
	static Segment::Ptr SegmentNew(SegmentType _segType) {
		Ptr m = new Segment(_segType);
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~Segment();

	Location::Ptr source() { return source_; }
	virtual void sourceIs(Location::Ptr _loc);
	Mile distance() { return distance_; }
	Segment::Ptr returnSegment() { return returnSegment_; }
	virtual void returnSegmentIs(Segment::Ptr _returnSegment);
	Difficulty difficulty() { return diff_; }
	void difficultyIs(Difficulty _diff) { diff_ = _diff; }
	Expedite expedite() { return expedite_; }
	void expediteIs(Expedite _expedite) { expedite_ = _expedite; }

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

	class NotifieeConst : public virtual Fwk::PtrInterface<NotifeeConst> {
	public:
		typedef Fwk::Ptr<NotifieeConst const> PtrConst;
		typedef Fwk::Ptr<NotifieeConst> Ptr;
	
		Engine::PtrConst notifier() const { return notifier_; }
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
				m->referencesDec(1);
				// decr. refer count to compensate for initial val of 1
				return m;
		}
		// Constructors ======================================
	protected:
		Engine::PtrConst notifier_;
		NotifieeConst();
	};
	
	class Notifiee : public virtual NotifieeConst, public virtual Fwk::PtrInterface<Notifee> {
	public:
		typedef Fwk::Ptr<Notifiee const> PtrConst;
		typedef Fwk::Ptr<Notifiee> Ptr;
		Engine::PtrConst notifier() const { return NotifieeConst::notifier(); }
		Segment::Ptr notifier() { return const_cast<Segment *>(NotifieeConst::notifier().ptr()); }
		// Non-const interface ==================================
		static Notifiee::Ptr NotifieeIs() {
			Ptr m = new Notifiee();
				m->referencesDec(1);
				// decr. refer count to compensate for initial val of 1
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
	Expedite expediteSupport_;
};


class BoatSegment : public Segment { 
public:
	typedef Fwk::Ptr<BoatSegment const> PtrConst;
	typedef Fwk::Ptr<BoatSegment> Ptr;
	static BoatSegment::Ptr BoatSegmentIs() {
		Ptr m = new BoatSegment();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~BoatSegment();
	void sourceIs(Location::Ptr _loc);
	void returnSegmentIs(Segment::Ptr _returnSegment);
	
protected:
	explicit BoatSegment();

};

class PlaneSegment : public Segment {
public:
	typedef Fwk::Ptr<PlaneSegment const> PtrConst;
	typedef Fwk::Ptr<PlaneSegment> Ptr;
	static PlaneSegment::Ptr PlaneSegmentIs() {
		Ptr m = new PlaneSegment();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~PlaneSegment();
	void sourceIs(Location::Ptr _loc);
	void returnSegmentIs(Segment::Ptr _returnSegment);
	
protected:	
	explicit PlaneSegment();

};

class TruckSegment : public Segment {
public:
	typedef Fwk::Ptr<TruckSegment const> PtrConst;
	typedef Fwk::Ptr<TruckSegment> Ptr;
	static TruckSegment::Ptr TruckSegmentIs() {
		Ptr m = new TruckSegment();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~TruckSegment();

	void sourceIs(Location::Ptr _loc);
	void returnSegmentIs(Segment::Ptr _returnSegment);
protected:
	explicit TruckSegment();
};


class Fleet : public Fwk::PtrInterface<Fleet> {
public:
	typedef Fwk::Ptr<Fleet const> PtrConst;
	typedef Fwk::Ptr<Fleet> Ptr;
static Fleet::Ptr FleetNew() {
Ptr m = new Fleet();
m->referencesDec(1);
// decr. refer count to compensate for initial val of 1
      	return m;	
}
~Fleet();
	Capacity capacity() { return capacity_; }
	void capacityIs(Capacity _capacity) { capacity_ = _capacity; }
	Cost cost() { return cost_; }
	void costIs(Cost _cost) { cost_ = _cost; }
	Speed speed() { return capacity_; }
	void speedIs(Speed _speed) { speed_ = _speed; }
protected:
explicit Fleet();
	Capacity capacity_;
	Cost cost_;
	Speed speed_;
}

class Stats : public Engine::Notifee {
public:
	typedef Fwk::Ptr<Stats const> PtrConst;
	typedef Fwk::Ptr<Stats> Ptr;
	static Stats::Ptr StatsNew(Engine* eng) {
		Ptr m = new Stats(eng);
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;
	}
	~Stats();
	U32 customers() { return customers_; }
	U32 ports() { return ports_; }
	U32 boatTerminals { return boatTerminals_; }
	U32 planeTerminals { return planeTerminals_; }
	U32 truckTerminals { return truckTerminals_; }
	U32 boatSegments { return boatSegments_; }
	U32 planeSegments { return planeSegments_; }
	U32 truckSegments { return truckSegments_; }
	U32 expediteSegments { return expediteSegments_; }
	float expeditePercentage { return ((float)expediteSegments)/(boatSegments_ + planeSegments_ + truckSegments_); }
	
	void onLocationNew( Location::Ptr loc );
	void onSegmentNew( Segment::Ptr seg );
  void onSegmentExpediteChange( Segment::Ptr seg, Expedite oldExp );

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

class Conn : public Fwk::PtrInterface<Conn> {
public:
	typedef Fwk::Ptr<Conn const> PtrConst;
	typedef Fwk::Ptr<Conn> Ptr;
	static Conn::Ptr ConnNew() {
		Ptr m = new Conn();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
	  return m;	
	}
	~Conn();
	static std::vector<Path::Ptr> ConstrainedGraph(Location::Ptr loc, Mile distance, Cost cost, Time time, Segment::Expedite expedited);
	static std::vector<Path::Ptr> Connections(Location::Ptr start, Location::Ptr end);

protected:
	explicit Conn();
};

class Path : public Fwk::PtrInterface<Path> {
public:
	typedef Fwk::Ptr<Path const> PtrConst;
	typedef Fwk::Ptr<Path> Ptr;
	static Path::Ptr PathNew() {
			Ptr m = new Path();
		m->referencesDec(1);
		// decr. refer count to compensate for initial val of 1
    return m;	
	}
	static Path::Ptr PathNew(Path::Ptr _path);
	~Path();

	Distance distance() { return distance_; }
	Cost cost() { return cost_; }
	Time time() { return time_; }
	Segment::Expedite expedited() { return expedited_; }
	
	void segmentAdd(Segment::Ptr _segment);
	U32 segments(){ return segments_.length(); }
	std::string toString();
protected:
	explicit Path();
	Segment::Expedite expedited_;
	Distance distance_;
	Cost cost_;
	Time time_;
	Segment::Expedite expedited_;
	std::list<Segment::Ptr> segments_;
};

// ordinal types
class Mile : public Ordinal<Mile, unsigned int> {
public:
	Mile(unsigned int num) : Ordinal<Mile, unsigned int>(num);
};

class Difficulty : public Ordinal<Difficulty, float> {
public:
	Difficulty(float num) : Ordinal<Difficulty, float>(num);
};

class Speed : public Ordinal<Speed, float> {
public:
	Speed(float num) : Ordinal<Speed, float>(num);
};

class Cost : public Ordinal<Cost, float> {
public:
	Cost(float num) : Ordinal<Cost, float>(num);
};

class Capacity : public Ordinal<Capacity, float> {
public:
	Capacity(float num) : Ordinal<Capacity, float>(num);

};

class Time : public Ordinal<Time, unsigned int> {
public:
	Time(unsigned int num) : Ordinal<Time, unsigned int>(num);
}

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












} /* end shipping namespace */

#endif
